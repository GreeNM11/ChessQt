#include "game_client/client.h"
#include <QDebug>

Client::Client(QObject *parent) : QObject(parent) {
    socket = new QTcpSocket(this);
    connect(socket, &QTcpSocket::connected, this, &Client::onConnect);
    connect(socket, &QTcpSocket::disconnected, this, &Client::onConnect);
    connect(socket, &QTcpSocket::readyRead, this, &Client::onReadyRead);

    socket->connectToHost("127.0.0.1", 1234);
}
void Client::onConnect() { emit connectedToServer(); }
void Client::onDisconnect() { emit disconnectedToServer(); }

// ---------------------- Received from ClientWrap + Server ---------------------- //
void Client::onReadyRead() {
    while (socket->canReadLine()) {
        QString line = socket->readLine().trimmed();
        receiveMessage(line);
    }
}

void Client::receiveMessage(const QString& msg) {
    //emit clientMessage(msg);
    QStringList parts = msg.split("|");
    if (parts[0] == "CREATE_GAME_S" && parts.size() >= 2) {
        emit createGameSession_S(parts[1]);
    }
    else if (parts[0] == "JOIN_GAME_S" && parts.size() >= 3) {
        emit joinGameSession_S(parts[1] == "1", parts[2] == "1");
    }
    else if (parts[0] == "SEND_PLAYER_MOVE_S" && parts.size() >= 2) {
        emit sendMove_S(parts[1]);
    }
    else if (parts[0] == "SEND_PLAYER_MESSAGE_S" && parts.size() >= 3) {
        emit sendPlayerMessage_S(parts[1], parts[2]);
    }
    else if (parts[0] == "SEND_ERROR_MESSAGE_S" && parts.size() >= 2) {
        emit sendErrorMessage_S(parts[1]);
    }
    else{
        emit clientMessage("Received Error Message from Server: " + msg);
    }
}
// ---------------------- Senders to ClientWrap + Server ---------------------- //
void Client::sendMessage(const QString &message) {
    // send to server clientwrap through socket //
    if (socket && socket->state() == QAbstractSocket::ConnectedState) {
        QByteArray data = message.toUtf8();
        socket->write(data);
        socket->flush();
    }
}

void Client::createGameSession(bool isWhite){
    sendMessage("CREATE_GAME|" + QString(isWhite ? "1" : "0") + "\n");
}
void Client::joinGameSession(const QString gameID){
    sendMessage("JOIN_GAME|" + gameID + "\n");
}
void Client::sendMove(const QString gameID, const QString move, const bool isWhite){
    // needs a space before | because it breaks without it //
    sendMessage("SEND_MOVE|" + gameID + "|" + QString::number(isWhite) + " |" + move + "\n");
}
void Client::sendPlayerMessage(QString gameID, const QString playerName, const QString msg){
    sendMessage("SEND_PLAYER_MESSAGE|" + gameID + "|" + playerName + "|" + msg + "\n");
}


