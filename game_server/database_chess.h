#ifndef DATABASE_CHESS_H
#define DATABASE_CHESS_H

#include <QObject>

#pragma once
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QString>
#include <QDebug>

class database_chess : public QObject {
    Q_OBJECT
public:
    database_chess();
    ~database_chess();

    // Connect to PostgreSQL database
    bool connect();

    // User operations
    bool registerUser(const QString &username, const QString &password);
    bool loginUser(const QString &username, const QString &password);

private:
    QSqlDatabase db;

    // Hash password for secure storage
    QString hashPassword(const QString &password);
    // Verify password against stored hash
    bool verifyPassword(const QString &password, const QString &storedHash);

signals:
    void ServerMessage(QString msg);
};

#endif // DATABASE_CHESS_H
