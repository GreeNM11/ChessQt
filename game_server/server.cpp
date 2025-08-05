#include "server.h"
#include <QDebug>

Server::Server(QObject *parent) : QObject(parent), server(new QTcpServer(this)), clientSocket(nullptr) {
    connect(server, &QTcpServer::newConnection, this, &Server::onNewConnection);
}

void Server::emitServerStatus(){
    if (!server->listen(QHostAddress::Any, 1234)) {
        emit newMessage("Server failed to start");
    }
    else {
        emit newMessage("Server Started at port 1234");
    }
}

void Server::onNewConnection() {
    QTcpSocket* clientSocket = server->nextPendingConnection();

    connect(clientSocket, &QTcpSocket::readyRead, this, [=]() {
        QByteArray data = clientSocket->readAll();
        QJsonParseError parseError;
        QJsonDocument doc = QJsonDocument::fromJson(data, &parseError);

        if (parseError.error == QJsonParseError::NoError && doc.isObject()) {
            QJsonObject obj = doc.object();

            QString type = obj["type"].toString();
            if (type == "move") {
                QString from = obj["from"].toString();
                QString to = obj["to"].toString();

                qDebug() << "Move received from client:" << from << "->" << to;
                // Now you can call your server-side chess logic
                //game->applyMove(from, to);
            }
        } else {
            qDebug() << '1';
        }
    });
}

void Server::onReadyRead() {
    QByteArray data = clientSocket->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);

    if (!doc.isNull() && doc.isObject()) {
        QJsonObject json = doc.object();
        emit newMessage( "Received JSON from client:"" << json;");
        sendBoardState(json);
    } else {
        emit newMessage("Invalid JSON received");
    }
}
void Server::sendBoardState(const QJsonObject &boardState) {
    if (clientSocket && clientSocket->isOpen()) {
        QJsonDocument doc(boardState);
        clientSocket->write(doc.toJson(QJsonDocument::Compact));
        clientSocket->flush();
    }
}
