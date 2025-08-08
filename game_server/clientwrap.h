#ifndef CLIENTWRAP_H
#define CLIENTWRAP_H

#pragma once

#include <QObject>
#include <QTcpSocket>

class ClientWrap : public QObject {
    Q_OBJECT

public:
    explicit ClientWrap(QTcpSocket* socket, QObject* parent = nullptr);

    QString getID() const;
    QTcpSocket* getSocket() const;

    void sendMessage(const QString& message);

signals:
    void serverMessage(QString msg);
    void clientDisconnect(ClientWrap* client);

    void requestGameCreation(ClientWrap* client, bool isWhite); // asks server to make game session //
    void moveReceived(QString gameID, QString move, bool isWhite); // sends received move to server //

private slots:
    void onReadyRead();
    void onDisconnect();

private:
    QTcpSocket* socket;
    QString clientId;

    void receiveMessage(const QString& message);
};
#endif // CLIENTWRAP_H
