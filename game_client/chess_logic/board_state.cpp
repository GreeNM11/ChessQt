#include "board_state.h"
#include <QDebug>
//----------------------------------- Handles Object Memory  -----------------------------------------//

void board_state::make_piece(QString pieceCode, int row, int col){
    if (pieceCode.at(1) == 'K'){
        piece_board[row][col] = new King(pieceCode, row, col, isWhite);
        if (pieceCode.at(0) == 'w'){white_king = dynamic_cast<King*>(piece_board[row][col]);}
        else if (pieceCode.at(0) == 'b') {black_king = dynamic_cast<King*>(piece_board[row][col]);}
    }
    else if (pieceCode.at(1) == 'Q'){ piece_board[row][col] = new Queen(pieceCode, row, col, isWhite); }
    else if (pieceCode.at(1) == 'R'){ piece_board[row][col] = new Rook(pieceCode, row, col, isWhite); }
    else if (pieceCode.at(1) == 'B'){ piece_board[row][col] = new Bishop(pieceCode, row, col, isWhite); }
    else if (pieceCode.at(1) == 'N'){ piece_board[row][col] = new Knight(pieceCode, row, col, isWhite); }
    else if (pieceCode.at(1) == 'P'){ piece_board[row][col] = new Pawn(pieceCode, row, col, isWhite); }

    emit make_piece_label(pieceCode, row, col);
    board[row][col] = pieceCode;
    if (pieceCode.at(0) == 'w'){ white_pieces.push_back(piece_board[row][col]); }
    else if (pieceCode.at(0) == 'b'){ black_pieces.push_back(piece_board[row][col]); }
}
void board_state::capture_piece(int row, int col){
    Piece* delPiece = piece_board[row][col];
    if (delPiece->get_color() == true){ // removes from piece vectors of white or black //
        white_pieces.erase(
        std::remove(white_pieces.begin(), white_pieces.end(), piece_board[row][col]),
        white_pieces.end());
    }
    else {
        black_pieces.erase(
        std::remove(black_pieces.begin(), black_pieces.end(), piece_board[row][col]),
        black_pieces.end());
    }
    board[row][col] = "";
    delete piece_board[row][col];
    piece_board[row][col] = nullptr;
    emit capture_piece_label(row, col);
}

//----------------------------------- Handles Piece Interaction -----------------------------------------//

void board_state::click_piece(int row, int col){

    // Checks if clicked piece is friendly, enemy, or invalid //

    if (row > 7 || row < 0 || col > 7 || col < 0 || piece_board[row][col] == nullptr ||
        (selected_piece != nullptr && row == selected_piece->get_row() && col == selected_piece->get_col()))
        { return; } // Cant be out of bounds, nullptr, or same piece //
    if ((isWhite == white_turn) || !isOnline){ // Player cant move unless their turn online or singleplayer //
        bool is_white_piece = piece_board[row][col]->get_color();
        if (is_white_piece == white_turn){
            select_piece(row, col); // Select piece thats their own //
        }
        else if (is_white_piece != white_turn && selected_piece != nullptr){ // clicked on enemy piece //
            int r; int c;
            for (const auto& move : s_move_list) {
                // checks if piece that got clicked on is to be captured //
                if (move.first == row && move.second == col){ // if piece is in the moveset //
                    send_move_request(selected_piece->get_row(), selected_piece->get_col(), row, col);
                    break;
                }
            }
            deselect_piece();
        }
    }
}
void board_state::click_tile(int row, int col){

    // Checks valid square, and if clicked on square is in moveset //

    if (row > 7 || row < 0 || col > 7 || col < 0 || selected_piece == nullptr){ return; }

    for (const auto& move : s_move_list) {
        if (row == move.first && col == move.second){ // clicked on tile is in moveset //
            send_move_request(selected_piece->get_row(), selected_piece->get_col(), row, col);
            return;
        }
    }
    deselect_piece();
}

void board_state::select_piece(int row, int col){

    // Gets available moveset of piece //

    deselect_piece();
    selected_piece = piece_board[row][col];

    s_move_list = selected_piece->get_moveset(board, piece_board, last_moved);

    // If not king //
    if (board[selected_piece->get_row()][selected_piece->get_col()].at(1) != 'K'){
        std::vector<std::pair<int, int>> result;
        if (in_check){ // Checks if in check //
            // Limit moveset to block checks //
            for (const auto& pair : block_move_list) { // block_move_list is set in check_if_in_check //
                if (std::find(s_move_list.begin(), s_move_list.end(), pair) != s_move_list.end()) {
                    result.push_back(pair); // only includes moves that are valid and stop check //
                }
            }
            s_move_list = result; // only provides moves that get out of check //
        }
        else{ // Checks if the piece is pinned //
            std::vector<std::pair<int, int>>pin_move_list = selected_piece->check_if_pinned(board);
            if (!pin_move_list.empty()){
                for (const auto& pair : pin_move_list) {
                    if (std::find(s_move_list.begin(), s_move_list.end(), pair) != s_move_list.end()) {
                        result.push_back(pair);
                    }
                }
                s_move_list = result; // only provides moves that pinned piece can do //
            }
        }
    }
    emit highlight_tiles(s_move_list, true); // updates labels to highlight //
    emit highlight_piece(row, col, true);

}
void board_state::deselect_piece(){

    // Deselects selected_piece and rids piece highlight //

    if (selected_piece != nullptr){ // Server version has no ui to emit to //
        emit highlight_piece(selected_piece->get_row(), selected_piece->get_col(), false);
        selected_piece = nullptr;
    }
    if (!s_move_list.empty()){
        emit highlight_tiles(s_move_list, false);// rids old highlighted moves //
        s_move_list.clear();
    }

}

