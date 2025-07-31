#include "server.h"
#include <QDebug>

Server::Server(QObject *parent) : QObject(parent), tcpServer(new QTcpServer(this)), clientSocket(nullptr) {
    connect(tcpServer, &QTcpServer::newConnection, this, &Server::onNewConnection);

    if (!tcpServer->listen(QHostAddress::Any, 1234)) {
        qDebug() << "Server failed to start!";
    } else {
        qDebug() << "Server started on port 1234";
    }
}

void Server::onNewConnection() {
    clientSocket = tcpServer->nextPendingConnection();
    connect(clientSocket, &QTcpSocket::readyRead, this, &Server::onReadyRead); // connects read //
    qDebug() << "Client connected!";
}

void Server::onReadyRead() {
    QByteArray data = clientSocket->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);

    if (!doc.isNull() && doc.isObject()) {
        QJsonObject json = doc.object();
        qDebug() << "Received JSON from client:" << json;

        // Example: echo back the same board to the client
        sendBoardState(json);
    } else {
        qDebug() << "Invalid JSON received:" << data;
    }
}

void Server::sendBoardState(const QJsonObject &boardState) {
    if (clientSocket && clientSocket->isOpen()) {
        QJsonDocument doc(boardState);
        clientSocket->write(doc.toJson(QJsonDocument::Compact));
        clientSocket->flush();
    }
}
