#include "chess_game.h"
//----------------------------------- |Slots| Handles Label Ability  -----------------------------------------//

void chess_game::make_piece_label(QString pieceCode, int row, int col){
    PieceLabel* p_label = new PieceLabel(boardLabel, row, col, tileSize);
    // sets up the label and sprite //
    p_label->setPixmap(pieceImages[pieceCode]);
    piece_label_board[row][col] = p_label;

    // Connects piece behavior when clicked on //
    connect(piece_label_board[row][col], &PieceLabel::clicked, this, [=](PieceLabel* clicked_label){
        click_piece_action(clicked_label);
    });

    if (pieceCode == "wK"){white_king_label = p_label;}
    else if (pieceCode == "bK") {black_king_label = p_label;}
}
void chess_game::move_piece_label(int p_row, int p_col, int new_row, int new_col){
    piece_label_board[new_row][new_col] = piece_label_board[p_row][p_col];
    piece_label_board[p_row][p_col]->move_piece(new_row, new_col, tileSize);
    piece_label_board[p_row][p_col] = nullptr;
}
void chess_game::capture_piece_label(int row, int col){
    delete piece_label_board[row][col];
    piece_label_board[row][col] = nullptr;
}

//------------------------------------- |Slots| Handles Label Selection  ---------------------------------------//

void chess_game::highlight_tiles(std::vector<std::pair<int,int>> move_list, bool turn){
    if (turn){ // turn if on or off
        for (const auto& move : move_list) {
            Tiles[move.first][move.second]->select();
        }
    }
    else{
        for (const auto& move : move_list) {
            Tiles[move.first][move.second]->deselect();
        }
    }
}
void chess_game::highlight_piece(int row, int col, bool turn){
    if (turn){ // turn if on or off
        piece_label_board[row][col]->select();
    }
    else{
        piece_label_board[row][col]->deselect();
    }

}

void chess_game::check_king_labels(bool white_turn, bool in_check){
    white_king_label->remove_check(); // removes red check background //
    black_king_label->remove_check();

    if (white_turn && in_check){white_king_label->check_king();}
    else if (!white_turn && in_check){black_king_label->check_king();}
}
void chess_game::checkmate_label(bool white_turn){
    bool checkmate = true;
}

//----------------------------------- Handles Label Action -----------------------------------------//

void chess_game::click_piece_action(PieceLabel* clicked_label){
    if (clicked_label != nullptr){
        game_state->click_piece(clicked_label->get_row(), clicked_label->get_col());
    }
}
void chess_game::click_tile_action(ClickableTileLabel* tile){
    if (tile != nullptr){
        game_state->click_tile(tile->get_row(), tile->get_col());
    }
}

void chess_game::setup_board(){
    // Load the sprite sheet for pieces //
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

    QPixmap tile1Pixmap(":/images/dark_tile_textures/dark_1.png");
    QPixmap tile2Pixmap(":/images/light_tile_textures/light_1.png");
    tileLayout = new QGridLayout(boardLabel);
    boardLabel->setLayout(tileLayout);
    tileLayout->setSpacing(0); // spacing between tiles //
    tileLayout->setContentsMargins(0,0,0,0); // margins //

    for (int row = 0; row < 8; ++row) {
        // Creates the 64 clickable tile labels //
        for (int col = 0; col < 8; ++col) {
            Tiles[row][col] = new ClickableTileLabel(row, col, boardLabel);

            int flip = 0;
            if (!isWhite){ flip = 1; }
            if ((col + row) % 2 == flip){ Tiles[row][col]->setPixmap(tile1Pixmap); }
            else { Tiles[row][col]->setPixmap(tile2Pixmap); }

            tileLayout->addWidget(Tiles[row][col], row, col);
            Tiles[row][col]->show();

            connect(Tiles[row][col], &ClickableTileLabel::TileClicked, this, [=](ClickableTileLabel* tile){
                // Connects behavior when empty tile clicked on //
                click_tile_action(tile);
            });
        }
    }
    // Creates the game logic handler //
    game_state = std::make_unique<board_state>(isWhite, isOnline, false);

    connect(game_state.get(), &board_state::make_piece_label, this, &chess_game::make_piece_label);
    connect(game_state.get(), &board_state::move_piece_label, this, &chess_game::move_piece_label);
    connect(game_state.get(), &board_state::capture_piece_label, this, &chess_game::capture_piece_label);

    connect(game_state.get(), &board_state::highlight_tiles, this, &chess_game::highlight_tiles);
    connect(game_state.get(), &board_state::highlight_piece, this, &chess_game::highlight_piece);

    connect(game_state.get(), &board_state::check_king_labels, this, &chess_game::check_king_labels);
    connect(game_state.get(), &board_state::checkmate_label, this, &chess_game::checkmate_label);

    connect(game_state.get(), &board_state::send_player_move, this, &chess_game::send_player_move);
    connect(game_state.get(), &board_state::clientMessage, this, &chess_game::clientMessage);

    game_state->setup_board();

}

//----------------------------------- Handles Server Requests  -----------------------------------------//

void chess_game::send_player_move(QString move, bool isWhite){ emit player_move(move, isWhite); }
void chess_game::receive_move(QString move){ game_state->receive_move(move); }

//----------------------------------- Class Defaults  -----------------------------------------//

chess_game::chess_game(QLabel* boardLabel, bool isWhite, int time = 0, int increment = 0, bool isOnline = false, QObject* parent)
    : boardLabel(boardLabel), isWhite(isWhite), time(time), increment(increment), isOnline(isOnline), QObject(parent){
    setup_board(); // sets up starting board position //
}

chess_game::~chess_game() {
    for (int row = 0; row < 8; ++row) { // deletes all 64 tiles of labels + pieceObjects and tiles //
        for (int col = 0; col < 8; ++col) {
            delete Tiles[row][col];
            delete piece_label_board[row][col];
        }
    }
}
