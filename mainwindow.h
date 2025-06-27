#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "chess_game.h"

class MainWindow;

QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow* ui;
    chess_game* game;
};
#endif // MAINWINDOW_H
