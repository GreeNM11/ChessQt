#ifndef BOARD_STATE_H
#define BOARD_STATE_H

#include <QObject>

#include "game_client/chess_logic/chess_pieces.h"

class board_state : public QObject
{
    Q_OBJECT
public:
    board_state(bool isWhite, bool isOnline);
    ~board_state();

    void setup_board();

    void click_piece(int row, int col);
    void click_tile(int row, int col);

    // Server Received //
    void receive_move(QString moveCode);

    // Server Validation //
    int validate_move(QString move); // returns an error number //
    int validate_checkmated();
    void server_move(QString move){ receive_move(move); }

private:
    // Variables //
    bool isWhite;
    bool white_turn;
    bool isOnline;
    bool isServer;

    QString board[8][8] = { // String Starting Chess Board //
        { "bR", "bN", "bB", "bQ", "bK", "bB", "bN", "bR" },
        { "bP", "bP", "bP", "bP", "bP", "bP", "bP", "bP" },
        { "",   "",   "",   "",   "",   "",   "",   "" },
        { "",   "",   "",   "",   "",   "",   "",   "" },
        { "",   "",   "",   "",   "",   "",   "",   "" },
        { "",   "",   "",   "",   "",   "",   "",   "" },
        { "wP", "wP", "wP", "wP", "wP", "wP", "wP", "wP" },
        { "wR", "wN", "wB", "wQ", "wK", "wB", "wN", "wR" }};
    Piece* piece_board[8][8]; // object_board[row][col] //

    Piece* selected_piece = nullptr;
    Piece* last_moved = nullptr; // keeps track of last moved piece for en peassant //
    std::pair<int,int> last_tile = std::make_pair(-1, -1);
    std::vector<std::pair<int,int>> s_move_list = {}; // vector selected piece's available moves //
    std::vector<std::pair<int,int>> block_move_list = {}; // vector of moves that get player out of check //

    std::vector<Piece*>white_pieces;
    std::vector<Piece*>black_pieces;

    King* white_king;
    King* black_king;
    int in_check = false; // 0 is no, 1 is check, 2 is double check //
    int checkmateCode = 0; // 0 (none), 1 (white checkmated), 2 (black checkmated), 3 (white stale), 4 (black stale) //

    // Handles Sending & Request from Server //
    void send_move_request(int p_row, int p_col, int new_row, int new_col);
    void send_checkmate_request(int checkmateCode);

    // Object Functions //
    void select_piece(int row, int col);
    void deselect_piece();

    void make_piece(QString pieceCode, int row, int col);
    void capture_piece(int row, int col);

    void move_piece(int p_row, int p_col, int new_row, int new_col);
    void pawn_mechanics(int old_row, int old_col, int row, int col, bool capture);

    // Game State Functions //
    void switch_turn();
    bool check_if_in_check();
    bool check_if_checkmated();

signals:
    // Signals to chess_game //
    void make_piece_label(QString pieceCode, int row, int col);
    void move_piece_label(int p_row, int p_col, int new_row, int new_col);
    void capture_piece_label(int row, int col);

    void highlight_tiles(std::vector<std::pair<int,int>> move_list, bool turn); // turn is on/off //
    void highlight_piece(int row, int col, bool turn);

    void check_king_labels(bool white_turn, bool in_check);
    void checkmated(int checkmateCode);

    void send_player_move(QString move, bool isWhite);
    void clientMessage(QString msg);

};

#endif // BOARD_STATE_H
