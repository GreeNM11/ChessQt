#include "chess_pieces.h"

// --------- // Base Piece Class // -------------------------------------------------------------------------- //

Piece::Piece(bool w){
    isWhite = w;
    same = !w ? 'b' : 'w';
    opposite = w ? 'b' : 'w';
}
Piece::~Piece(){}
bool Piece::getColor() const { return isWhite; }
void Piece::has_moved(){ hasMoved = true; }
std::vector<std::pair<int,int>>Piece::get_moveset(int row, int col, const QString board[8][8]){return moveset;}

void Piece::moves_line(int p_row, int p_col, int r_mult, int c_mult, const QString board[8][8], std::vector<std::pair<int,int>> &m){

// Takes in piece location and a direction, then adds available moves in that line/diaganol to moveset //
// p_row/col is piece location, mult = -1, 0, 1 are inputs for direction, negative is up and left //

    int r = p_row + r_mult;
    int c = p_col + c_mult;
    bool blocked = false;
    while (r < 8 && c < 8 && r >= 0 && c >= 0 && !blocked){
        if (board[r][c] == ""){
            m.push_back(std::make_pair(r, c));
        }
        else if (board[r][c].at(0) == opposite){
            m.push_back(std::make_pair(r, c));
            blocked = true;
        }
        else if (board[r][c].at(0) != opposite){
            blocked = true;
        }
        r += r_mult;
        c += c_mult;
    }
}

void Piece::knight_move(int row, int col, int vertical, int horizontal, const QString board[8][8], std::vector<std::pair<int,int>> &m){
    int move_row = row - vertical;
    int move_col = col - horizontal;
    if (move_row < 8 && move_row >= 0 && move_col < 8 && move_col >= 0){
        if (board[move_row][move_col] == "" || board[move_row][move_col].at(0) == opposite){
            m.push_back(std::make_pair(move_row, move_col));
        }
    }
}
// --------- // King // -------------------------------------------------------------------------------------- //

King::King(bool w) : Piece(w) {
    type = isWhite ? piece_type(wK) : piece_type(bK);
}
King::~King(){}
std::vector<std::pair<int,int>>King::get_moveset(int row, int col, const QString board[8][8]){
    moveset.clear();
    // All 8 possible king moves //
    add_valid_move(row, col+1, board, moveset);
    add_valid_move(row, col-1, board, moveset);
    add_valid_move(row+1, col, board, moveset);
    add_valid_move(row-1, col, board, moveset);
    add_valid_move(row+1, col+1, board, moveset);
    add_valid_move(row+1, col-1, board, moveset);
    add_valid_move(row-1, col+1, board, moveset);
    add_valid_move(row-1, col-1, board, moveset);
    return moveset;
}

bool King::is_in_check(int row, int col, const QString board[8][8]){
    return check_if_valid(row,col,board);
}
void King::add_valid_move(int row, int col, const QString board[8][8], std::vector<std::pair<int,int>> &m){
    if (check_if_valid(row,col,board)){
        m.push_back(std::make_pair(row, col));
    }
}

bool King::check_if_valid(int row, int col, const QString board[8][8]){
    if (row >= 8 || row < 0 || col >= 8 || col < 0 || board[row][col].at(0) == same){
        return false; // cant move outside board or same color piece
    }
    std::vector<std::pair<int,int>> check_pieces;
    // Checks all tiles where a queen or rook can be checking //
    moves_line(row, col,  1, 0, board, check_pieces);
    moves_line(row, col, -1, 0, board, check_pieces);
    moves_line(row, col, 0,  1, board, check_pieces);
    moves_line(row, col, 0, -1, board, check_pieces);
    for (const auto& move : check_pieces){
        if (board[move.first][move.second] == (opposite + 'Q') || board[move.first][move.second] == (opposite + 'R')){
            return false; // tile being covered by queen or rook
        }
    }
    check_pieces.clear();
    // Checks all tiles where queen or bishop can be checking //
    moves_line(row, col,  1,  1, board, check_pieces);
    moves_line(row, col,  1, -1, board, check_pieces);
    moves_line(row, col, -1,  1, board, check_pieces);
    moves_line(row, col, -1, -1, board, check_pieces);
    for (const auto& move : check_pieces){
        if (board[move.first][move.second] == (opposite + 'Q') || board[move.first][move.second] == (opposite + 'B')){
            return false; // tile being covered by queen or rook
        }
    }
    check_pieces.clear();
    // Checks all tiles where a knight can be checking //
    knight_move(row, col, 2,  1, board, check_pieces);
    knight_move(row, col, 2, -1, board, check_pieces);
    knight_move(row, col, 1,  2, board, check_pieces);
    knight_move(row, col, 1, -2, board, check_pieces);
    knight_move(row, col, -1,  2, board, check_pieces);
    knight_move(row, col, -1, -2, board, check_pieces);
    knight_move(row, col, -2,  1, board, check_pieces);
    knight_move(row, col, -2, -1, board, check_pieces);
    for (const auto& move : check_pieces){
        if (board[move.first][move.second] == (opposite + 'N')){
            return false; // tile being covered by queen or rook
        }
    }
    check_pieces.clear();

    // Checks the 2 pawn moves that can check //

    // Checks if enemy king covers //

    return true;
}

