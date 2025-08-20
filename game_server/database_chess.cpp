#include "database_chess.h"
#include <QCryptographicHash> // simple hashing for now, replace with bcrypt for production

database_chess::database_chess() {

}

database_chess::~database_chess() {
    if (db.isOpen()) db.close();
}

bool database_chess::connect() {
    db = QSqlDatabase::addDatabase("QPSQL");
    db.setHostName("localhost");
    db.setPort(7598);
    db.setDatabaseName("chess_db"); // your DB name
    db.setUserName("postgres");
    db.setPassword("lindanny75sql");

    if (!db.open()) {
        emit ServerMessage("❌DB Connection failed:" + db.lastError().text());
        return false;
    }
    emit ServerMessage("✅Connected to chess_db!");
    return true;
}

// Simple SHA-256 hashing (for demonstration, replace with bcrypt in production)
QString database_chess::hashPassword(const QString &password) {
    QByteArray hash = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256);
    return QString(hash.toHex());
}

bool database_chess::verifyPassword(const QString &password, const QString &storedHash) {
    return hashPassword(password) == storedHash;
}

bool database_chess::registerUser(const QString &username, const QString &password) {
    QSqlQuery query;
    query.prepare("INSERT INTO users (username, password_hash) VALUES (:username, :password)");
    query.bindValue(":username", username);
    query.bindValue(":password", hashPassword(password));

    if (!query.exec()) {
        emit ServerMessage("❌ Register failed:" + query.lastError().text());
        return false;
    }
    emit ServerMessage("✅ User registered:" + username);
    return true;
}

bool database_chess::loginUser(const QString &username, const QString &password) {
    QSqlQuery query;
    query.prepare("SELECT password_hash FROM users WHERE username = :username");
    query.bindValue(":username", username);

    if (!query.exec() || !query.next()) {
        emit ServerMessage("❌ Login failed: user not found");
        return false;
    }

    QString storedHash = query.value(0).toString();
    if (verifyPassword(password, storedHash)) {
        emit ServerMessage("✅ Login successful:" + username);
        return true;
    } else {
        emit ServerMessage("❌ Login failed: wrong password");
        return false;
    }
}


