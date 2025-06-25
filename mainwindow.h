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

                // Board Variables //
    Ui::MainWindow* ui;
    QMap<QString, QPixmap> pieceImages; // holds sprites

    ClickableTileLabel* Tiles[8][8]; // holds tile labels of board, Tile[row][col] //
    QString board[8][8]; // chess board with only string names, board[row][col] //
    PieceLabel* piece_label_board[8][8]; // chess board but for piece objects, piece_label_board[row][col] //

    PieceLabel* s_label = nullptr; // keeps track of selected piece
    std::vector<std::pair<int,int>> s_move_list = {}; // vector of selected piece's available moves

                // Board Functions //
    Piece* make_piece(QString pieceCode);
    PieceLabel* make_piece_label(int row, int col, QString pieceCode, int tileSize, QMap<QString, QPixmap> pieceImages);

    void deselect_all();
    void select_piece(PieceLabel* clicked_label);

    void move_piece(PieceLabel* p, int new_row, int new_col);
    void click_piece_action(PieceLabel* clicked_label);
    void click_tile_action(ClickableTileLabel* tile);

    void setup_board();
};
#endif // MAINWINDOW_H
