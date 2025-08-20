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
    explicit Server(QObject *parent = nullptr);

    void emitServerStatus();

private slots:
    void onNewConnection();
    void clientDisconnect(ClientWrap* client);

    void newGameSession(ClientWrap* client, bool isWhite);
    void joinGameSession(ClientWrap* client, QString gameID);

    void moveReceived(QString gameID,  bool isWhite, QString move);
    void playerMessageReceived(QString gameID, QString playerName, QString msg);

private:
    QTcpServer *server;
    std::unique_ptr<database_chess> database;

    void serverMessage(QString msg);

    QHash<QString, GameSession*> activeSessions;
    QHash<QString, ClientWrap*> clientConnected;

signals:
    void newMessage(const QString &msg);
};

#endif // SERVER_H
