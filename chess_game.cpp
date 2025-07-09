#include "chess_game.h"

Piece* chess_game::make_piece(QString pieceCode){

    bool w = true;
    if (pieceCode.at(0) == "b"){
        w = false;
    }

    Piece* p;

    if (pieceCode.at(1) == "K"){
        p = new King(w);
        if (pieceCode.at(0) == "w"){white_king = dynamic_cast<King*>(p);}
        else if (pieceCode.at(0) == "b") {black_king = dynamic_cast<King*>(p);}
    }
    else if (pieceCode.at(1) == "Q"){
        p = new Queen(w);
    }
    else if (pieceCode.at(1) == "R"){
        p = new Rook(w);
    }
    else if (pieceCode.at(1) == "B"){
        p = new Bishop(w);
    }
    else if (pieceCode.at(1) == "N"){
        p = new Knight(w);
    }
    else if (pieceCode.at(1) == "P"){
        p = new Pawn(w);
    }

    return p;
}

PieceLabel* chess_game::make_piece_label(int row, int col, QString pieceCode, int tileSize, QMap<QString, QPixmap> pieceImages){
    Piece* piece_object = make_piece(pieceCode);
    PieceLabel* p_label = new PieceLabel(ui->labelBoard, piece_object, row, col);
    // sets up the label and sprite //
    p_label->setPixmap(pieceImages[pieceCode]);
    p_label->setScaledContents(true);
    p_label->setFixedSize(tileSize, tileSize);
    p_label->show();
    p_label->move_piece(row, col, tileSize);
    piece_label_board[row][col] = p_label;

    // Connects piece behavior when clicked on //
    connect(piece_label_board[row][col], &PieceLabel::clicked, this, [=](PieceLabel* clicked_label){
        click_piece_action(clicked_label);
    });

    if (pieceCode == "wK"){white_king_label = p_label;}
    else if (pieceCode == "bK") {black_king_label = p_label;}

    return p_label;
}

void chess_game::deselect_all(){
    if (s_label != nullptr){
        s_label->deselect();
    }
    if (!in_check){
        for (const auto& move : s_move_list) {Tiles[move.first][move.second]->deselect();}
    }
    else{
        for (const auto& move : block_move_list) {Tiles[move.first][move.second]->deselect();}
    }

    s_move_list.clear();
    s_label = nullptr;
}

void chess_game::select_piece(PieceLabel* clicked_label){
    // sets selected piece, gets moveset, and displays it //
    if (clicked_label != nullptr){
        s_label = clicked_label;
        s_label->select();
        s_move_list = s_label->get_object()->get_moveset(s_label->get_row(), s_label->get_col(), board);
        if (in_check){
            std::vector<std::pair<int, int>> result;
            for (const auto& pair : block_move_list) {
                if (std::find(s_move_list.begin(), s_move_list.end(), pair) != s_move_list.end()) {
                    result.push_back(pair);
                }
            }
            s_move_list = result;
        }
        for (const auto& move : s_move_list) {Tiles[move.first][move.second]->select();}
    }
}

void chess_game::switch_turn(){
    white_king_label->deselect();
    black_king_label->deselect();

    white_turn = !white_turn;
    in_check = check_if_in_check();
    if (white_turn && in_check){white_king_label->check_king();}
    else if (!white_turn && in_check){black_king_label->check_king();}

}

void chess_game::move_piece(PieceLabel* p, int new_row, int new_col){
    int p_col = s_label->get_col();
    int p_row = s_label->get_row();
    // updates the label, label board, and string board //
    s_label->move_piece(new_row, new_col, tileSize);
    s_label->get_object()->has_moved();
    piece_label_board[new_row][new_col] = piece_label_board[p_row][p_col];
    piece_label_board[p_row][p_col] = nullptr;
    board[new_row][new_col] = board[p_row][p_col];
    board[p_row][p_col] = "";
}

void chess_game::click_piece_action(PieceLabel* clicked_label){
    bool same = (clicked_label == s_label);
    int clicked_row = clicked_label->get_row();
    int clicked_col = clicked_label->get_col();

    if (clicked_label->get_color() == white_turn){ // clicked on piece and is players turn //
        if (s_label == nullptr){
            // if no piece selected, set selected piece and get moveset //
            select_piece(clicked_label);
        }
        else if (!same){
            // deselected old piece and selected new piece and get new moveset //
            deselect_all();
            select_piece(clicked_label);
        }
    }

    if (clicked_label->get_color() != white_turn){ // clicked on enemy piece //
        bool capture_piece = false;
        int r; int c;
        for (const auto& move : s_move_list) {
            // checks if piece that got clicked on is to be captured //
            if (move.first == clicked_row && move.second == clicked_col){
                delete clicked_label;
                move_piece(s_label, clicked_row, clicked_col);
                deselect_all();
                switch_turn();
                break;
            }
        }
    }
}

