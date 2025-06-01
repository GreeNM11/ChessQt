#include "mainwindow.h"
#include "chess_pieces.cpp"


Piece* make_piece(int col, int row, QString pieceCode, QLabel* pieceLabel){
    bool w = true;
    if (pieceCode.at(0) == "b"){
        w = false;
    }

    Piece* p;

    if (pieceCode.at(1) == "K"){
        p = new King(w, col, row, pieceLabel);
    }
    else if (pieceCode.at(1) == "Q"){
        p = new Queen(w, col, row, pieceLabel);
    }
    else if (pieceCode.at(1) == "R"){
        p = new Rook(w, col, row, pieceLabel);
    }
    else if (pieceCode.at(1) == "B"){
        p = new Bishop(w, col, row, pieceLabel);
    }
    else if (pieceCode.at(1) == "N"){
        p = new Knight(w, col, row, pieceLabel);
    }
    else if (pieceCode.at(1) == "P"){
        p = new Pawn(w, col, row, pieceLabel);
    }

    return p;
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
    const int pieceWidth = 80;
    const float pieceHeight = 80;
    const int tileSize = 80;

    QMap<QString, QPixmap> pieceImages; // holds all png files for pieces

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

    // Starting chess position
    QString board[8][8] = {
        { "bR", "bN", "bB", "bQ", "bK", "bB", "bN", "bR" },
        { "bP", "bP", "bP", "bP", "bP", "bP", "bP", "bP" },
        { "",   "",   "",   "",   "",   "",   "",   "" },
        { "",   "",   "",   "",   "",   "",   "",   "" },
        { "",   "",   "",   "",   "",   "",   "",   "" },
        { "",   "",   "",   "",   "",   "",   "",   "" },
        { "wP", "wP", "wP", "wP", "wP", "wP", "wP", "wP" },
        { "wR", "wN", "wB", "wQ", "wK", "wB", "wN", "wR" }
    };

    Piece* object_board[8][8]; // chess board but for piece objects

    // creating the pieces objects at starting positions
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            QString pieceCode = board[row][col];
            if (pieceCode != ""){
                QLabel* pieceLabel = new QLabel(ui->labelBoard);
                pieceLabel->setPixmap(pieceImages[pieceCode]);
                pieceLabel->setScaledContents(true);
                pieceLabel->setFixedSize(tileSize, tileSize);
                pieceLabel->show();

                object_board[col][row] = make_piece(col, row, pieceCode, pieceLabel);
                object_board[col][row]->move_piece(col, row, tileSize);
            }
        }
    }
}



MainWindow::~MainWindow()
{
    delete ui;
}


