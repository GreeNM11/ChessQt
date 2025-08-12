#include "clientwrap.h"
#include <QUuid>
#include <QDebug>

ClientWrap::ClientWrap(QTcpSocket* socket, QObject* parent)
    : QObject(parent), socket(socket) {

    clientId = QUuid::createUuid().toString(QUuid::WithoutBraces);

    connect(socket, &QTcpSocket::readyRead, this, &ClientWrap::onReadyRead);
    connect(socket, &QTcpSocket::disconnected, this, &ClientWrap::onDisconnect);
}

QString ClientWrap::getID() const { return clientId; }
QTcpSocket* ClientWrap::getSocket() const { return socket; }

void ClientWrap::onDisconnect() { emit clientDisconnect(this); }

// ---------------------- Received from Client + MainWindow --------------------- //
void ClientWrap::onReadyRead() {
    while (socket->canReadLine()) {
        QString line = socket->readLine().trimmed();
        receiveMessage(line);
    }
}

void ClientWrap::receiveMessage(const QString& msg) {
    if (msg == ""){ return; }
    QStringList parts = msg.split("|");
    if (parts[0] == "CREATE_GAME" && parts.size() >= 2) {
        emit createGameSession(this, parts[1] == '1'); // clientwrap, isWhite //
    }
    else if (parts[0] == "JOIN_GAME" && parts.size() >= 2) {
        emit joinGameSession(this, parts[1]); // clientwrap, join code //
    }
    else if (parts[0] == "SEND_MOVE" && parts.size() >= 4) {
        emit moveReceived(parts[1], (parts[2] == "1 "), parts[3]); // gameID, move, isWhite //
    }
    else if (parts[0] == "SEND_PLAYER_MESSAGE" && parts.size() >= 4) {
        emit playerMessageReceived(parts[1], parts[2], parts[3]); // gameID, playerName, msg //
    }
    else {
        emit serverMessage("Unknown Message received");
    }
}

// ---------------------- Senders to Local Client + MainWindow ---------------------- //
void ClientWrap::sendMessage(const QString& message) {
    // send through socket back to client //
    if (socket && socket->state() == QAbstractSocket::ConnectedState) {
        QByteArray data = message.toUtf8();  // convert string to bytes
        socket->write(data);
        socket->flush();
    }
}

void ClientWrap::createGameSession_S(QString gameID){
    sendMessage("CREATE_GAME_S|" + gameID + "\n");
}
void ClientWrap::joinGameSession_S(bool gameFound, bool isWhite){
    sendMessage("JOIN_GAME_S|" + QString(gameFound ? "1" : "0") + "|" + QString(isWhite ? "1" : "0") + "\n");
}
void ClientWrap::sendMove_S(QString move){
    sendMessage("SEND_PLAYER_MOVE_S|" + move + "\n");
}
void ClientWrap::sendPlayerMessage_S(QString playerName, QString msg){
    sendMessage("SEND_PLAYER_MESSAGE_S|" + playerName + "|" + msg + "\n");
}
void ClientWrap::sendErrorMessage_S(QString msg){
    sendMessage("SEND_ERROR_MESSAGE_S|" + msg + "\n");
}


