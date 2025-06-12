#include <QApplication>
#include <utility>
#include <vector>
#include <QPixmap>
#include <QMap>

class Piece {
    public:

        enum piece_type {wK, wQ, wB, wN, wR, wP, bK, bQ, bB, bN, bR, bP};

        Piece(bool w){
            isWhite = w;
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

        bool getColor() const { return isWhite; }

    protected:
        bool isWhite;
        QChar opposite;
        piece_type type;
        std::vector<std::pair<int,int>>moveset;
        bool hasMoved = false;
};

class King : public Piece {
    public:
        King(bool w) : Piece(w) {
            type = isWhite ? piece_type(wK) : piece_type(bK);
        }

        std::vector<std::pair<int,int>>get_moveset(int row, int col, QString board[8][8]){

            return moveset;
        }
};

class Queen : public Piece {
    public:
        Queen(bool w) : Piece(w) {
            type = isWhite ? piece_type(wQ) : piece_type(bQ);
        }

        std::vector<std::pair<int,int>>get_moveset(int row, int col, QString board[8][8]){
            moveset.clear();
            // combines both bishop and rook moves //
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
            type = isWhite ? piece_type(wR) : piece_type(bR);
        }

        std::vector<std::pair<int,int>>get_moveset(int row, int col, QString board[8][8]){
            moveset.clear();
            // 2 horizontal and 2 vertical lines //
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
            type = isWhite ? piece_type(wB) : piece_type(bB);
        }

        std::vector<std::pair<int,int>>get_moveset(int row, int col, QString board[8][8]){
            moveset.clear();
            // 4 diagonals for bishop //
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
            type = isWhite ? piece_type(wN) : piece_type(bN);
        }

        std::vector<std::pair<int,int>>get_moveset(int row, int col, QString board[8][8]){
            moveset.clear();
            // all 8 possible moves for knights //
            knight_move(row, col, 2, 1, board);
            knight_move(row, col, 2, -1, board);
            knight_move(row, col, 1, 2, board);
            knight_move(row, col, 1, -2, board);
            knight_move(row, col, -1, 2, board);
            knight_move(row, col, -1, -2, board);
            knight_move(row, col, -2, 1, board);
            knight_move(row, col, -2, -1, board);
            return moveset;
        }

    private:
        void knight_move(int row, int col, int vertical, int horizontal, QString board[8][8]){
            int move_row = row - vertical;
            int move_col = col - horizontal;
            if (move_row < 8 && move_row >= 0 && move_col < 8 && move_col >= 0){
                if (board[move_row][move_col] == ""){
                    moveset.push_back(std::make_pair(move_row, move_col));
                }
                else if (board[move_row][move_col].at(0) == opposite){
                    moveset.push_back(std::make_pair(move_row, move_col));
                }
            }
        }

};

class Pawn : public Piece {
    public:
        Pawn(bool w) : Piece(w) {
            type = isWhite ? piece_type(wP) : piece_type(bP);
        }
        std::vector<std::pair<int,int>>get_moveset(int row, int col, QString board[8][8]){
            moveset.clear();
            if (isWhite){
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

