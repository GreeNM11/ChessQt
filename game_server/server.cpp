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
    connect(client, &ClientWrap::requestGameCreation, this, &Server::newGameSession);
    connect(client, &ClientWrap::moveReceived, this, &Server::moveReceived);

    QString clientInfo = clientSocket->peerAddress().toString() + "|" + QString::number(clientSocket->peerPort());
    serverMessage("Client Connected: " + clientInfo);
}


void Server::clientDisconnect(ClientWrap* client){
    activeSessions.remove(client->getID());
    serverMessage("Client " + client->getID() + " Disconnected");
}

void Server::newGameSession(ClientWrap* client, bool isWhitef){
    QString gameId = QUuid::createUuid().toString(QUuid::WithoutBraces);

    // Create and store session
    GameSession* session = new GameSession;
    session->gameID = gameId;
    session->player1 = client;
    session->isWhite = isWhitef;

    activeSessions.insert(gameId, session);

    serverMessage("Created new game session ID: " + gameId);
}

void Server::joinGameSession(ClientWrap* client, QString gameID){
    if (activeSessions.contains(gameID)){
        GameSession* session = activeSessions.value(gameID);
        session->player2 = client;
    }
    else{

    }
}

void Server::moveReceived(QString gameID, QString move, bool isWhite){
    serverMessage("Move Received!: " + move + " : " + (isWhite ? "White" : "Black"));
}
