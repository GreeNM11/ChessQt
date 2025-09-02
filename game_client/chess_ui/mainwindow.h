#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "game_server/server.h"

#include "game_client/client.h"
#include "game_client/chess_logic/chess_game.h"

class MainWindow : public QMainWindow{
    Q_OBJECT
public:
    MainWindow(bool isServer, bool isRemote, QWidget *parent = nullptr);
    ~MainWindow();
private:
    const QString localIP = "127.0.0.1";
    const QString remoteIP = "35.224.252.245"; // external IP - from chessqt-server GCP //
    const int port = 7575;

    Ui::ChessQt* ui;
    std::unique_ptr<Client> client;
    std::unique_ptr<Server> server;

    std::unique_ptr<chess_game> game;
    QString gameID;
    QString* opponentID;
    bool isWhite = false;

    QString playerName;
    bool isOnline = false;

    void createGamePage(bool isWhite, bool isOnline);

private slots:
    void ClientMessage(const QString msg);

    // Received by Game //
    void onPlayerMove_G(const QString move, const bool isWhite);
    void onCheckmated_G(int code);

    // Recieved by Client //
    void onClientConnected_C();
    void onClientDisconnected_C();

    void onRegisterUser_C(QString code);
    void onLoginUser_C(QString code);

    void onCreateGameSession_C(QString gameID);
    void onJoinGameSession_C(bool joined, bool isWhite);

    void onReceiveMove_C(QString move);
    void onReceiveCheckmated_C(int code);

    void onPlayerMessage_C(QString playerName, QString msg);
    void onErrorMessage_C(QString msg);

    // Server Function Slots //
    void ServerMessage(const QString msg);

    // Buttons //
    void registerButtonClicked();
    void loginButtonClicked();

    void singleplayerClicked();
    void hostGameClicked();
    void joinGameClicked();
    void backMenuClicked();

};

#endif

