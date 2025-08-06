#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>

#include "game_client/client.h"

class Server : public QObject {
    Q_OBJECT

public:
    explicit Server(QObject *parent = nullptr);

    void emitServerStatus();

private slots:
    void onNewConnection();

    void newGameSession(Client* client, bool isWhite);
    void joinGameSession(Client* client, QString gameID);

private:
    QTcpServer *server;

    struct GameSession {
        QString gameId;
        bool p1_isWhite;
        Client* player1 = nullptr;
        Client* player2 = nullptr;
    };

    QHash<QString, GameSession*> activeSessions;
    QHash<QString, QTcpSocket*> clientSockets; // need to make a class //

signals:
    void newMessage(const QString &msg);
};

#endif // SERVER_H
