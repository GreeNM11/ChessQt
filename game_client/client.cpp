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

void Client::onReadyRead() {
    QByteArray data = socket->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (!doc.isObject()) return;

    QJsonObject obj = doc.object();
    if (obj["type"] == "update") {
        qDebug() << "Received board update:" << obj;
        // TODO: update the chess board visually
    }
}

void Client::sendMove(const QString &move, const bool isWhite_move) {
    if (socket->state() == QAbstractSocket::ConnectedState) {
        QString message = move + "|" + (isWhite_move ? "1" : "0");
        socket->write(message.toUtf8());
        socket->flush();
        qDebug() << "Sent:" << message;
    }
    else {
        qDebug() << "Not connected to server.";
    }
}
