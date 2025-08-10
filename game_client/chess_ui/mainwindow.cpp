#include "mainwindow.h"
#include "ui_mainwindow.h"

///-------------------------------------- Client Functions  --------------------------------------///
void MainWindow::ClientMessage(const QString msg){
    ui->boardChat->append(msg);  // adds message and a newline //

    QTextCursor cursor = ui->ServerLog->textCursor(); // scrolls //
    cursor.movePosition(QTextCursor::End);
    ui->ServerLog->setTextCursor(cursor);
}

void MainWindow::SingleplayerClicked() {
    ui->mainStack->setCurrentWidget(ui->boardPage);

    isWhite = true;
    game = std::make_unique<chess_game>(ui->labelBoard, isWhite, 10, 1);
}
void MainWindow::hostGameClicked() {
    QString color = isWhite ? "White" : "Black";
    ClientMessage("Hosting Game.. You are: " + color);
    ui->mainStack->setCurrentWidget(ui->boardPage); // sets ui //

    isWhite = true; // will asks for it later

    game = std::make_unique<chess_game>(ui->labelBoard, isWhite, 10, 1); // creates game + loads board //
    connect(game.get(), &chess_game::player_move, this, &MainWindow::onPlayerMove);
    connect(game.get(), &chess_game::clientMessage, this, &MainWindow::ClientMessage);

    client->createGameSession(isWhite); // sends to client < clientWrap < Server to make a game session //
}
void MainWindow::joinGameClicked() {
    ui->mainStack->setCurrentWidget(ui->joinGamePage); // sets ui //

    connect(ui->joinGameEnter, &QLineEdit::returnPressed, this, [this]() {
        gameID = ui->joinGameEnter->text();
        ui->joinGameEnter->clear();
        client->joinGameSession(gameID);
    });
}

void MainWindow::onClientConnected() {
    ui->ClientStatus->clear();
    ui->ClientStatus->append("<span style='color:green;'>Server Status: Connected</span>");
}
void MainWindow::onClientDisconnected() {
    ui->ClientStatus->clear();
    ui->ClientStatus->append("<span style='color:red;'>Server Status: Disconnected</span>");
}

void MainWindow::onCreateGameSession(QString ID){
    gameID = ID;
    ClientMessage("Created Game Session. ID: " + gameID);
}
void MainWindow::onJoinGameSession(bool joined, bool isWhite_S){
    if (joined){
        QString color = isWhite_S ? "White" : "Black";
        ClientMessage("Joining Game.. You are " + color);
        isWhite = isWhite_S;
        ui->mainStack->setCurrentWidget(ui->boardPage); // sets ui //

        game = std::make_unique<chess_game>(ui->labelBoard, isWhite, 10, 1); // creates game + loads board //
        connect(game.get(), &chess_game::player_move, this, &MainWindow::onPlayerMove);
        connect(game.get(), &chess_game::clientMessage, this, &MainWindow::ClientMessage);
    }
    else{
        ClientMessage("Retry Join Code");
    }
}

void MainWindow::onReceiveMove(QString move){ game->receiveMove(move); }
///------------------------------------- MainWindow Slots  -------------------------------------///

void MainWindow::onPlayerMove(const QString move, const bool isWhite){ client->sendMove(gameID, move, isWhite); }

///-------------------------------------- Server UI --------------------------------------///

void MainWindow::ServerMessage(const QString message) {
    ui->ServerLog->append(message);  // adds message and a newline //

    QTextCursor cursor = ui->ServerLog->textCursor(); // scrolls //
    cursor.movePosition(QTextCursor::End);
    ui->ServerLog->setTextCursor(cursor);
}

///-------------------------------------- Main Window --------------------------------------///
MainWindow::MainWindow(bool isServer, QWidget *parent) : QMainWindow(parent) , ui(new Ui::ChessQt){
    ui->setupUi(this);

    /////////////////////   Client   /////////////////////
    if (isServer == false){
        client = std::make_unique<Client>(this);

        // UI & Buttons Setup //
        ui->mainStack->setCurrentWidget(ui->menuPage);
        connect(ui->SingleplayerButton, &QPushButton::clicked, this, &MainWindow::SingleplayerClicked);
        connect(ui->HostGameButton, &QPushButton::clicked, this, &MainWindow::hostGameClicked);
        connect(ui->JoinGameButton, &QPushButton::clicked, this, &MainWindow::joinGameClicked);

        // Client Network Setup //
        connect(client.get(), &Client::connectedToServer, this, &MainWindow::onClientConnected);
        connect(client.get(), &Client::clientMessage, this, &MainWindow::ClientMessage);

        connect(client.get(), &Client::createGameSession_S, this, &MainWindow::onCreateGameSession);
        connect(client.get(), &Client::joinGameSession_S, this, &MainWindow::onJoinGameSession);

        connect(client.get(), &Client::sendMove_S, this, &MainWindow::onReceiveMove);

    }
    //////////////////////   Server   /////////////////////
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



