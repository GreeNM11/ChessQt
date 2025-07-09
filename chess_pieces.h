#ifndef CHESS_PIECES_H
#define CHESS_PIECES_H

#include <QApplication>
#include <utility>
#include <vector>
#include <QPixmap>
#include <QMap>

class Piece {
public:
    Piece(bool w);
    virtual ~Piece();

    bool get_color() const;
    bool get_piece_type() const;
    void has_moved();
    virtual std::vector<std::pair<int,int>>get_moveset(int row, int col, const QString board[8][8]);

protected:
    enum piece_type {wK, wQ, wB, wN, wR, wP, bK, bQ, bB, bN, bR, bP};

    // Variables //
    bool isWhite;
    QChar same;
    QChar opposite;
    piece_type type;
    std::vector<std::pair<int,int>> moveset;
    bool hasMoved = false;

    // Functions //
    void moves_line(int p_row, int p_col, int r_mult, int c_mult, const QString board[8][8], std::vector<std::pair<int,int>> &m);
    void knight_move(int row, int col, int vertical, int horizontal, const QString board[8][8], std::vector<std::pair<int,int>> &m);
};

class King : public Piece {
public:
    King(bool w);
    ~King();
    std::vector<std::pair<int,int>> get_moveset(int row, int col, const QString board[8][8]);

    std::vector<std::pair<int,int>> is_in_check(int row, int col, const QString board[8][8]);
private:
    std::vector<std::pair<int,int>> check_if_valid(int row, int col, const QString board[8][8]);
    void add_valid_move(int row, int col, const QString board[8][8], std::vector<std::pair<int,int>> &m);
};

class Queen : public Piece {
public:
    Queen(bool w);
    ~Queen();
    std::vector<std::pair<int,int>>get_moveset(int row, int col, const QString board[8][8]);
};

class Rook : public Piece {
public:
    Rook(bool w);
    ~Rook();
    std::vector<std::pair<int,int>>get_moveset(int row, int col, const QString board[8][8]);
};

class Bishop : public Piece {
public:
    Bishop(bool w);
    ~Bishop();
    std::vector<std::pair<int,int>>get_moveset(int row, int col, const QString board[8][8]);
};

class Knight : public Piece {
public:
    Knight(bool w);
    ~Knight();
    std::vector<std::pair<int,int>>get_moveset(int row, int col, const QString board[8][8]);
};

class Pawn : public Piece {
public:
    Pawn(bool w);
    ~Pawn();
    std::vector<std::pair<int,int>>get_moveset(int row, int col, const QString board[8][8]);
    bool just_moved(bool stop); // stop is true if en passant isnt available anymore //
private:
    bool pawn_just_moved = false;
};

#endif // CHESS_PIECES_H
