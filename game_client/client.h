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
    void sendMove(const QString &from, const QString &to);

private slots:
    void onConnected();
    void onReadyRead();

private:
    QTcpSocket *socket;
};

#endif // CLIENT_H
