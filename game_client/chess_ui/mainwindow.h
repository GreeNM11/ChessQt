#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QTimer>
#include "game_server/server.h"

#include "game_client/client.h"
#include "game_client/chess_logic/chess_game.h"
#include "NeroMate---Chessbot/simple_ai_bot.h"

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
    QLabel* labelBoard;
    QString gameID;
    bool isWhite = true;

    QString playerName;
    bool isOnline = false;
    bool canChat = true;
    
    // AI Bot Integration
    std::unique_ptr<SimpleAIBot> aiBot;
    bool isAIGame = false;
    int aiDifficulty = 3;
    int aiThinkingTime = 2000;

    void createGamePage(bool isWhite, bool isOnline, bool isAI = false);

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
    void onJoinGameSession_C(bool joined, bool isWhite, int code, QString moveList);

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
    void playVsAIClicked();
    void backMenuClicked();
    
    // AI Bot Slots
    void onAIMove(QString move);
    void onAIThinking(bool thinking);
    void onAIMessage(QString message);
    void onAIDifficultyChanged(int value);
    void onAIThinkingTimeChanged(int value);

};

#endif

