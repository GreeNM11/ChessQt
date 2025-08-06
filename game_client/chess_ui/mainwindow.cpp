#include "mainwindow.h"
#include "ui_mainwindow.h"

///-------------------------------------- Client UI  --------------------------------------///
void MainWindow::SingleplayerClicked() {
    ui->mainStack->setCurrentWidget(ui->boardPage);
    game = std::make_unique<chess_game>(ui->labelBoard, 10, 1);
}

void MainWindow::MultiplayerClicked() {
    ui->mainStack->setCurrentWidget(ui->boardPage);
    game = std::make_unique<chess_game>(ui->labelBoard, 10, 1);
    connect(game.get(), &chess_game::player_move, this, &MainWindow::onPlayerMove);
    client->createGameSession(true);
}
void MainWindow::onClientConnected() {
    ui->ClientStatus->clear();
    ui->ClientStatus->append("<span style='color:green;'>Server Status: Connected</span>");
}
void MainWindow::onClientDisconnected() {
    ui->ClientStatus->clear();
    ui->ClientStatus->append("<span style='color:red;'>Server Status: Disconnected</span>");
}

void MainWindow::receiveOpponent(Client* o){ opponent = o; }
void MainWindow::onInvalidJoinCode(){ qDebug() << "invalid"; }

// Receives from chess_game, now sends to client //
void MainWindow::onPlayerMove(const QString &move, const bool isWhite){ client->sendMove(move, isWhite); }

///-------------------------------------- Server UI --------------------------------------///

void MainWindow::ServerMessage(const QString &message) {
    ui->ServerLog->append(message);  // adds message and a newline //

    QTextCursor cursor = ui->ServerLog->textCursor(); // scrolls //
    cursor.movePosition(QTextCursor::End);
    ui->ServerLog->setTextCursor(cursor);
}

///-------------------------------------- Main Window --------------------------------------///
MainWindow::MainWindow(bool isServer, QWidget *parent) : QMainWindow(parent) , ui(new Ui::ChessQt){
    ui->setupUi(this);
    if (isServer == false){ // Start Client //
        client = std::make_unique<Client>(this);

        // UI Network Setup //
        ui->mainStack->setCurrentWidget(ui->menuPage);
        connect(ui->SingleplayerButton, &QPushButton::clicked, this, &MainWindow::SingleplayerClicked);
        connect(ui->MultiplayerButton, &QPushButton::clicked, this, &MainWindow::MultiplayerClicked);

        // Client Network Setup //
        connect(client.get(), &Client::connectedToServer, this, &MainWindow::onClientConnected);
        connect(client.get(), &Client::sendOpponent, this, &MainWindow::receiveOpponent);
        connect(client.get(), &Client::sendInvalidJoinCode, this, &MainWindow::onInvalidJoinCode);

    }
    else{ // Start Server //
        ui->mainStack->setCurrentWidget(ui->serverPage);
        server = std::make_unique<Server>(this);

        connect(server.get(), &Server::newMessage, this, &MainWindow::ServerMessage);
        server->emitServerStatus();
    }
}

MainWindow::~MainWindow(){
    delete ui;
}



