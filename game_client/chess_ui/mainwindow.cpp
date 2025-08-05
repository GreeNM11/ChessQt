#include "mainwindow.h"
#include "ui_mainwindow.h"


void MainWindow::onStartGameClicked() {
    ui->mainStack->setCurrentWidget(ui->boardPage);
    game = new chess_game(ui->labelBoard, 10, 1);
}

MainWindow::MainWindow(bool isServer, QWidget *parent) : QMainWindow(parent) , ui(new Ui::ChessQt){
    ui->setupUi(this);
    if (isServer == false){
        Client c;
        connect(ui->startGameButton, &QPushButton::clicked, this, &MainWindow::onStartGameClicked);
    }
    else{
        Server s;
    }
}

MainWindow::~MainWindow(){
    delete ui;
    delete game;
}



