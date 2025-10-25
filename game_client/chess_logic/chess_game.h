#ifndef CHESS_GAME_H
#define CHESS_GAME_H

#include <QMainWindow>
#include <QMap>
#include <QGridLayout>

#include "game_client/chess_ui/piece_label.h"
#include "game_client/chess_ui/clickable_tile_label.h"
#include "game_client/chess_logic/board_state.h"

namespace Ui {class ChessQt;}

class chess_game : public QObject{
    Q_OBJECT
public:
    chess_game(QLabel* boardLabel, bool isWhite, int time, int increment, bool isOnline, bool isAIGame = false, QObject* parent = nullptr);
    ~chess_game();

    void receive_move(QString move);
    void receive_moveList(QString moveList);
    
    // AI Integration
    board_state* getBoardState() { return game_state.get(); }

private:
    const int pieceWidth = 80;
    const int pieceHeight = 80;
    const int tileSize = 80;

    // Player Variables //
    bool isOnline;
    bool isWhite;
    bool isAIGame;
    int time;
    int increment;

    std::unique_ptr<board_state> game_state; // handles all logic //

    // Board Label Variables //
    QLabel* boardLabel;
    QMap<QString, QPixmap> pieceImages; // holds sprites //

    QGridLayout* tileLayout;
    ClickableTileLabel* Tiles[8][8]; // board for Tile[row][col] //
    PieceLabel* piece_label_board[8][8]; // label_board[row][col] //

    PieceLabel* white_king_label;
    PieceLabel* black_king_label;

    // Board Functions //
    void click_piece_action(PieceLabel* clicked_label);
    void click_tile_action(ClickableTileLabel* tile);

    void setup_board();

private slots:
    // Label Requests //
    void make_piece_label(QString pieceCode, int row, int col);
    void move_piece_label(int p_row, int p_col, int new_row, int new_col);
    void capture_piece_label(int row, int col);

    void highlight_tiles(std::vector<std::pair<int,int>> move_list, bool turn); // turn is on/off //
    void highlight_piece(int row, int col, bool turn);

    void check_king_labels(bool white_turn, bool in_check);

    // Server Information //
    void send_player_move(QString move, bool isWhite);
    void send_checkmated(int code);

signals:
    // MainWindow Requests //
    void clientMessage(const QString msg);

    // Server Requests //
    void player_move(const QString move, const bool isWhite);
    void player_checkmated(int code);
};

#endif // CHESS_GAME_H
