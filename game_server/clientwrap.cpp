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

void ClientWrap::onReadyRead() {
    while (socket->canReadLine()) {
        QString line = socket->readLine().trimmed();
        receiveMessage(line);
    }
}

void ClientWrap::sendMessage(const QString& message) {
    // send through socket back to client //
    if (socket && socket->state() == QAbstractSocket::ConnectedState) {
        QByteArray data = message.toUtf8();  // convert string to bytes
        socket->write(data);
        socket->flush();
    } else {
        qDebug() << "Socket is not connected.";
    }

}

void ClientWrap::receiveMessage(const QString& msg) {
    QStringList parts = msg.split("|");

    if (parts[0] == "CREATE_SESSION" && parts.size() >= 2) {
        emit requestGameCreation(this, parts[1] == '1'); // color
    }
    else if (parts[0] == "MOVE" && parts.size() >= 3) {
        emit moveReceived(parts[1], parts[2], (parts[3] == '1')); // game ID, move, isWhite
    }
    else {
        qDebug() << "Unknown or malformed message:" << msg;
    }
}

