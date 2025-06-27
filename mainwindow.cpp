#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) , ui(new Ui::MainWindow){
    ui->setupUi(this);
    game = new chess_game(ui, 10, 1);
}

MainWindow::~MainWindow(){
    delete ui;
    delete game;
}


