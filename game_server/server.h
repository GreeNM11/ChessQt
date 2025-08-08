#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>

#include "clientwrap.h"

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

    void moveReceived(QString gameID, QString move, bool isWhite);

private:
    QTcpServer *server;
    void serverMessage(QString msg);

    struct GameSession {
        QString gameID;
        bool isWhite; // if player1 is white //
        ClientWrap* player1 = nullptr;
        ClientWrap* player2 = nullptr;
    };

    QHash<QString, GameSession*> activeSessions;
    QHash<QString, ClientWrap*> clientConnected; // need to make a class //

signals:
    void newMessage(const QString &msg);
};

#endif // SERVER_H
