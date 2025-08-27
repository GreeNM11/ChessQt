#include "mainwindow.h"
#include "ui_mainwindow.h"

///-------------------------------------- Button Actions  --------------------------------------///
void MainWindow::registerButtonClicked(){
    QString user = ui->usernameEnter->text();
    QString pass = ui->passwordEnter->text();
    ui->usernameEnter->clear();
    ui->passwordEnter->clear();

    client->registerUser(user, pass);
}
void MainWindow::loginButtonClicked(){
    QString user = ui->usernameEnter->text();
    QString pass = ui->passwordEnter->text();
    ui->usernameEnter->clear();
    ui->passwordEnter->clear();

    client->loginUser(user, pass);
}

void MainWindow::singleplayerClicked() {
    QString color = isWhite ? "White" : "Black";
    ClientMessage("Singleplayer Game Created. You are: " + color);

    isWhite = true;
    createGamePage(isWhite, false);
}
void MainWindow::hostGameClicked() {
    QString color = isWhite ? "White" : "Black";
    ClientMessage("Hosting Game.. You are: " + color);
    playerName = color;

    isWhite = true;
    createGamePage(isWhite, true);

    client->createGameSession(isWhite); // sends to client < clientWrap < Server to make a game session //
}
void MainWindow::joinGameClicked() {
    ui->mainStack->setCurrentWidget(ui->joinGamePage); // sets ui //

}

///-------------------------------------- Client Functions  --------------------------------------///
void MainWindow::ClientMessage(const QString msg){
    ui->boardChat->append(msg);  // adds message and a newline //

    QTextCursor cursor = ui->ServerLog->textCursor(); // scrolls //
    cursor.movePosition(QTextCursor::End);
    ui->ServerLog->setTextCursor(cursor);
}

void MainWindow::onClientConnected() {
    ui->ClientStatus->clear();
    ui->ClientStatus->append("<span style='color:green;'>Server Status: Connected</span>");
}
void MainWindow::onClientDisconnected() {
    ui->ClientStatus->clear();
    ui->ClientStatus->append("<span style='color:red;'>Server Status: Disconnected</span>");
}

void MainWindow::onRegisterUser(QString code){
    if (code.at(0) == "✅"){
        ui->mainStack->setCurrentWidget(ui->menuPage);
    }
    ClientMessage(code);
}
void MainWindow::onLoginUser(QString code){
    if (code.at(0) == "✅"){
        ui->mainStack->setCurrentWidget(ui->menuPage);
    }
    ClientMessage(code);
}

void MainWindow::onCreateGameSession(QString ID){
    gameID = ID;
    ClientMessage("Created Game Session. ID: " + gameID);
}
void MainWindow::onJoinGameSession(bool joined, bool w){
    if (joined){
        QString color = w ? "White" : "Black";
        createGamePage(w, true);
        playerName = color;
        client->sendPlayerMessage(gameID, playerName," has joined");
        ClientMessage("You are " + color);
    }
    else{
        ClientMessage("Retry Join Code");
    }
}

void MainWindow::onReceiveMove(QString move){ game->receive_move(move); }

    //-----  Helper Functions ------//
void MainWindow::createGamePage(bool w, bool isOnline){
    isWhite = w;
    ui->mainStack->setCurrentWidget(ui->boardPage); // sets ui //

    game = std::make_unique<chess_game>(ui->labelBoard, isWhite, 10, 1, isOnline); // creates game + loads board //
    connect(game.get(), &chess_game::player_move, this, &MainWindow::onPlayerMove);
    connect(game.get(), &chess_game::clientMessage, this, &MainWindow::ClientMessage);
    connect(ui->gameChatEnter, &QLineEdit::returnPressed, this, [this]() {
        QString playerMsg = ui->gameChatEnter->text();
        ui->gameChatEnter->clear();
        client->sendPlayerMessage(gameID, playerName, playerMsg);
    });
    ui->gameChatEnter->setMaxLength(150); // max 150 characters per player message //
}

///------------------------------------- MainWindow Slots  -------------------------------------///

void MainWindow::onPlayerMove(const QString move, const bool isWhite){ client->sendMove(gameID, move, isWhite); }
void MainWindow::onPlayerMessage(QString playerName, QString msg){ ClientMessage(playerName + ": " + msg); }
void MainWindow::onErrorMessage(QString msg){ ClientMessage("ServerCode ~ " + msg); }

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

        ui->mainStack->setCurrentWidget(ui->loginPage);

        // UI & Buttons Setup //
        connect(ui->registerButton, &QPushButton::clicked, this, &MainWindow::registerButtonClicked);
        connect(ui->loginButton, &QPushButton::clicked, this, &MainWindow::loginButtonClicked);

        connect(ui->SingleplayerButton, &QPushButton::clicked, this, &MainWindow::singleplayerClicked);
        connect(ui->HostGameButton, &QPushButton::clicked, this, &MainWindow::hostGameClicked);
        connect(ui->JoinGameButton, &QPushButton::clicked, this, &MainWindow::joinGameClicked);

        connect(ui->joinGameEnter, &QLineEdit::returnPressed, this, [this]() {
            gameID = ui->joinGameEnter->text();
            ui->joinGameEnter->clear();
            client->joinGameSession(gameID);
        });

        // Client-Server Network Setup //
        connect(client.get(), &Client::connectedToServer, this, &MainWindow::onClientConnected);
        connect(client.get(), &Client::clientMessage, this, &MainWindow::ClientMessage);

        connect(client.get(), &Client::registerUser_S, this, &MainWindow::onRegisterUser);
        connect(client.get(), &Client::loginUser_S, this, &MainWindow::onLoginUser);

        connect(client.get(), &Client::createGameSession_S, this, &MainWindow::onCreateGameSession);
        connect(client.get(), &Client::joinGameSession_S, this, &MainWindow::onJoinGameSession);

        connect(client.get(), &Client::sendMove_S, this, &MainWindow::onReceiveMove);
        connect(client.get(), &Client::sendPlayerMessage_S, this, &MainWindow::onPlayerMessage);
        connect(client.get(), &Client::sendErrorMessage_S, this, &MainWindow::onErrorMessage);

    }
    //////////////////////   Server   /////////////////////
    else{
        ui->mainStack->setCurrentWidget(ui->serverPage);
        ui->boardChatFrame->hide();
        ui->boardChat->hide();

        server = std::make_unique<Server>(this);

        connect(server.get(), &Server::newMessage, this, &MainWindow::ServerMessage);
        server->emitServerStatus();
        server->createDB();
    }
}

MainWindow::~MainWindow(){
    delete ui;
}



