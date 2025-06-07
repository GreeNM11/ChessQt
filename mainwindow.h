#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ui_mainwindow.h"
#include "piece_label.h"
#include "clickable_tile_label.h"

QT_BEGIN_NAMESPACE
namespace Ui {

class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    const int pieceWidth = 80;
    const int pieceHeight = 80;
    const int tileSize = 80;

    Ui::MainWindow* ui;

    ClickableTileLabel* Tiles[8][8];
    QString board[8][8]; // chess board with only string names
    PieceLabel* piece_label_board[8][8]; // chess board but for piece objects
    PieceLabel* s_label = nullptr; // keeps track of which piece is selected
    std::vector<std::pair<int,int>> s_move_list = {};
};
#endif // MAINWINDOW_H