void board_state::send_move_request(int p_row, int p_col, int new_row, int new_col){
    deselect_piece();
    // Emits string of 4 numbers for piece move to server //
    if (!isOnline) {
        move_piece(p_row, p_col, new_row, new_col);
    }
    else if (isWhite == white_turn && isOnline){ // Must validate move with server to move //
        int moveInt = p_row * 1000 + p_col * 100 + new_row * 10 + new_col;
        QString move = QString::number(moveInt);
        if (moveInt < 1000){ move = "0" + move; } // no 0 in front of int //
        emit send_player_move(move, isWhite);
    }
}
void board_state::receive_move(QString move){
    // Decodes String into 4 integers //
    int from_row = move.at(0).digitValue();
    int from_col = move.at(1).digitValue();
    int to_row = move.at(2).digitValue();
    int to_col = move.at(3).digitValue();

    move_piece(from_row, from_col, to_row, to_col);
}
void board_state::send_checkmate_request(int code){ emit checkmated(code); }

void board_state::move_piece(int p_row, int p_col, int new_row, int new_col){

    // Updates moves in string and object boards  //
    bool capture = false;
    if (piece_board[new_row][new_col] != nullptr){ // Removes piece if is capture //
        capture = true;
        capture_piece(new_row, new_col);
    }

    // Updates object board and string board //
    piece_board[new_row][new_col] = piece_board[p_row][p_col];
    piece_board[p_row][p_col] = nullptr;

    piece_board[new_row][new_col]->set_rowcol(new_row, new_col);
    piece_board[new_row][new_col]->has_moved();

    board[new_row][new_col] = board[p_row][p_col];
    board[p_row][p_col] = "";
    emit move_piece_label(p_row, p_col, new_row, new_col);

    // Pawn Mechancis (en passant and promotion) //
    if (piece_board[new_row][new_col]->get_piece_type().at(1) == 'P'){pawn_mechanics(p_row, p_col, new_row, new_col, capture);}

    // Castling mechanics //
    if (piece_board[new_row][new_col]->get_piece_type().at(1) == 'K'){ // right side castle check //
        if (new_col - p_col > 1){
            for (int c = new_col+1; c < 8; c++){
                if (board[new_row][c].at(1) == 'R'){
                    // finds the right rook and castles it //
                    move_piece(new_row, c, new_row, new_col-1);
                    return;
                }
            }
        }
        else if (new_col - p_col < -1){ // left side castle check //
            for (int c = new_col-1; c >= 0; c--){
                if (board[new_row][c].at(1) == 'R'){
                    // finds the left rook and castles it //
                    move_piece(new_row, c, new_row, new_col+1);
                    return;
                }
            }
        }
    }

    // dehighlights old moved piece label //
    if (last_tile != std::make_pair(-1, -1) && last_moved){
        emit highlight_piece(last_moved->get_row(), last_moved->get_col(), false);
        emit highlight_tiles({{last_tile.first, last_tile.second}}, false);
    }
    // highlights new moved //
    emit highlight_piece(new_row, new_col, true);
    emit highlight_tiles({{p_row, p_col}}, true);

    last_tile = {p_row, p_col};
    last_moved = piece_board[new_row][new_col]; // tracks last move //

    // non castling switch moves //
    switch_turn();
}
void board_state::pawn_mechanics(int old_row, int old_col, int row, int col, bool capture){

    // Checks Doueble First move, Pawn Promotion, En peassant //

    bool promote = false;
    QChar pawn_color = piece_board[row][col]->get_piece_type().at(0);
    QString selected_piececode;

    int white_promotion_row;
    int black_promotion_row;

    // Checks if Pawn Moved 2 Squsres //
    if (std::abs(old_row - row) > 1){ piece_board[row][col]->set_move_twice(); }

    // Checks Promotion //
    if (isWhite){ white_promotion_row = 0; black_promotion_row = 7; }
    else { white_promotion_row = 7; black_promotion_row = 0; }

    if (pawn_color == 'w' && row == white_promotion_row){
        selected_piececode = "wQ";
        promote = true;
    }
    else if (pawn_color == 'b' && row == black_promotion_row){
        selected_piececode = "bQ";
        promote = true;
    }

    if (promote == true){
        capture_piece(row, col); // captures the promoting pawn //
        make_piece(selected_piececode, row, col); // makes the new promoted piece //
    }
    // Checks Enpeassant //
    if (!capture && old_col != col){ // if moves diagonal without capture, adds the en passant capture //
        capture_piece(old_row, col);
    }
}

