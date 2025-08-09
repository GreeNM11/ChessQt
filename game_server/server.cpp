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

void Server::moveReceived(QString gameID, QString move, bool isWhite){
    serverMessage("Move Received: " + QString((isWhite ? "White - " : "Black - ")) + move);
    GameSession* session = activeSessions.value(gameID);
    if (isWhite){
        session->player2->sendMove_S(move);
    }
    else{
        session->player1->sendMove_S(move);
    }
}
