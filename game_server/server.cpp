#include "server.h"

// ------------------------- Server Side Functions ------------------------- //

void Server::serverMessage(QString msg){ emit newMessage(msg); }

void Server::createDB(){
    database = std::make_unique<database_chess>();
    connect(database.get(), &database_chess::ServerMessage, this, &Server::serverMessage);
    database->connect();
}
void Server::emitServerStatus(){
    if (!server->listen(QHostAddress::Any, port)) {
        serverMessage("❌Server failed to start");
    }
    else {
        serverMessage("✅Server Started at port " + QString::number(port));
    }
}

void Server::onNewConnection() {
    QTcpSocket* clientSocket = server->nextPendingConnection(); // gets the client that wanted to connect //
    ClientWrap* client = new ClientWrap(clientSocket, this);

    connect(client, &ClientWrap::serverMessage, this, &Server::serverMessage);

    connect(client, &ClientWrap::registerUser, this, &Server::registerUser);
    connect(client, &ClientWrap::loginUser, this, &Server::loginUser);

    connect(client, &ClientWrap::clientDisconnect, this, &Server::clientDisconnect);
    connect(client, &ClientWrap::createGameSession, this, &Server::newGameSession);
    connect(client, &ClientWrap::joinGameSession, this, &Server::joinGameSession);

    connect(client, &ClientWrap::moveReceived, this, &Server::moveReceived);
    connect(client, &ClientWrap::playerMessageReceived, this, &Server::playerMessageReceived);

    QString clientInfo = clientSocket->peerAddress().toString() + "|" + QString::number(clientSocket->peerPort());
    serverMessage("Client Connected: " + clientInfo);
}
void Server::clientDisconnect(ClientWrap* client){
    serverMessage("Client " + client->getID() + " Disconnected");
    delete client;
}

void Server::registerUser(ClientWrap* client, const QString &user, const QString &pass){
    client->registerUser_S(database->registerUser(user, pass), user); // returns message of success or fail //
}
void Server::loginUser(ClientWrap* client, const QString &user, const QString &pass){
    client->loginUser_S(database->loginUser(user, pass), user);
}

// ---------------------- Chess Game Session Functions ---------------------- //

void Server::onSessionEmpty(const QString& gameID){

}

void Server::newGameSession(ClientWrap* client, bool isWhite){
    QString gameID = QUuid::createUuid().toString(QUuid::WithoutBraces).left(4); // 4 char game id //
    // Create and store session
    GameSession* session = new GameSession(gameID, client, isWhite);

    activeSessions.insert(gameID, session); // adds to hash map of game sessions //
    connect(session, &GameSession::sessionEmpty, this, &Server::onSessionEmpty);
    client->createGameSession_S(gameID); // sends back to local client //
}
void Server::joinGameSession(ClientWrap* client, const QString &gameID){
    if (activeSessions.contains(gameID)){
        GameSession* session = activeSessions.value(gameID);
        if (!session){ return; }
        session->player_join(client);
    }
    else{
        client->joinGameSession_S(false, false, 0, ""); // game not found //
    }
}

void Server::moveReceived(const QString &gameID, bool isWhite, const QString &move){
    GameSession* session = activeSessions.value(gameID);
    if (!session){ return; }
    session->validate_move(isWhite, move);
}
void Server::playerMessageReceived(const QString &gameID, const QString &playerName, const QString &msg){
    GameSession* session = activeSessions.value(gameID);
    if (!session){ return; }
    session->sendPlayerMessage(playerName, msg);
}

// --------------------------- Default Functions --------------------------- //

Server::Server(QObject *parent, int port) : QObject(parent), port(port), server(new QTcpServer(this)) {
    connect(server, &QTcpServer::newConnection, this, &Server::onNewConnection);
}






