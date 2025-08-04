#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(bool isServer, QWidget *parent) : QMainWindow(parent) , ui(new Ui::MainWindow){
    ui->setupUi(this);
    if (isServer == false){
        game = new chess_game(ui->labelBoard, 10, 1);
        Client c;
    }
    else{
        Server s;
    }
}

MainWindow::~MainWindow(){
    delete ui;
    delete game;
}


