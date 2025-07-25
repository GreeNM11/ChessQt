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
    void has_moved();
    bool get_moved();
    bool get_color() const;
    QString get_piece_type() const;
    bool get_move_twice(); // pawns only //

    virtual std::vector<std::pair<int,int>>get_moveset(int row, int col, const QString board[8][8], Piece* p_board[8][8], Piece* last_moved);
    std::vector<std::pair<int,int>> check_if_pinned(int row, int col, const QString board[8][8]);

protected:
     // Constants //
    const int direction[8][2] = {{1,0},{0, 1},{-1,0},{0, -1},{1,1},{-1, 1},{1,-1},{-1, -1}};
    const int knight_direction[8][2] = {{2, 1},{2, -1},{-2, 1},{-2, -1},{1, 2},{-1, 2},{1, -2},{-1, -2}};

    // Variables //
    bool isWhite;
    QChar same;
    QChar opposite;
    QString type;
    std::vector<std::pair<int,int>> moveset;
    bool hasMoved = false;
    bool move_twice = false; // pawns only //

    // Functions //
    void moves_line(int p_row, int p_col, int r_mult, int c_mult, const QString board[8][8], std::vector<std::pair<int,int>> &m);
    void knight_move(int row, int col, int vertical, int horizontal, const QString board[8][8], std::vector<std::pair<int,int>> &m);
};

class King : public Piece {
public:
    King(bool w);
    ~King();
    std::vector<std::pair<int,int>> get_moveset(int row, int col, const QString board[8][8], Piece* p_board[8][8], Piece* last_moved);

    std::vector<std::pair<int,int>> is_in_check(int row, int col, const QString board[8][8]);
private:
    std::vector<std::pair<int,int>> check_if_valid(int row, int col, const QString board[8][8]);
    void add_valid_move(int row, int col, const QString board[8][8], std::vector<std::pair<int,int>> &m);
};

class Queen : public Piece {
public:
    Queen(bool w);
    ~Queen();
    std::vector<std::pair<int,int>>get_moveset(int row, int col, const QString board[8][8], Piece* p_board[8][8], Piece* last_moved);
};

class Rook : public Piece {
public:
    Rook(bool w);
    ~Rook();
    std::vector<std::pair<int,int>>get_moveset(int row, int col, const QString board[8][8], Piece* p_board[8][8], Piece* last_moved);
};

class Bishop : public Piece {
public:
    Bishop(bool w);
    ~Bishop();
    std::vector<std::pair<int,int>>get_moveset(int row, int col, const QString board[8][8], Piece* p_board[8][8], Piece* last_moved);
};

class Knight : public Piece {
public:
    Knight(bool w);
    ~Knight();
    std::vector<std::pair<int,int>>get_moveset(int row, int col, const QString board[8][8], Piece* p_board[8][8], Piece* last_moved);
};

class Pawn : public Piece {
public:
    Pawn(bool w);
    ~Pawn();
    std::vector<std::pair<int,int>>get_moveset(int row, int col, const QString board[8][8], Piece* p_board[8][8], Piece* last_moved);
};

#endif // CHESS_PIECES_H
