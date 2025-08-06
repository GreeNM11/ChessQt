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

void Client::requestGameSession(bool isWhite){
    // send through socket //
}
void sendMove(const QString &move, const bool isWhite){
    // send through socket //
}


void Client::receiveOponent(Client* client){
     // send through socket //
    emit sendOpponent(client);
}
void Client::invalidJoinCode(){
     // send through socket //
    emit sendInvalidJoinCode();
}


