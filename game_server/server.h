#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>

#include "server_security.h"
#include "gamesession.h"
#include "database_chess.h"

class Server : public QObject {
    Q_OBJECT

public:
    explicit Server(QObject *parent = nullptr, int port = 7575);

    void emitServerStatus();
    void createDB();

private slots:
    void onNewConnection();
    void clientDisconnect(ClientWrap* client);

    void onSessionEmpty(const QString &gameID);

    void registerUser(ClientWrap* client, const QString &user, const QString &pass);
    void loginUser(ClientWrap* client, const QString &user, const QString &pass);

    void newGameSession(ClientWrap* client, bool isWhite);
    void joinGameSession(ClientWrap* client, const QString &gameID);

    void moveReceived(const QString &gameID,  bool isWhite, const QString &move);

    void playerMessageReceived(const QString &gameID, const QString &playerName, const QString &msg);

    void serverMessage(QString msg);

private:
    int port;

    QTcpServer *server;
    std::unique_ptr<database_chess> database;

    QHash<QString, GameSession*> activeSessions;
    QHash<QString, ClientWrap*> clientConnected;

signals:
    void newMessage(const QString &msg);
};

#endif // SERVER_H
