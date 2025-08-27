#include "server.h"
#include <QDebug>

// ------------------------- Server Side Functions ------------------------- //

void Server::serverMessage(QString msg){ emit newMessage(msg); }

void Server::createDB(){
    database = std::make_unique<database_chess>();
    connect(database.get(), &database_chess::ServerMessage, this, &Server::serverMessage);
    database->connect();
}

void Server::emitServerStatus(){
    if (!server->listen(QHostAddress::Any, 7575)) {
        serverMessage("❌Server failed to start");
    }
    else {
        serverMessage("✅Server Started at port 7575");
    }
}

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

// ---------------------- Chess Game Session Functions ---------------------- //

void Server::newGameSession(ClientWrap* client, bool isWhite){
    QString gameID = QUuid::createUuid().toString(QUuid::WithoutBraces).left(4); // 4 char game id //
    // Create and store session
    GameSession* session = new GameSession(gameID, client, isWhite);

    activeSessions.insert(gameID, session); // adds to hash map of game sessions //
    client->createGameSession_S(gameID); // sends back to local client //
}

void Server::joinGameSession(ClientWrap* client, QString gameID){
    if (activeSessions.contains(gameID)){
        GameSession* session = activeSessions.value(gameID);
        session->setPlayer2(client);
        client->joinGameSession_S(true, session->player2_color()); // gameFound | p2 isWhite parameters //
    }
    else{
        client->joinGameSession_S(false, false); // game not found | just false is dont care //
    }
}

void Server::moveReceived(QString gameID, bool isWhite, QString move){
    GameSession* session = activeSessions.value(gameID);
    session->validate_move(isWhite, move);
}

void Server::playerMessageReceived(QString gameID, QString playerName, QString msg){
    GameSession* session = activeSessions.value(gameID);
    session->getPlayer1()->sendPlayerMessage_S(playerName, msg); // sends to both players so message only shows if received by server //
    session->getPlayer2()->sendPlayerMessage_S(playerName, msg);
}

// --------------------------- Default Functions --------------------------- //

Server::Server(QObject *parent) : QObject(parent), server(new QTcpServer(this)) {
    connect(server, &QTcpServer::newConnection, this, &Server::onNewConnection);
}






