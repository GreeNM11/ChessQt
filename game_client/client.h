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

public slots:
    void sendMove(const QString &move, const bool isWhite);

private slots:
    void onConnect();
    void onDisconnect();
    void onReadyRead();

private:
    QTcpSocket *socket;
    bool isWhite;

signals:
    void connectedToServer();
    void disconnectedToServer();
};

#endif // CLIENT_H
