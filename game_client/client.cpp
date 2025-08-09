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
    QStringList parts = msg.split("|");
    if (parts[0] == "CREATE_GAME_S" && parts.size() >= 2) {
        emit createGameSession_S(parts[1]); // clientwrap, isWhite //
    }
    else if (parts[0] == "JOIN_GAME_S" && parts.size() >= 2) {
        emit joinGameSession_S(parts[1] == "1", parts[2] == "1"); // clientwrap, join code //
    }
    else if (parts[0] == "SEND_MOVE_S" && parts.size() >= 3) {
        emit move_S(parts[1] == "1"); // gameID, move, isWhite //
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
void Client::sendMove(const QString gameID, const QString &move, const bool isWhite){
    sendMessage("SEND_MOVE|" + gameID + "|" + move +  "|" + QString::number(isWhite) + "\n");
}


