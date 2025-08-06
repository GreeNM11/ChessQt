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

void Server::onNewConnection() {
    QTcpSocket* clientSocket = server->nextPendingConnection(); // gets the client that wanted to connect //
    QString clientID = QUuid::createUuid().toString();
    clientSockets.insert(clientID, clientSocket);

    QString clientInfo = clientSocket->peerAddress().toString() + "|" + QString::number(clientSocket->peerPort());
    emit newMessage("Client Connected: " + clientInfo);
}

void Server::newGameSession(Client* client, bool isWhite){
    QString gameID = QUuid::createUuid().toString(QUuid::WithoutBraces).mid(0, 4);
    GameSession* newSession = new GameSession;
    newSession->gameId = gameID;
    newSession->p1_isWhite = isWhite;
    newSession->player1 = client;
    newSession->player2 = nullptr;

    activeSessions.insert(gameID, newSession);
    emit newMessage("Created new game session ID: " + gameID);
}

void Server::joinGameSession(Client* client, QString gameID){
    if (activeSessions.contains(gameID)){
        GameSession* session = activeSessions.value(gameID);
        session->player2 = client;
        session->player2->receiveOponent(session->player1);
        session->player1->receiveOponent(session->player2);
    }
    else{
        client->invalidJoinCode();
    }
}
