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
    void createGameSession(bool isWhite);
    void receiveOponent(Client* client);
    void invalidJoinCode();

    // Through socket //
    void requestGameSession(bool isWhite);
    void sendMove(const QString &move, const bool isWhite);

private slots:
    void onConnect();
    void onDisconnect();

private:
    QTcpSocket *socket;

signals:
    // Client to MainWindow //
    void connectedToServer();
    void disconnectedToServer();
    void sendOpponent(Client* opponent);
    void sendInvalidJoinCode();
};

#endif // CLIENT_H
