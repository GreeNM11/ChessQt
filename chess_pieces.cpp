
#include <QApplication>
#include <utility>
#include <vector>
#include <QPixmap>
#include <QLabel>
#include "ui_mainwindow.h"
#include <QMap>

class Piece {
    public:
        enum Color {white, black};
        enum piece_type {wK, wQ, wB, wN, wR, wP, bK, bQ, bB, bN, bR, bP};

        Piece(bool w, int col, int row, QLabel* pieceLabel) : col(col), row(row), pieceLabel(pieceLabel){
            color = w ? white : black;
        }
        virtual ~Piece() {}

        void move_piece(int c, int r, int tileSize){
            col = c;
            row = r;
            pieceLabel->move(col * tileSize, row * tileSize);

        }

        virtual void calculate_moveset(){}

        Color getColor() const { return color; }

    protected:
        Color color;
        piece_type type;
        int col;
        int row;
        QLabel* pieceLabel;
        std::vector<std::pair<int,int>> move_list;
};

class King : public Piece {
    public:
        King(bool w, int col, int row, QLabel *pieceLabel) : Piece(color, col, row, pieceLabel) {
            color = w ? white : black;
            type = (color == white) ? piece_type(wK) : piece_type(bK);
        }

        void calculate_moveset(){
            move_list.clear();
        }
};

class Queen : public Piece {
    public:
        Queen(bool w, int col, int row, QLabel *pieceLabel) : Piece(color, col, row, pieceLabel) {
            color = w ? white : black;
            type = (color == white) ? piece_type(wQ) : piece_type(bQ);
        }

        void calculate_moveset(){
            move_list.clear();
        }
};

class Rook : public Piece {
    public:
        Rook(bool w, int col, int row, QLabel *pieceLabel) : Piece(color, col, row, pieceLabel) {
            color = w ? white : black;
            type = (color == white) ? piece_type(wR) : piece_type(bR);
        }

        void calculate_moveset(){
            move_list.clear();
        }
};

class Bishop : public Piece {
    public:
        Bishop(bool w, int col, int row, QLabel *pieceLabel) : Piece(color, col, row, pieceLabel) {
            color = w ? white : black;
            type = (color == white) ? piece_type(wB) : piece_type(bB);
        }

        void calculate_moveset(){
            move_list.clear();
        }
};

class Knight : public Piece {
    public:
        Knight(bool w, int col, int row, QLabel *pieceLabel) : Piece(color, col, row, pieceLabel) {
            color = w ? white : black;
            type = (color == white) ? piece_type(wN) : piece_type(bN);
        }

        void calculate_moveset(){
            move_list.clear();
        }
};

class Pawn : public Piece {
    public:
        Pawn(bool w, int col, int row, QLabel *pieceLabel) : Piece(color, col, row, pieceLabel) {
            color = w ? white : black;
            type = (color == white) ? piece_type(wP) : piece_type(bP);
        }

        void calculate_moveset(){
            move_list.clear();
        }
};

