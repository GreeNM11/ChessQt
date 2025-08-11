#include "chess_pieces.h"

// --------- // Base Piece Class // -------------------------------------------------------------------------- //

Piece::Piece(bool isWhite, bool playerIsWhite) : isWhite(isWhite), playerIsWhite(playerIsWhite){
    same = !isWhite ? 'b' : 'w';
    opposite = isWhite ? 'b' : 'w';
}
Piece::~Piece(){}
void Piece::has_moved(){ hasMoved = true; }
bool Piece::get_moved(){ return hasMoved;}
bool Piece::get_color() const { return isWhite; }
QString Piece::get_piece_type() const { return type; }

std::vector<std::pair<int,int>>Piece::get_moveset(int row, int col, const QString board[8][8], Piece* p_board[8][8], Piece* last_moved){return moveset;}

std::vector<std::pair<int,int>> Piece::check_if_pinned(int row, int col, const QString board[8][8]){
    moveset.clear();
    int c;
    for (c = 0;c < 8; c++){ // if a king is in a line, then add the whole line to the moveset //
        moves_line(row, col,  direction[c][0], direction[c][1], board, moveset); // tries to find king //
        int king_row = row+direction[c][0];
        int king_col = col+direction[c][1]; // adds one increment, moveset stops before friendly piece //
        if (!moveset.empty()){
            king_row = moveset.back().first+direction[c][0];
            king_col = moveset.back().second+direction[c][1];
        }
        if (king_row >= 0 && king_row < 8 && king_col >= 0 && king_col < 8 && board[king_row][king_col] == same + 'K'){
            moves_line(row, col,  -1*direction[c][0], -1*direction[c][1], board, moveset); // adds other side of line //
            break; // stops clear to use moveset after //
        }
        moveset.clear();
    }
    if (moveset.empty()){return {};} // found no king to be pinned to //

    QString end = board[moveset.back().first][moveset.back().second]; // piece that was found, end of vector //
    if (end == "" || end.at(0) == same || QString("PKN").contains(end.at(1))){
        return {}; // no enemy piece, friendly piece to pin, or enemy piece: pawn, king, or knight cannot pin//
    }
    else if (c < 4 && end == (opposite + 'Q') || end == (opposite + 'R')){
        return moveset; // found enemy Q or R on up or down lines //
    }
    else if (c >= 4 && end == (opposite + 'Q') || end == (opposite + 'B')){
        return moveset; // found enemy Q or B on diagonal lines //
    }
    return {};
}

