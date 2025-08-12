#ifndef CHESS_PIECES_H
#define CHESS_PIECES_H

#include <QApplication>
#include <utility>
#include <vector>
#include <QPixmap>
#include <QMap>

class Piece {
public:
    Piece(QString pieceCode, int row, int col, bool playerIsWhite);
    virtual ~Piece();

    // Public Functions //
    int get_row(){ return row; }
    int get_col(){ return col; }
    void set_rowcol(int r, int c){ row = r; col = c; }

    void has_moved(){ hasMoved = true; }
    bool get_moved(){ return hasMoved; }
    bool get_color(){ return isWhite; }
    QString get_piece_type() const{ return type; }

    void set_move_twice(){ move_twice = true; }
    bool get_move_twice(){ return move_twice;} // pawns only //

    virtual std::vector<std::pair<int,int>>get_moveset(const QString board[8][8], Piece* p_board[8][8], Piece* last_moved);
    std::vector<std::pair<int,int>> check_if_pinned(const QString board[8][8]);

protected:
     // Constants //
    const int direction[8][2] = {{1,0},{0, 1},{-1,0},{0, -1},{1,1},{-1, 1},{1,-1},{-1, -1}};
    const int knight_direction[8][2] = {{2, 1},{2, -1},{-2, 1},{-2, -1},{1, 2},{-1, 2},{1, -2},{-1, -2}};

    // Variables //
    bool isWhite;
    bool playerIsWhite;
    QChar same;
    QChar opposite;
    QString type;

    int row;
    int col;
    std::vector<std::pair<int,int>> moveset;
    bool hasMoved = false;
    bool move_twice = false; // pawns only //

    // Functions //
    void moves_line(int p_row, int p_col, int r_mult, int c_mult, const QString board[8][8], std::vector<std::pair<int,int>> &m);
    void knight_move(int row, int col, int vertical, int horizontal, const QString board[8][8], std::vector<std::pair<int,int>> &m);
};

class King : public Piece {
public:
    King(QString pieceCode, int row, int col, bool playerIsWhite);
    ~King();
    std::vector<std::pair<int,int>> get_moveset(const QString board[8][8], Piece* p_board[8][8], Piece* last_moved);
            // King Helper Fuctions //
    std::vector<std::pair<int,int>> is_in_check(const QString board[8][8]);
private:
    // If in check, returns vector of tiles that its being checked from - block_move_list algorithm //
    std::vector<std::pair<int,int>> check_if_valid(int row, int col, const QString board[8][8]);
    void add_valid_move(int row, int col, const QString board[8][8], std::vector<std::pair<int,int>> &m);
    void add_castle_move(int row, int col, const QString board[8][8], Piece* p_board[8][8]);
};

class Queen : public Piece {
public:
    Queen(QString pieceCode, int row, int col, bool playerIsWhite);
    ~Queen();
    std::vector<std::pair<int,int>>get_moveset(const QString board[8][8], Piece* p_board[8][8], Piece* last_moved);
};

class Rook : public Piece {
public:
    Rook(QString pieceCode, int row, int col, bool playerIsWhite);
    ~Rook();
    std::vector<std::pair<int,int>>get_moveset(const QString board[8][8], Piece* p_board[8][8], Piece* last_moved);
};

class Bishop : public Piece {
public:
    Bishop(QString pieceCode, int row, int col, bool playerIsWhite);
    ~Bishop();
    std::vector<std::pair<int,int>>get_moveset(const QString board[8][8], Piece* p_board[8][8], Piece* last_moved);
};

class Knight : public Piece {
public:
    Knight(QString pieceCode, int row, int col, bool playerIsWhite);
    ~Knight();
    std::vector<std::pair<int,int>>get_moveset(const QString board[8][8], Piece* p_board[8][8], Piece* last_moved);
};

class Pawn : public Piece {
public:
    Pawn(QString pieceCode, int row, int col, bool playerIsWhite);
    ~Pawn();
    std::vector<std::pair<int,int>>get_moveset(const QString board[8][8], Piece* p_board[8][8], Piece* last_moved);
};

#endif // CHESS_PIECES_H
