#include "server.h"
#include <QDebug>

Server::Server(QObject *parent) : QObject(parent), server(new QTcpServer(this)) {
    connect(server, &QTcpServer::newConnection, this, &Server::onNewConnection);
}
void Server::emitServerStatus(){
    if (!server->listen(QHostAddress::Any, 1234)) {
        emit newMessage("Server failed to start");
    }
    else {
        emit newMessage("Server Started at port 1234");
    }
}

void Server::serverMessage(QString msg){ emit newMessage(msg); }

void Server::onNewConnection() {
    QTcpSocket* clientSocket = server->nextPendingConnection(); // gets the client that wanted to connect //
    ClientWrap* client = new ClientWrap(clientSocket, this);

    connect(client, &ClientWrap::serverMessage, this, &Server::serverMessage);
    connect(client, &ClientWrap::clientDisconnect, this, &Server::clientDisconnect);
    connect(client, &ClientWrap::createGameSession, this, &Server::newGameSession);
    connect(client, &ClientWrap::joinGameSession, this, &Server::joinGameSession);

    connect(client, &ClientWrap::moveReceived, this, &Server::moveReceived);
    connect(client, &ClientWrap::playerMessageReceived, this, &Server::playerMessageReceived);

    QString clientInfo = clientSocket->peerAddress().toString() + "|" + QString::number(clientSocket->peerPort());
    serverMessage("Client Connected: " + clientInfo);
}


void Server::clientDisconnect(ClientWrap* client){
    activeSessions.remove(client->getID());
    serverMessage("Client " + client->getID() + " Disconnected");
}

void Server::newGameSession(ClientWrap* client, bool isWhitef){
    QString gameId = QUuid::createUuid().toString(QUuid::WithoutBraces).left(4); // 4 char game id //
    // Create and store session
    GameSession* session = new GameSession;
    session->gameID = gameId;
    session->player1 = client;
    session->isWhite = isWhitef;

    activeSessions.insert(gameId, session); // adds to hash map of game sessions //
    client->createGameSession_S(gameId); // sends back to local client //
}

void Server::joinGameSession(ClientWrap* client, QString gameID){
    if (activeSessions.contains(gameID)){
        GameSession* session = activeSessions.value(gameID);
        session->player2 = client;
        client->joinGameSession_S(true, !session->isWhite); // gameFound | p2 isWhite parameters //
    }
    else{
        client->joinGameSession_S(false, false); // game not found | just false is dont care //
    }
}

void Server::moveReceived(QString gameID, bool isWhite, QString move){
    GameSession* session = activeSessions.value(gameID);

    int from_row = move.at(0).digitValue();
    int from_col = move.at(1).digitValue();
    int to_row = move.at(2).digitValue();
    int to_col = move.at(3).digitValue();

    bool validMove = false;
    QString flipMove;
    // Validates Move //
    if ((from_row >= 0 && from_row <= 7) && (from_col >= 0 && from_col <= 7) &&
        (to_row >= 0 && to_row <= 7) && (to_col >= 0 && to_col <= 7)) {
        // need to also check with security for legal moves //
        validMove = true;
        // '7 -' to flip coordinates since opponent has flipped board //
        int flipInt = (7-from_row) * 1000 + (7-from_col) * 100 + (7-to_row) * 10 + (7-to_col);
        flipMove = QString::number(flipInt);
        if (flipInt < 1000){ move = "0" + move; } // no 0 in front of int //
    }

    QString ErrorMessage = "";
    if (!validMove){
        if (isWhite){ ErrorMessage = "Whites move was out of bounds"; }
        else { ErrorMessage = "Blacks move was out of bounds"; }
        session->player1->sendErrorMessage_S(ErrorMessage);
        session->player2->sendErrorMessage_S(ErrorMessage);
        return;
    }
    else if(session->player1 == nullptr) {
        ErrorMessage = "White player not found";
        session->player2->sendErrorMessage_S(ErrorMessage);
        return;
    }
    else if(session->player2 == nullptr) {
        ErrorMessage = "Black player not found";
        session->player1->sendErrorMessage_S(ErrorMessage);
        return;
    }

    // If doesn't Error //
    if (isWhite){
        session->player1->sendMove_S(move);
        session->player2->sendMove_S(flipMove); // black flips white move //
    }
    else if (!isWhite){
        session->player1->sendMove_S(flipMove); // white flips black move //
        session->player2->sendMove_S(move);
    }
}

void Server::playerMessageReceived(QString gameID, QString playerName, QString msg){
    GameSession* session = activeSessions.value(gameID);
    session->player1->sendPlayerMessage_S(playerName, msg); // sends to both players so message only shows if received by server //
    session->player2->sendPlayerMessage_S(playerName, msg);
}









