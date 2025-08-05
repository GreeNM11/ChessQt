#include "mainwindow.h"
#include "ui_mainwindow.h"

///-------------------------------------- Client  --------------------------------------///
void MainWindow::SingleplayerClicked() {
    ui->mainStack->setCurrentWidget(ui->boardPage);
    game = std::make_unique<chess_game>(ui->labelBoard, 10, 1);
}

void MainWindow::MultiplayerClicked() {
    ui->mainStack->setCurrentWidget(ui->boardPage);
    game = std::make_unique<chess_game>(ui->labelBoard, 10, 1);
    if (client){
        connect(game.get(), &chess_game::player_move, client.get(), &Client::sendMove);
    }
}
void MainWindow::onClientConnected() {
    ui->ClientStatus->clear();
    ui->ClientStatus->append("<span style='color:green;'>Server Status: Connected</span>");
    // You can also change page or enable buttons here
}
void MainWindow::onClientDisconnected() {
    ui->ClientStatus->clear();
    ui->ClientStatus->append("<span style='color:red;'>Server Status: Disconnected</span>");
    // You can also change page or enable buttons here
}
///-------------------------------------- Server --------------------------------------///
void MainWindow::ServerMessage(const QString &message) {
    ui->ServerLog->append(message);  // adds message and a newline

    // Auto-scroll to the bottom
    QTextCursor cursor = ui->ServerLog->textCursor();
    cursor.movePosition(QTextCursor::End);
    ui->ServerLog->setTextCursor(cursor);
}

///-------------------------------------- Main Window --------------------------------------///
MainWindow::MainWindow(bool isServer, QWidget *parent) : QMainWindow(parent) , ui(new Ui::ChessQt){
    ui->setupUi(this);
    if (isServer == false){
        ui->mainStack->setCurrentWidget(ui->menuPage);
        client = std::make_unique<Client>(this);

        connect(ui->SingleplayerButton, &QPushButton::clicked, this, &MainWindow::SingleplayerClicked);
        connect(client.get(), &Client::connectedToServer, this, &MainWindow::onClientConnected);
    }
    else{
        ui->mainStack->setCurrentWidget(ui->serverPage);
        server = std::make_unique<Server>(this);

        connect(server.get(), &Server::newMessage, this, &MainWindow::ServerMessage);
        server->emitServerStatus();
    }
}

MainWindow::~MainWindow(){
    delete ui;
}



