#include "clientwrap.h"
#include <QUuid>
#include <QDebug>

ClientWrap::ClientWrap(QTcpSocket* socket, QObject* parent)
    : QObject(parent), socket(socket) {

    clientId = QUuid::createUuid().toString(QUuid::WithoutBraces);

    connect(socket, &QTcpSocket::readyRead, this, &ClientWrap::onReadyRead);
    connect(socket, &QTcpSocket::disconnected, this, &ClientWrap::onDisconnect);
}

void ClientWrap::onDisconnect() { emit clientDisconnect(this); }

// ---------------------- Received from Client + MainWindow --------------------- //
void ClientWrap::onReadyRead() {
    while (socket->canReadLine()) {
        QString line = socket->readLine().trimmed();
        receiveMessage(line);
    }
}

void ClientWrap::receiveMessage(const QString &msg) {
    if (msg == ""){ return; }
    QStringList parts = msg.split("|");
    if (parts[0] == "CREATE_GAME" && parts.size() >= 2) {
        emit createGameSession(this, parts[1] == "1"); // clientwrap, isWhite //
    }
    else if (parts[0] == "JOIN_GAME" && parts.size() >= 2) {
        emit joinGameSession(this, parts[1]); // clientwrap, join code //
    }
    else if (parts[0] == "SEND_MOVE" && parts.size() >= 4) {
        emit moveReceived(parts[1], (parts[2] == "1"), parts[3]); // gameID, move, isWhite //
    }
    else if (parts[0] == "SEND_CHECKMATED" && parts.size() >= 3) {
        emit checkmatedReceived(parts[1], parts[2]); // gameID, move, isWhite //
    }
    else if (parts[0] == "SEND_PLAYER_MESSAGE" && parts.size() >= 4) {
        emit playerMessageReceived(parts[1], parts[2], parts[3]); // gameID, playerName, msg //
    }
    else if (parts[0] == "REGISTER_USER" && parts.size() >= 3) {
        emit registerUser(this, parts[1], parts[2]); // user, pass //
    }
    else if (parts[0] == "LOGIN_USER" && parts.size() >= 3) {
        emit loginUser(this ,parts[1], parts[2]); // user, pass //
    }
    else {
        emit serverMessage("Unknown Message received");
    }
}

// ---------------------- Senders to Local Client + MainWindow ---------------------- //
void ClientWrap::sendMessage(const QString &message) {
    // send through socket back to client //
    if (socket && socket->state() == QAbstractSocket::ConnectedState) {
        QByteArray data = message.toUtf8();  // convert string to bytes
        socket->write(data);
        socket->flush();
    }
}

void ClientWrap::registerUser_S(const QString &code, const QString &user){
    sendMessage("REGISTER_USER_S|" + code + "\n");
    clientUser = user;
}
void ClientWrap::loginUser_S(const QString &code, const QString &user){
    sendMessage("LOGIN_USER_S|" + code + "\n");
    clientUser = user;
}

void ClientWrap::createGameSession_S(const QString &gameID){
    sendMessage("CREATE_GAME_S|" + gameID + "\n");
}
void ClientWrap::joinGameSession_S(bool gameFound, bool isWhite, int code, const QString &moveList){
    sendMessage("JOIN_GAME_S|" + QString(gameFound ? "1" : "0") + "|" + QString(isWhite ? "1" : "0") + "|" +
    QString::number(code) + "|" + moveList + "\n");
}

void ClientWrap::sendMove_S(const QString &move){
    sendMessage("SEND_PLAYER_MOVE_S|" + move + "\n");
}
void ClientWrap::sendCheckmated_S(const QString &isWhite){
    sendMessage("SEND_CHECKMATED_S|" + isWhite + "\n");
}

void ClientWrap::sendPlayerMessage_S(const QString &playerName, const QString &msg){
    sendMessage("SEND_PLAYER_MESSAGE_S|" + playerName + "|" + msg + "\n");
}
void ClientWrap::sendErrorMessage_S(const QString &msg){
    if (msg != ""){
        sendMessage("SEND_ERROR_MESSAGE_S|" + msg + "\n");
    }
}


