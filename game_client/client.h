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
    //
    void receiveOponent(Client* client);

    // Through socket //
    void createGameSession(bool isWhite);
    void sendMove(const QString gameID, const QString &move, const bool isWhite);

private slots:
    void onConnect();
    void onDisconnect();

private:
    void sendMessage(const QString &message);
    QTcpSocket *socket;

signals:
    // Client to MainWindow //
    void clientMessage(QString msg);
    void connectedToServer();
    void disconnectedToServer();

    void sendInvalidJoinCode();
};

#endif // CLIENT_H
