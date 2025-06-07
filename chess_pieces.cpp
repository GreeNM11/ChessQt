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
            opposite = w ? 'b' : 'w';
        }
        virtual ~Piece() {}
        virtual std::vector<std::pair<int,int>>get_moveset(int row, int col, QString board[8][8]){return moveset;}
        void has_moved(){ hasMoved = true; }


        void moves_line(int p_row, int p_col, int r_mult, int c_mult, QString board[8][8]){

        // Takes in piece location and a direction, then adds available moves in that line/diaganol to moveset //
        // p_row/col is piece location, mult = -1, 0, 1 are inputs for direction, negative is up and left //

            int r = p_row + r_mult;
            int c = p_col + c_mult;
            bool blocked = false;
            while (r < 8 && c < 8 && r >= 0 && c >= 0 && !blocked){
                if (board[r][c] == ""){
                    moveset.push_back(std::make_pair(r, c));
                }
                else if (board[r][c].at(0) == opposite){
                    moveset.push_back(std::make_pair(r, c));
                    blocked = true;
                }
                else{
                    blocked = true;
                }
                r += r_mult;
                c += c_mult;
            }
        }

        Color getColor() const { return color; }

    protected:
        Color color;
        QChar opposite;
        piece_type type;
        std::vector<std::pair<int,int>>moveset;
        bool hasMoved = false;
};

class King : public Piece {
    public:
        King(bool w) : Piece(w) {
            type = (color == white) ? piece_type(wK) : piece_type(bK);
        }

        std::vector<std::pair<int,int>>get_moveset(int row, int col, QString board[8][8]){

            return moveset;
        }
};

class Queen : public Piece {
    public:
        Queen(bool w) : Piece(w) {
            type = (color == white) ? piece_type(wQ) : piece_type(bQ);
        }

        std::vector<std::pair<int,int>>get_moveset(int row, int col, QString board[8][8]){
            moveset.clear();
            moves_line(row, col,  1, 0, board);
            moves_line(row, col, -1, 0, board);
            moves_line(row, col, 0,  1, board);
            moves_line(row, col, 0, -1, board);

            moves_line(row, col,  1,  1, board);
            moves_line(row, col,  1, -1, board);
            moves_line(row, col, -1,  1, board);
            moves_line(row, col, -1, -1, board);

            return moveset;
        }
};

class Rook : public Piece {
    public:
        Rook(bool w) : Piece(w) {
            type = (color == white) ? piece_type(wR) : piece_type(bR);
        }

        std::vector<std::pair<int,int>>get_moveset(int row, int col, QString board[8][8]){
            moveset.clear();
            moves_line(row, col,  1, 0, board);
            moves_line(row, col, -1, 0, board);
            moves_line(row, col, 0,  1, board);
            moves_line(row, col, 0, -1, board);
            return moveset;
        }
};

class Bishop : public Piece {
    public:
        Bishop(bool w) : Piece(w) {
            type = (color == white) ? piece_type(wB) : piece_type(bB);
        }

        std::vector<std::pair<int,int>>get_moveset(int row, int col, QString board[8][8]){
            moveset.clear();
            moves_line(row, col,  1,  1, board);
            moves_line(row, col,  1, -1, board);
            moves_line(row, col, -1,  1, board);
            moves_line(row, col, -1, -1, board);
            return moveset;
        }
};

class Knight : public Piece {
    public:
        Knight(bool w) : Piece(w) {
            type = (color == white) ? piece_type(wN) : piece_type(bN);
        }

        std::vector<std::pair<int,int>>get_moveset(int row, int col, QString board[8][8]){

            return moveset;
        }
};

class Pawn : public Piece {
    public:
        Pawn(bool w) : Piece(w) {
            type = (color == white) ? piece_type(wP) : piece_type(bP);
        }

        std::vector<std::pair<int,int>>get_moveset(int row, int col, QString board[8][8]){
            moveset.clear();
            if (color == white){
                if (board[row-1][col] == ""){
                    moveset.push_back(std::make_pair(row - 1, col));
                    if (!hasMoved && board[row-2][col] == ""){
                        moveset.push_back(std::make_pair(row - 2, col));
                    }
                }
            }
            else{
                if (board[row+1][col] == ""){
                    moveset.push_back(std::make_pair(row + 1, col));
                    if (!hasMoved && board[row+2][col] == ""){
                        moveset.push_back(std::make_pair(row + 2, col));
                    }
                }
            }

            return moveset;
        }
};

