#ifndef CHESS_GAME_H
#define CHESS_GAME_H

#include <QMainWindow>
#include "ui_mainwindow.h"
#include "piece_label.h"
#include "clickable_tile_label.h"

class chess_game : public QObject{

public:
    chess_game(Ui::MainWindow* ui, int time, int increment);
    ~chess_game();

private:
    const int pieceWidth = 80;
    const int pieceHeight = 80;
    const int tileSize = 80;

    int time;
    int increment;

    // Board Variables //
    Ui::MainWindow* ui;
    QMap<QString, QPixmap> pieceImages; // holds sprites

    QString board[8][8] = {  // String Chess Board //
        { "bR", "bN", "bB", "bQ", "bK", "bB", "bN", "bR" },
        { "bP", "bP", "bP", "bP", "bP", "bP", "bP", "bP" },
        { "",   "",   "",   "",   "",   "",   "",   "" },
        { "",   "",   "",   "",   "",   "",   "",   "" },
        { "",   "",   "",   "",   "",   "",   "",   "" },
        { "",   "",   "",   "",   "",   "",   "",   "" },
        { "wP", "wP", "wP", "wP", "wP", "wP", "wP", "wP" },
        { "wR", "wN", "wB", "wQ", "wK", "wB", "wN", "wR" }
    };

    ClickableTileLabel* Tiles[8][8]; // board for Tile[row][col] //
    PieceLabel* piece_label_board[8][8]; // label_board[row][col] //

    PieceLabel* s_label = nullptr; // keeps track of selected piece
    std::vector<std::pair<int,int>> s_move_list = {}; // vector of selected piece's available moves

    bool white_turn = true;
    PieceLabel* last_piece_moved;

    PieceLabel* white_king_label;
    PieceLabel* black_king_label;
    King* white_king;
    King* black_king;
    std::vector<std::pair<int,int>> block_move_list = {}; // movelist that get player out of check //

    int in_check; // 0 is no, 1 is check, 2 is double check //
    bool checkmate;

    // Board Functions //
    Piece* make_piece(QString pieceCode);
    PieceLabel* make_piece_label(int row, int col, QString pieceCode, int tileSize, QMap<QString, QPixmap> pieceImages);

    void deselect_all();
    void select_piece(PieceLabel* clicked_label);
    void switch_turn();

    void move_piece(PieceLabel* p, int new_row, int new_col);
    void click_piece_action(PieceLabel* clicked_label);
    void click_tile_action(ClickableTileLabel* tile);

    bool check_if_in_check();
    bool check_if_checkmate();

    void setup_board();
};

#endif // CHESS_GAME_H
