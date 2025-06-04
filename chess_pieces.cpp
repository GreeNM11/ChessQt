#include <QApplication>
#include <utility>
#include <vector>
#include <QPixmap>
#include <QMap>

class Piece {
    public:
        enum Color {white, black};
        enum piece_type {wK, wQ, wB, wN, wR, wP, bK, bQ, bB, bN, bR, bP};

        Piece(bool w){
            color = w ? white : black;
        }
        virtual ~Piece() {}
        virtual std::vector<std::pair<int,int>>get_moveset(int row, int col){return moveset;}
        void has_moved(){ hasMoved = true; }

        Color getColor() const { return color; }

    protected:
        Color color;
        piece_type type;
        std::vector<std::pair<int,int>>moveset;
        bool hasMoved = false;
};

class King : public Piece {
    public:
        King(bool w) : Piece(w) {
            type = (color == white) ? piece_type(wK) : piece_type(bK);
        }

        std::vector<std::pair<int,int>>get_moveset(int row, int col){

            return moveset;
        }
};

class Queen : public Piece {
    public:
        Queen(bool w) : Piece(w) {
            type = (color == white) ? piece_type(wQ) : piece_type(bQ);
        }

        std::vector<std::pair<int,int>>get_moveset(int row, int col){

            return moveset;
        }
};

class Rook : public Piece {
    public:
        Rook(bool w) : Piece(w) {
            type = (color == white) ? piece_type(wR) : piece_type(bR);
        }

        std::vector<std::pair<int,int>>get_movesetet(){

            return moveset;
        }
};

class Bishop : public Piece {
    public:
        Bishop(bool w) : Piece(w) {
            type = (color == white) ? piece_type(wB) : piece_type(bB);
        }

        std::vector<std::pair<int,int>>get_moveset(int row, int col){


            return moveset;
        }
};

class Knight : public Piece {
    public:
        Knight(bool w) : Piece(w) {
            type = (color == white) ? piece_type(wN) : piece_type(bN);
        }

        std::vector<std::pair<int,int>>get_moveset(int row, int col){


            return moveset;
        }
};

class Pawn : public Piece {
    public:
        Pawn(bool w) : Piece(w) {
            type = (color == white) ? piece_type(wP) : piece_type(bP);
        }

        std::vector<std::pair<int,int>>get_moveset(int row, int col){
            moveset.clear();
            if (color == white){
                moveset.push_back(std::make_pair(row - 1, col));
                if (!hasMoved){
                    moveset.push_back(std::make_pair(row - 2, col));
                }
            }
            else{
                moveset.push_back(std::make_pair(row + 1, col));
                if (!hasMoved){
                    moveset.push_back(std::make_pair(row + 2, col));
                }
            }

            return moveset;
        }
};

