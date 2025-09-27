#include "game_client/client.h"
#include <QDebug>

Client::Client(QObject *parent, QString ip, int port) : QObject(parent) {
    socket = new QTcpSocket(this);
    connect(socket, &QTcpSocket::connected, this, &Client::onConnect);
    connect(socket, &QTcpSocket::disconnected, this, &Client::onConnect);
    connect(socket, &QTcpSocket::readyRead, this, &Client::onReadyRead);
    qDebug() << ip + " : " + QString::number(port);
    socket->connectToHost(ip, port);
}
void Client::onConnect() { connected_to_server = true; emit connectedToServer();}
void Client::onDisconnect() { connected_to_server = false; emit disconnectedToServer();}

void Client::serverStatus(){
    if (connected_to_server){ emit connectedToServer(); }
    else { emit disconnectedToServer(); }
}

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
    else if (parts[0] == "JOIN_GAME_S" && parts.size() >= 5) {
        emit joinGameSession_S(parts[1] == "1", parts[2] == "1", parts[3].toInt(), parts[4]);
    }
    else if (parts[0] == "SEND_PLAYER_MOVE_S" && parts.size() >= 2) {
        emit sendMove_S(parts[1]);
    }
    else if (parts[0] == "SEND_CHECKMATED_S" && parts.size() >= 2) {
        emit sendCheckmated_S(parts[1].toInt());
    }
    else if (parts[0] == "SEND_PLAYER_MESSAGE_S" && parts.size() >= 3) {
        emit sendPlayerMessage_S(parts[1], parts[2]);
    }
    else if (parts[0] == "SEND_ERROR_MESSAGE_S" && parts.size() >= 2) {
        emit sendErrorMessage_S(parts[1]);
    }
    else if (parts[0] == "REGISTER_USER_S" && parts.size() >= 2) {
        emit registerUser_S(parts[1]);
    }
    else if (parts[0] == "LOGIN_USER_S" && parts.size() >= 2) {
        emit loginUser_S(parts[1]);
    }
    else{
        emit clientMessage("Received Error Message from Server: " + msg);
    }
}
// ---------------------- Senders to ClientWrap + Server ---------------------- //
bool Client::sendMessage(const QString &message) {
    // send to server clientwrap through socket //
    if (socket && socket->state() == QAbstractSocket::ConnectedState) {
        QByteArray data = message.toUtf8();
        socket->write(data);
        socket->flush();
        return true;
    }
    return false;
}

bool Client::registerUser(QString user, QString pass){
    return (sendMessage("REGISTER_USER|" + user + "|" + pass + "\n"));
}
bool Client::loginUser(QString user, QString pass){
    return sendMessage("LOGIN_USER|" + user + "|" + pass + "\n");
}

bool Client::createGameSession(bool isWhite){
    return sendMessage("CREATE_GAME|" + QString(isWhite ? "1" : "0") + "\n");
}
bool Client::joinGameSession(const QString gameID){
    return sendMessage("JOIN_GAME|" + gameID + "\n");
}
bool Client::sendMove(const QString gameID, const QString move, const bool isWhite){
    // needs a space before | because it breaks without it //
    return sendMessage("SEND_MOVE|" + gameID + "|" + QString::number(isWhite) + " |" + move + "\n");
}

bool Client::sendPlayerMessage(QString gameID, const QString playerName, const QString msg){
    return sendMessage("SEND_PLAYER_MESSAGE|" + gameID + "|" + playerName + "|" + msg + "\n");
}


