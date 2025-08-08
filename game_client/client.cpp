#include "game_client/client.h"
#include <QDebug>

Client::Client(QObject *parent) : QObject(parent) {
    socket = new QTcpSocket(this);
    connect(socket, &QTcpSocket::connected, this, &Client::onConnect);
    connect(socket, &QTcpSocket::disconnected, this, &Client::onConnect);

    socket->connectToHost("127.0.0.1", 1234);
}
void Client::onConnect() { emit connectedToServer(); }
void Client::onDisconnect() { emit disconnectedToServer(); }

void Client::sendMessage(const QString &message) {
    // send to server clientwrap through socket //
    if (socket && socket->state() == QAbstractSocket::ConnectedState) {
        QByteArray data = message.toUtf8();
        socket->write(data);
        socket->flush();
    } else {
    }
}

void Client::createGameSession(bool isWhite){
    QString data = "CREATE_GAME|" + QString(isWhite ? "1" : "0") + "\n";
    sendMessage(data);
}
void Client::sendMove(const QString gameID, const QString &move, const bool isWhite){
    QString data = "MOVE|" + gameID+ "|" + move +  "|" + QString::number(isWhite) + "\n";
    sendMessage(data);
}