void chess_game::click_tile_action(ClickableTileLabel* tile){
    if (s_label != nullptr){
        for (const auto& move : s_move_list) {
            if (tile == Tiles[move.first][move.second]){ // clicked on tile is in moveset //
                move_piece(s_label, move.first, move.second);
                switch_turn();
            }
            Tiles[move.first][move.second]->deselect();
        }
        s_label->deselect();
        s_label = nullptr;
    }
}

bool chess_game::check_if_in_check(){
    if (white_turn){
        block_move_list = white_king->is_in_check(white_king_label->get_row(), white_king_label->get_col(),board);
    }
    else{
        block_move_list = black_king->is_in_check(black_king_label->get_row(), black_king_label->get_col(),board);
    }
    if (!block_move_list.empty()){
        if (block_move_list.at(0).first == -1){ // double check case
            block_move_list.clear();
            return true;
        }
        else if(block_move_list.at(0).first == -2){ // invalid case, not check
            block_move_list.clear();
            return false;
        }
        else{ return true; } // 1 check
    }
    return false;
}

void chess_game::setup_board(){
    // Load the board image from resources and set it on labelBoard
    QPixmap boardPixmap(":/images/board.png");
    ui->labelBoard->setFixedSize(640, 640);
    ui->labelBoard->setPixmap(boardPixmap);
    ui->labelBoard->setFixedSize(boardPixmap.size());  // Fix size to board size

    // Load the sprite sheet for pieces
    QPixmap sprite(":/images/pieces.png");

    // Creates the pieces spirtes from the pieces png //
    pieceImages["wK"] = sprite.copy(0 * (pieceWidth + 4), 0, pieceWidth, pieceHeight);
    pieceImages["wQ"] = sprite.copy(1 * (pieceWidth + 4), 0, pieceWidth, pieceHeight);
    pieceImages["wB"] = sprite.copy(2 * (pieceWidth + 4), 0, pieceWidth, pieceHeight);
    pieceImages["wN"] = sprite.copy(3 * (pieceWidth + 4), 0, pieceWidth, pieceHeight);
    pieceImages["wR"] = sprite.copy(4 * (pieceWidth + 4), 0, pieceWidth, pieceHeight);
    pieceImages["wP"] = sprite.copy(5 * (pieceWidth + 3.7), 0, pieceWidth, pieceHeight);

    pieceImages["bK"] = sprite.copy(0 * (pieceWidth + 4), pieceHeight + 7, pieceWidth, pieceHeight);
    pieceImages["bQ"] = sprite.copy(1 * (pieceWidth + 4), pieceHeight + 7, pieceWidth, pieceHeight);
    pieceImages["bB"] = sprite.copy(2 * (pieceWidth + 4), pieceHeight + 7, pieceWidth, pieceHeight);
    pieceImages["bN"] = sprite.copy(3 * (pieceWidth + 4), pieceHeight + 7, pieceWidth, pieceHeight);
    pieceImages["bR"] = sprite.copy(4 * (pieceWidth + 4), pieceHeight + 7, pieceWidth, pieceHeight);
    pieceImages["bP"] = sprite.copy(5 * (pieceWidth + 3.7), pieceHeight + 7, pieceWidth, pieceHeight);

    for (int row = 0; row < 8; ++row) {
        // Creates the 64 clickable tile labels //
        for (int col = 0; col < 8; ++col) {
            Tiles[row][col] = new ClickableTileLabel(row, col, ui->labelBoard);
            Tiles[row][col]->setGeometry(col * tileSize, row * tileSize, tileSize, tileSize);
            Tiles[row][col]->setStyleSheet("");
            Tiles[row][col]->show();

            connect(Tiles[row][col], &ClickableTileLabel::TileClicked, this, [=](ClickableTileLabel* tile){
                // Connects behavior when empty tile clicked on //
                click_tile_action(tile);
            });
        }
    }

    for (int col = 0; col < 8; ++col) {
        // Creates the 32 pieces objects at starting positions //
        for (int row = 0; row < 8; ++row) {
            if (board[row][col] != ""){
                make_piece_label(row, col, board[row][col], tileSize, pieceImages);
            }
        }
    }
}

chess_game::chess_game(Ui::MainWindow* ui, int time = 0, int increment = 0) : ui(ui), time(time), increment(increment){
    setup_board(); // sets up starting board position //
}

chess_game::~chess_game()
{
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            delete Tiles[row][col];
            delete piece_label_board[row][col];
        }
    }
}
