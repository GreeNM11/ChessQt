#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPixmap>
#include <QLabel>
#include <QMap>

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

    // Map piece codes to their respective sliced images
    QMap<QString, QPixmap> pieceImages;
    // White pieces (top row, y = 0)
    pieceImages["wK"] = sprite.copy(0 * (pieceWidth + 4), 0, pieceWidth, pieceHeight);
    pieceImages["wQ"] = sprite.copy(1 * (pieceWidth + 4), 0, pieceWidth, pieceHeight);
    pieceImages["wB"] = sprite.copy(2 * (pieceWidth + 4), 0, pieceWidth, pieceHeight);
    pieceImages["wN"] = sprite.copy(3 * (pieceWidth + 4), 0, pieceWidth, pieceHeight);
    pieceImages["wR"] = sprite.copy(4 * (pieceWidth + 4), 0, pieceWidth, pieceHeight);
    pieceImages["wP"] = sprite.copy(5 * (pieceWidth + 3.5), 0, pieceWidth, pieceHeight);

    // Black pieces (bottom row, y = pieceHeight)
    pieceImages["bK"] = sprite.copy(0 * (pieceWidth + 4), pieceHeight + 7, pieceWidth, pieceHeight);
    pieceImages["bQ"] = sprite.copy(1 * (pieceWidth + 4), pieceHeight + 7, pieceWidth, pieceHeight);
    pieceImages["bB"] = sprite.copy(2 * (pieceWidth + 4), pieceHeight + 7, pieceWidth, pieceHeight);
    pieceImages["bN"] = sprite.copy(3 * (pieceWidth + 4), pieceHeight + 7, pieceWidth, pieceHeight);
    pieceImages["bR"] = sprite.copy(4 * (pieceWidth + 4), pieceHeight + 7, pieceWidth, pieceHeight);
    pieceImages["bP"] = sprite.copy(5 * (pieceWidth + 3.5), pieceHeight + 7, pieceWidth, pieceHeight);


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

    // Create piece QLabel children on top of the board QLabel
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            QString pieceCode = board[row][col];
            if (!pieceCode.isEmpty()) {
                QLabel *pieceLabel = new QLabel(ui->labelBoard);
                pieceLabel->setPixmap(pieceImages[pieceCode]);
                //pieceLabel->setScaledContents(true);
                pieceLabel->setFixedSize(tileSize, tileSize);
                pieceLabel->move(col * tileSize, row * tileSize);
                pieceLabel->show();
            }
        }
    }

}

MainWindow::~MainWindow()
{
    delete ui;
}


