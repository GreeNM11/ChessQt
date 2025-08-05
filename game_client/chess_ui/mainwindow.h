#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "game_client/chess_logic/chess_game.h"
#include "game_server/server.h"
#include "game_client/client.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(bool isServer, QWidget *parent = nullptr);
    ~MainWindow();
private:
    Ui::ChessQt* ui;
    chess_game* game;

private slots:
    void onStartGameClicked();
};

#endif

