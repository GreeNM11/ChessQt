#include "mainwindow.h"

Piece* MainWindow::make_piece(QString pieceCode){

    bool w = true;
    if (pieceCode.at(0) == "b"){
        w = false;
    }

    Piece* p;

    if (pieceCode.at(1) == "K"){
        p = new King(w);
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
PieceLabel* MainWindow::make_piece_label(int row, int col, QString pieceCode, int tileSize, QMap<QString, QPixmap> pieceImages){
    Piece* piece_object = make_piece(pieceCode);
    PieceLabel* p_label = new PieceLabel(ui->labelBoard, piece_object, row, col);

    p_label->setPixmap(pieceImages[pieceCode]);
    p_label->setScaledContents(true);
    p_label->setFixedSize(tileSize, tileSize);
    p_label->show();
    p_label->move_piece(row, col, tileSize);
    piece_label_board[row][col] = p_label;

    // Connects piece behavior when clicked on //

    connect(piece_label_board[row][col], &PieceLabel::clicked, this, [=](PieceLabel* clicked_label){
        bool same = (clicked_label == s_label);

        if (s_label == nullptr){
            // if null, set selected piece and moveset //
            s_label = clicked_label;
            s_move_list = s_label->get_object()->get_moveset(s_label->get_row(), s_label->get_col(), board);
            for (const auto& move : s_move_list) {Tiles[move.first][move.second]->select();}
        }
        else if (!same && s_label->get_color() == clicked_label->get_color()){
            // deselected old piece and selected new piece and moveset //
            s_label->deselect();
            for (const auto& move : s_move_list) {Tiles[move.first][move.second]->deselect();}
            s_label = clicked_label;

            s_move_list = s_label->get_object()->get_moveset(s_label->get_row(), s_label->get_col(), board);
            for (const auto& move : s_move_list) {Tiles[move.first][move.second]->select();}
        }
        else{
            bool capture_piece = false;
            int r;
            int c;
            for (const auto& move : s_move_list) {
                // checks if selected piece is to capture //
                if (clicked_label == piece_label_board[move.first][move.second]){
                    capture_piece = true; break;
                }
            }
            if (capture_piece){
                // captures piece and moves it //
                for (const auto& move : s_move_list) {
                    Tiles[move.first][move.second]->deselect();
                }
                delete clicked_label;
            }
        }
    });
    return p_label;
}

void move_piece(){

}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Load the board image from resources and set it on labelBoard
    QPixmap boardPixmap(":/images/board.png");
    ui->labelBoard->setFixedSize(640, 640);
    ui->labelBoard->setPixmap(boardPixmap);
    ui->labelBoard->setFixedSize(boardPixmap.size());  // Fix size to board size

    // Load the sprite sheet for pieces
    QPixmap sprite(":/images/pieces.png");

    QMap<QString, QPixmap> pieceImages; // holds sprites

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

    QString default_board[8][8] = {  // Starting chess position //
        { "bR", "bN", "bB", "bQ", "bK", "bB", "bN", "bR" },
        { "bP", "bP", "bP", "bP", "bP", "bP", "bP", "bP" },
        { "",   "",   "",   "",   "",   "",   "",   "" },
        { "",   "",   "",   "",   "",   "",   "",   "" },
        { "",   "",   "",   "",   "",   "",   "",   "" },
        { "",   "",   "",   "",   "",   "",   "",   "" },
        { "wP", "wP", "wP", "wP", "wP", "wP", "wP", "wP" },
        { "wR", "wN", "wB", "wQ", "wK", "wB", "wN", "wR" }
    };
    memcpy(board, default_board, sizeof(board));


    for (int row = 0; row < 8; ++row) {

            // Creates the 64 clickable tile labels //

        for (int col = 0; col < 8; ++col) {
            Tiles[row][col] = new ClickableTileLabel(row, col, ui->labelBoard);
            Tiles[row][col]->setGeometry(col * tileSize, row * tileSize, tileSize, tileSize);
            Tiles[row][col]->setStyleSheet("");
            Tiles[row][col]->show();

            // Connects behavior when empty tile clicked on //

            connect(Tiles[row][col], &ClickableTileLabel::TileClicked, this, [=](ClickableTileLabel* tile){
                if (s_label != nullptr){
                    for (const auto& move : s_move_list) {
                        if (tile == Tiles[move.first][move.second]){
                            int p_col = s_label->get_col();
                            int p_row = s_label->get_row();

                            s_label->move_piece(move.first, move.second, tileSize);
                            s_label->get_object()->has_moved();
                            piece_label_board[move.first][move.second] = piece_label_board[p_row][p_col];
                            piece_label_board[p_row][p_col] = nullptr;
                            board[move.first][move.second] = board[p_row][p_col];
                            board[p_row][p_col] = "";
                        }
                        Tiles[move.first][move.second]->deselect();
                    }
                    s_label->deselect();
                    s_label = nullptr;
                }
            });
        }
    }

    for (int col = 0; col < 8; ++col) {
        // Creates the 32 pieces objects at starting positions //
        for (int row = 0; row < 8; ++row) {
            QString pieceCode = board[row][col];
            if (pieceCode != ""){
                make_piece_label(row, col, pieceCode, tileSize, pieceImages);
            }
        }
    }
}

MainWindow::~MainWindow()
{
    delete ui;
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            delete Tiles[row][col];
            delete piece_label_board[row][col];
        }
    }
}


