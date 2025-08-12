#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>

class Client : public QObject {
    Q_OBJECT

public:
    explicit Client(QObject *parent = nullptr);

    // Send Through Socket //
    void createGameSession(bool isWhite);
    void joinGameSession(const QString gameIDe);
    void sendMove(const QString gameID, const QString move, const bool isWhite);
    void sendPlayerMessage(const QString gameID, const QString playerName, const QString msg);

    // Receive Through Socket //
    void receiveMessage(const QString& msg);

private slots:
    void onReadyRead();

    void onConnect();
    void onDisconnect();

private:
    void sendMessage(const QString &message);
    QTcpSocket *socket;

signals:
    // Received from Server: Client to MainWindow Signals//
    void clientMessage(QString msg);
    void connectedToServer();
    void disconnectedToServer();

    // _S means feedback from server //
    void createGameSession_S(QString gameID);
    void joinGameSession_S(bool gameJoined, bool isWhite);
    void sendMove_S(QString move);
    void sendPlayerMessage_S(QString playerName, QString msg);
    void sendErrorMessage_S(QString msg);
};

#endif // CLIENT_H
