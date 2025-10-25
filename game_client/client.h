#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>

class Client : public QObject {
    Q_OBJECT

public:
    explicit Client(QObject *parent = nullptr, const QString &ip = QString(), int port = 0);

    // Send Through Socket //
    bool registerUser(const QString &user, const QString &pass);
    bool loginUser(const QString &user, const QString &pass);

    bool createGameSession(bool isWhite);
    bool joinGameSession(const QString &gameIDe);
    bool sendMove(const QString &gameID, const QString &move, const bool isWhite);

    bool sendPlayerMessage(const QString &gameID, const QString &playerName, const QString &msg);
    void serverStatus();

    // Receive Through Socket //
    void receiveMessage(const QString &msg);

private slots:
    void onReadyRead();

    void onConnect();
    void onDisconnect();

private:
    bool connected_to_server = false;

    bool sendMessage(const QString &message);
    QTcpSocket *socket;

signals:
    // Received from Server: Client to MainWindow Signals//
    void clientMessage(QString msg);
    void connectedToServer();
    void disconnectedToServer();

    // _S means feedback from server //
    void registerUser_S(QString code);
    void loginUser_S(QString code);

    void createGameSession_S(QString gameID);
    void joinGameSession_S(bool gameJoined, bool isWhite, int code, QString moveList);

    void sendMove_S(QString move);
    void sendCheckmated_S(int code);

    void sendPlayerMessage_S(QString playerName, QString msg);
    void sendErrorMessage_S(QString msg);
};

#endif // CLIENT_H