// --------- // Queen // ------------------------------------------------------------------------------------- //

Queen::Queen(bool w) : Piece(w) {
    type = isWhite ? piece_type(wQ) : piece_type(bQ);
}
Queen::~Queen(){}
std::vector<std::pair<int,int>>Queen::get_moveset(int row, int col, const QString board[8][8]){
    moveset.clear();
    // combines both bishop and rook moves //
    moves_line(row, col,  1, 0, board, moveset);
    moves_line(row, col, -1, 0, board, moveset);
    moves_line(row, col, 0,  1, board, moveset);
    moves_line(row, col, 0, -1, board, moveset);

    moves_line(row, col,  1,  1, board, moveset);
    moves_line(row, col,  1, -1, board, moveset);
    moves_line(row, col, -1,  1, board, moveset);
    moves_line(row, col, -1, -1, board, moveset);

    return moveset;
}
// --------- // Rook // -------------------------------------------------------------------------------------- //

Rook::Rook(bool w) : Piece(w) {
    type = isWhite ? piece_type(wR) : piece_type(bR);
}
Rook::~Rook(){}
std::vector<std::pair<int,int>>Rook::get_moveset(int row, int col, const QString board[8][8]){
    moveset.clear();
    // 2 horizontal and 2 vertical lines //
    moves_line(row, col,  1, 0, board, moveset);
    moves_line(row, col, -1, 0, board, moveset);
    moves_line(row, col, 0,  1, board, moveset);
    moves_line(row, col, 0, -1, board, moveset);
    return moveset;
}
// --------- // Bishop // ------------------------------------------------------------------------------------ //

Bishop::Bishop(bool w) : Piece(w) {
    type = isWhite ? piece_type(wB) : piece_type(bB);
}
Bishop::~Bishop(){}
std::vector<std::pair<int,int>>Bishop::get_moveset(int row, int col, const QString board[8][8]){
    moveset.clear();
    // 4 diagonals for bishop //
    moves_line(row, col,  1,  1, board, moveset);
    moves_line(row, col,  1, -1, board, moveset);
    moves_line(row, col, -1,  1, board, moveset);
    moves_line(row, col, -1, -1, board, moveset);
    return moveset;
}
// --------- // Knight // ------------------------------------------------------------------------------------ //

Knight::Knight(bool w) : Piece(w) {
    type = isWhite ? piece_type(wN) : piece_type(bN);
}
Knight::~Knight(){}
std::vector<std::pair<int,int>>Knight::get_moveset(int row, int col, const QString board[8][8]){
    moveset.clear();
    // all 8 possible moves for knights //
    knight_move(row, col, 2,  1, board, moveset);
    knight_move(row, col, 2, -1, board, moveset);
    knight_move(row, col, 1,  2, board, moveset);
    knight_move(row, col, 1, -2, board, moveset);
    knight_move(row, col, -1,  2, board, moveset);
    knight_move(row, col, -1, -2, board, moveset);
    knight_move(row, col, -2,  1, board, moveset);
    knight_move(row, col, -2, -1, board, moveset);
    return moveset;
}
// --------- // Pawn // ------------------------------------------------------------------------------------- //

Pawn::Pawn(bool w) : Piece(w) {
    type = isWhite ? piece_type(wP) : piece_type(bP);
}
Pawn::~Pawn(){}
std::vector<std::pair<int,int>>Pawn::get_moveset(int row, int col, const QString board[8][8]){
    moveset.clear();
    int direction = 1;
    if (isWhite){direction = -1;}
    int up_one = row+direction;
    qDebug() << "test";
    if (board[up_one][col] == ""){
        moveset.push_back(std::make_pair(up_one, col));
        if (!hasMoved && board[up_one + direction][col] == ""){
            moveset.push_back(std::make_pair(up_one + direction, col));
        }
    }
    /*
    if (board[up_one][col+direction].at(0) == opposite){
        moveset.push_back(std::make_pair(up_one, col+direction));
    }
    if (board[up_one][col+direction].at(0) == opposite){
        moveset.push_back(std::make_pair(up_one, col-direction));
    }
    */
    return moveset;
}
// --------------------------------------------------------------------------------------------------------- //

