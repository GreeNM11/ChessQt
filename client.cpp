#include "client.h"
#include <QDebug>

Client::Client(QObject *parent) : QObject(parent) {
    socket = new QTcpSocket(this);
    connect(socket, &QTcpSocket::connected, this, &Client::onConnected);
    connect(socket, &QTcpSocket::readyRead, this, &Client::onReadyRead);

    socket->connectToHost("127.0.0.1", 1234); // Adjust IP as needed
}

void Client::onConnected() {
    qDebug() << "Connected to server.";
}

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

void Client::sendMove(const QString &from, const QString &to) {
    QJsonObject move;
    move["type"] = "move";
    move["from"] = from;
    move["to"] = to;

    QJsonDocument doc(move);
    socket->write(doc.toJson());
}
