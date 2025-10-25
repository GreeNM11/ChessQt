#ifndef CLIENTWRAP_H
#define CLIENTWRAP_H

#pragma once

#include <QObject>
#include <QTcpSocket>

class ClientWrap : public QObject {
    Q_OBJECT

public:
    explicit ClientWrap(QTcpSocket* socket, QObject* parent = nullptr);

    QString getID() { return clientId; }
    QTcpSocket* getSocket() { return socket; }
    QString getCurrentGameSession() { return currentGameSession; }
    void setCurrentGameSession(QString gameID) { currentGameSession = gameID; }

    void sendMessage(const QString& message); // sends message to server ui //

    // _S Sends information back through socket to local //
    void registerUser_S(const QString &code, const QString &user);
    void loginUser_S(const QString &code, const QString &user);

    void createGameSession_S(const QString &gameID);
    void joinGameSession_S(bool gameFound, bool isWhite, int code, const QString &moveList);

    void sendMove_S(const QString &move);
    void sendCheckmated_S(const QString &isWhite);

    void sendPlayerMessage_S(const QString &playerName, const QString &msg);
    void sendErrorMessage_S(const QString &msg);

    QString getUser(){ return clientUser; }

private:
    QTcpSocket* socket;
    QString clientId;
    QString clientUser;
    QString currentGameSession;

    void receiveMessage(const QString &message);
private slots:
void onReadyRead();
void onDisconnect();

signals:
void serverMessage(QString msg);
void clientDisconnect(ClientWrap* client);

void registerUser(ClientWrap* self, QString user, QString pass);
void loginUser(ClientWrap* self, QString user, QString pass);

void createGameSession(ClientWrap* client, bool isWhite); // asks server to make game session //
void joinGameSession(ClientWrap* client, QString joinCode); // asks server for client to join existing game //

void moveReceived(QString gameID,  bool isWhite, QString move); // sends received move to server //
void checkmatedReceived(QString gameID, QString isWhite);

void playerMessageReceived(QString gameID, QString playerName, QString msg);

};
#endif // CLIENTWRAP_H