//----------------------------------- Handles Board States -----------------------------------------//

void board_state::switch_turn(){
    deselect_piece();
qDebug() << "1a";
    white_turn = !white_turn; // switches turn //

    in_check = check_if_in_check(); // always sees if in check, accounts for discoveries //
    qDebug() << "2a";
    emit check_king_labels(white_turn, in_check); // checks king labels //
    if (in_check && check_if_checkmated()){ send_checkmate_request(checkmateCode); }
    qDebug() << "2b";
}

bool board_state::check_if_in_check(){
    block_move_list.clear();
    if (white_turn && white_king != nullptr){
        block_move_list = white_king->is_in_check(board);
    }
    else if (!white_turn && black_king != nullptr){
        block_move_list = black_king->is_in_check(board);
    }

    if (!block_move_list.empty()){
        if (block_move_list.at(0).first == -1){ // double check case or king guarding square //
            block_move_list.clear();
            return true;
        }
        else if(block_move_list.at(0).first == -2){ // invalid case, not check //
            block_move_list.clear();
            return false;
        }
        else { return true; } // 1 check //
    }
    else { return false; } // is empty //
}
bool board_state::check_if_checkmated(){
    // Check if white king got checkmated //
    if (white_turn){
        // white king cant move //
        if (!white_king->get_moveset(board, piece_board, last_moved).empty()) { return false; }

        std::vector<std::pair<int, int>> w_moveset;
        // checks all white piece movesets if can block //
        for (Piece* w_piece : white_pieces){
            w_moveset = w_piece->get_moveset(board, piece_board, last_moved);
            for (const auto& block_move : block_move_list) { // if move in block_move, then its not checkmate //
                if (std::find(w_moveset.begin(), w_moveset.end(), block_move) != w_moveset.end()) {
                    return false; // found a piece that can block in its moveset //
                }
            }
        }
        checkmateCode = 1;
    }
    // Check if black got checkmated //
    else {
        // black king cant move //
        if (!black_king->get_moveset(board, piece_board, last_moved).empty()) { return false; }

        std::vector<std::pair<int, int>> b_moveset;
        // checks all black piece movesets if can block //
        for (Piece* b_piece : black_pieces){
            b_moveset = b_piece->get_moveset(board, piece_board, last_moved);
            for (const auto& block_move : block_move_list) { // if move in block_move, then its not checkmate //
                if (std::find(b_moveset.begin(), b_moveset.end(), block_move) != b_moveset.end()) {
                    return false; // found a piece that can block in its moveset //
                }
            }
        }
        checkmateCode = 2;
    }
    return true; // cant escape check //
}
void board_state::setup_board(){
    if (!isWhite){ // flips the board if black perspective //
        QString flippedBoard[8][8];
        for (int row = 0; row < 8; ++row) {
            for (int col = 0; col < 8; ++col) {
                flippedBoard[row][col] = board[7 - row][7 - col];
            }
        }
        for (int row = 0; row < 8; ++row) {
            for (int col = 0; col < 8; ++col) {
                board[row][col] = flippedBoard[row][col];
            }
        }
    }
    for (int row = 0; row < 8; row++) {
        // Creates the 32 pieces objects at starting positions //
        for (int col = 0; col < 8; col++) {
            piece_board[row][col] = nullptr;
            if (board[row][col] != ""){
                make_piece(board[row][col], row, col);
            }
        }
    }
    white_turn = true;
}

//----------------------------------- Class Defaults -----------------------------------------//

board_state::board_state(bool isWhite, bool isOnline) : isWhite(isWhite), isOnline(isOnline) {}
board_state::~board_state(){
    for (int row = 0; row < 8; ++row) {
        // Deletes all piece objects //
        for (int col = 0; col < 8; ++col) {
            if (piece_board[row][col] != nullptr){
                delete piece_board[row][col];
            }
        }
    }
}

//----------------------------------- Server Validation -----------------------------------------//

int board_state::validate_move(QString move){
    if (move.length() < 4){ return 1; }
    int from_row = move.at(0).digitValue();
    int from_col = move.at(1).digitValue();
    int to_row   = move.at(2).digitValue();
    int to_col   = move.at(3).digitValue();

    // Bounds check //
    if ((from_row < 0 || from_row >= 8) ||
        (from_col < 0 || from_col >= 8) ||
        (to_row < 0 || to_row >= 8) ||
        (to_col < 0 || to_col >= 8)) {
        return 1; // out of bounds //
    }
    // Piece existence check //
    if (piece_board[from_row][from_col] == nullptr) { return 2; } // no piece to move //
    Piece* piece = piece_board[from_row][from_col];

    // Turn check
    if (piece->get_color() != white_turn) {
        return 3; // not players turn //
    }
    // Generate moveset (without selecting in UI)
    auto moveset = piece->get_moveset(board, piece_board, last_moved);

    // Is destination in moveset?
    if (std::find(moveset.begin(), moveset.end(), std::make_pair(to_row, to_col)) == moveset.end()) {
        return 4; // move not legal for that piece
    }

    return 0; // valid move
}
int board_state::validate_checkmated(){ return checkmateCode; }
