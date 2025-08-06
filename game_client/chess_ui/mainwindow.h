#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "game_server/server.h"


#include "game_client/chess_logic/chess_game.h"

class MainWindow : public QMainWindow{
    Q_OBJECT
public:
    MainWindow(bool isServer, QWidget *parent = nullptr);
    ~MainWindow();
private:
    Ui::ChessQt* ui;
    std::unique_ptr<Client> client;
    std::unique_ptr<Server> server;
    std::unique_ptr<chess_game> game;

    QString gameID;
    Client* opponent;

    void setOpponent(Client* opponent);

private slots:
    // Client Function Slots //
    void onClientConnected();
    void onClientDisconnected();
    void receiveOpponent(Client* opponent);
    void onInvalidJoinCode();
    void onPlayerMove(const QString &move, const bool isWhite);

    // Server Function Slots //
    void ServerMessage(const QString &msg);

    // Buttons //
    void SingleplayerClicked();
    void MultiplayerClicked();

};

#endif