bool Piece::get_move_twice(){return move_twice;}

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
        else if (board[r][c].at(0) == same){
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

King::King(bool isWhite, bool playerIsWhite) : Piece(isWhite, playerIsWhite) {
    type = isWhite ? "wK" : "bK";
}
King::~King(){}
std::vector<std::pair<int,int>>King::get_moveset(int row, int col, const QString board[8][8], Piece* p_board[8][8], Piece* last_moved){
    moveset.clear();
    QString b[8][8];
    for (int r=0; r<8; r++){
        for (int c=0; c<8; c++){
            if (board[r][c] != same+'K'){
                b[r][c] = board[r][c]; // makes board without same side king to avoid king blocking checks //
            }
        }
    }
    // All 8 possible king moves //
    for (int i = 0; i < 8; i++){
        add_valid_move(row+direction[i][0], col+direction[i][1], b, moveset);
    }
    if (!hasMoved && check_if_valid(row, col, board).empty()){add_castle_move(row, col, board, p_board);}
    return moveset;
}

void King::add_castle_move(int row, int col, const QString board[8][8], Piece* p_board[8][8]){
    for (int c = col+1; c < 8; c++){ // right side castle //
        if (board[row][c] == same + 'R' && p_board[row][c]->get_moved() == false){
            moveset.push_back(std::make_pair(row, col+2));
        }
        else if (board[row][c] != "" || (c - col <= 2 && !check_if_valid(row, c, board).empty())){
            break;
        }
    }
    for (int c = col-1; c >= 0; c--){ // left side castle //
        if (board[row][c] == same + 'R' && p_board[row][c]->get_moved() == false){
            moveset.push_back(std::make_pair(row, col-2));
        }
        else if (board[row][c] != "" || (col - c <= 2 && !check_if_valid(row, c, board).empty())){
            break;
        }
    }
}

std::vector<std::pair<int,int>> King::is_in_check(int row, int col, const QString board[8][8]){
    return check_if_valid(row,col,board);
}

void King::add_valid_move(int row, int col, const QString board[8][8], std::vector<std::pair<int,int>> &m){
    if (check_if_valid(row,col,board).empty()){
        m.push_back(std::make_pair(row, col));
    }
}

std::vector<std::pair<int,int>> King::check_if_valid(int row, int col, const QString board[8][8]){
    if (row >= 8 || row < 0 || col >= 8 || col < 0 || (board[row][col] != nullptr && board[row][col].at(0) == same && board[row][col] != same + 'K')){
        return {{-2,-1}};  // cant move outside board or same color piece
    }
    std::vector<std::pair<int,int>> check_pieces;
    std::vector<std::pair<int,int>> block_check_list;
    int check_count = 0; // account for double checks //

    // Checks all tiles where a queen or rook can be checking //
    for (int i = 0; i < 4; i++){
        moves_line(row, col,  direction[i][0], direction[i][1], board, check_pieces);
        for (auto tile : check_pieces){
            QString p = board[tile.first][tile.second];
            if (p == (opposite + 'Q') || p == (opposite + 'R')){
                check_count += 1; // tile being covered by queen or rook //
                block_check_list = check_pieces;
            }
        }
        check_pieces.clear();
    }
    if (check_count > 1){return {{-1,-1}};} // returns if double check, cannot be blocked //
//-------------------------------------------------------------------------------------------------------------//
    // Checks all tiles where queen or bishop can be checking //
    for (int i = 4; i < 8; i++){
        moves_line(row, col,  direction[i][0], direction[i][1], board, check_pieces);
        for (auto tile : check_pieces){
            QString p = board[tile.first][tile.second];
            if (p == (opposite + 'Q') || p == (opposite + 'B')){
                check_count += 1; // tile being covered by queen or bishop //
                block_check_list = check_pieces;
            }
        }
        check_pieces.clear();
    }
    if (check_count > 1){return {{-1,-2}};}

//-------------------------------------------------------------------------------------------------------------//
    // Checks all 8 tiles where a knight can be checking //
    for (int i = 0; i < 8; i++){
        knight_move(row, col,  knight_direction[i][0], knight_direction[i][1], board, check_pieces);
        for (auto tile : check_pieces){
            QString p = board[tile.first][tile.second];
            if (p == (opposite + 'N')){
                check_count += 1; // tile being covered by knight, cannot be blocked //
                block_check_list.push_back(std::make_pair(row+knight_direction[i][0], col+knight_direction[i][1])); // can capture the knight //
            }
        }
    }
    if (check_count > 1){return {{-1,-3}};}

//-------------------------------------------------------------------------------------------------------------//
    // Checks the 2 pawn positions that can check //
    int up = 1;
    if (!isWhite){ up = -1; }
    if (same == 'w' && row-up >= 0 && row-up < 8){ // white king scenario //
        if (col+1 < 8 && board[row-up][col+1] == (opposite + 'P')){
            check_count += 1;
            block_check_list.push_back(std::make_pair(row-up, col+1));
        }
        else if(col-1 >= 0 && board[row-up][col-1] == (opposite + 'P')){
            check_count += 1;
            block_check_list.push_back(std::make_pair(row-up, col-1));
        }
    }
    else if(same == 'b' && row-up >= 0 && row-up < 8){ // black king scenario //
        if (col+1 < 8 && board[row+up][col+1] == (opposite + 'P')){
            check_count += 1;
            block_check_list.push_back(std::make_pair(row+up, col+1));
        }
        else if(col-1 >= 0 && board[row+up][col-1] == (opposite + 'P')){
            check_count += 1;
            block_check_list.push_back(std::make_pair(row+up, col-1));
        }
    }
    if (check_count > 1){return {{-1,-4}};}

//-------------------------------------------------------------------------------------------------------------//
    // Checks if enemy king covers, 8 tiles //
    bool pawn_check = false;
    if (row >= 0 && col+1 < 8 &&       board[row-1][col+1] == (opposite + 'K')){
        check_count += 1;
        pawn_check = true;
    }
    else if (row >= 0 &&               board[row-1][col  ] == (opposite + 'K')){
        check_count += 1;
        pawn_check = true;
    }
    else if (row >= 0 && col-1 >= 0 && board[row-1][col-1] == (opposite + 'K')){
        check_count += 1;
        pawn_check = true;
    }
    else if (row < 8 && col+1 < 8 &&   board[row+1][col+1] == (opposite + 'K')){
        check_count += 1;
        pawn_check = true;
    }
    else if (row < 8 &&                board[row+1][col  ] == (opposite + 'K')){
        check_count += 1;
        pawn_check = true;
    }
    else if (row < 8 && col-1 >= 0 &&  board[row+1][col-1] == (opposite + 'K')){
        check_count += 1;
        pawn_check = true;
    }
    else if (col-1 >= 0 &&             board[row  ][col-1] == (opposite + 'K')){
        check_count += 1;
        pawn_check = true;
    }
    else if (col+1 < 8 &&              board[row  ][col+1] == (opposite + 'K')){
        check_count += 1;
        pawn_check = true;
    }
    if (pawn_check){block_check_list.push_back(std::make_pair(-1, -1));}
    if (check_count > 1){return {{-1,-5}};}

    // if block_check_list empty, then no check //
    return block_check_list; // if check count only 1, then returns move list //
}

// --------- // Queen // ------------------------------------------------------------------------------------- //

Queen::Queen(bool isWhite, bool playerIsWhite) : Piece(isWhite, playerIsWhite) {
    type = isWhite ? "wQ" : "bQ";
}
Queen::~Queen(){}
std::vector<std::pair<int,int>>Queen::get_moveset(int row, int col, const QString board[8][8], Piece* p_board[8][8], Piece* last_moved){
    moveset.clear();
    // combines both bishop and rook moves, entire direction array //
    for (int i = 0; i < 8; i++){
        moves_line(row, col,  direction[i][0], direction[i][1], board, moveset);
    }
    return moveset;
}
// --------- // Rook // -------------------------------------------------------------------------------------- //

Rook::Rook(bool isWhite, bool playerIsWhite) : Piece(isWhite, playerIsWhite) {
    type = isWhite ? "wR" : "bR";
}
Rook::~Rook(){}
std::vector<std::pair<int,int>>Rook::get_moveset(int row, int col, const QString board[8][8], Piece* p_board[8][8], Piece* last_moved){
    moveset.clear();
    // 2 horizontal and 2 vertical lines, first half of direction array //
    for (int i = 0; i < 4; i++){
        moves_line(row, col,  direction[i][0], direction[i][1], board, moveset);
    }
    return moveset;
}
// --------- // Bishop // ------------------------------------------------------------------------------------ //

Bishop::Bishop(bool isWhite, bool playerIsWhite) : Piece(isWhite, playerIsWhite) {
    type = isWhite ? "wB" : "bB";
}
Bishop::~Bishop(){}
std::vector<std::pair<int,int>>Bishop::get_moveset(int row, int col, const QString board[8][8], Piece* p_board[8][8], Piece* last_moved){
    moveset.clear();
    // 4 diagonals for bishop, second half of direction array //
    for (int i = 4; i < 8; i++){
        moves_line(row, col,  direction[i][0], direction[i][1], board, moveset);
    }
    return moveset;
}
// --------- // Knight // ------------------------------------------------------------------------------------ //

Knight::Knight(bool isWhite, bool playerIsWhite) : Piece(isWhite, playerIsWhite) {
    type = isWhite ? "wN" : "bN";
}
Knight::~Knight(){}
std::vector<std::pair<int,int>>Knight::get_moveset(int row, int col, const QString board[8][8], Piece* p_board[8][8], Piece* last_moved){
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

Pawn::Pawn(bool isWhite, bool playerIsWhite) : Piece(isWhite, playerIsWhite) {
    type = isWhite ? "wP" : "bP";
}
Pawn::~Pawn(){}
std::vector<std::pair<int,int>>Pawn::get_moveset(int row, int col, const QString board[8][8], Piece* p_board[8][8], Piece* last_moved){
    moveset.clear();
    int direction = 1;
    if (isWhite ^ !playerIsWhite){direction = -1;}
    int up_one = row+direction;

    if (board[up_one][col] == ""){ // forward moves //
        moveset.push_back(std::make_pair(up_one, col));
        if (!hasMoved && board[up_one + direction][col] == ""){
            moveset.push_back(std::make_pair(up_one + direction, col));
            move_twice = true; // for en passant availability //
        }
    }
    // capturing moves //
    if (col+1 < 8){ // right side scenario //
        if (board[up_one][col+1].at(0) == opposite){
            moveset.push_back(std::make_pair(up_one, col+1));
        }
        else if (board[row][col+1] == opposite + 'P' && p_board[row][col+1] == last_moved){ // en passant //
            qDebug() << last_moved->get_piece_type();
            if (last_moved->get_move_twice() == true){
                moveset.push_back(std::make_pair(up_one, col+1));
            }
        }
    }
    if (col-1 > 0){ // left side scenario //
        if (board[up_one][col-1].at(0) == opposite){
            moveset.push_back(std::make_pair(up_one, col-1));
        }
        else if (board[row][col-1] == opposite + 'P' && p_board[row][col-1] == last_moved){
            qDebug() << last_moved->get_piece_type();
            if (last_moved->get_move_twice() == true){
                moveset.push_back(std::make_pair(up_one, col-1));
            }
        }
    }
    return moveset;
}
// --------------------------------------------------------------------------------------------------------- //

