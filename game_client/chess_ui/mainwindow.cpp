#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QDebug"
#include <QTimer>

///-------------------------------------- Button Actions  --------------------------------------///
void MainWindow::registerButtonClicked(){
    QString user = ui->usernameEnter->text();
    QString pass = ui->passwordEnter->text();
    ui->usernameEnter->clear();
    ui->passwordEnter->clear();

    if(!client->registerUser(user, pass)){
        onErrorMessage_C("Could not connect to register user");
    }
    else{
        // load menu page
    }
}
void MainWindow::loginButtonClicked(){
    QString user = ui->usernameEnter->text();
    QString pass = ui->passwordEnter->text();
    ui->usernameEnter->clear();
    ui->passwordEnter->clear();

    if (!client->loginUser(user, pass)){
        onErrorMessage_C("Could not connect to login user");
    }
    else{
        // load menu page
    }
}

void MainWindow::singleplayerClicked() {
    QString color = isWhite ? "White" : "Black";
    ClientMessage("Offline Game Created. You are: " + color);

    createGamePage(isWhite, false, false);
}

void MainWindow::playVsAIClicked() {
    // Always make human white and AI black for consistency
    isWhite = true;
    ClientMessage("AI Game Created. You are White vs AI (Black)");
    ClientMessage("You move first. AI will respond after your move.");

    createGamePage(true, false, true); // Human is white, AI is black
}
void MainWindow::hostGameClicked() {
    // sends to client < clientWrap < Server to make a game session //
    if (!client->createGameSession(isWhite)){
        onErrorMessage_C(" Could not connect to host game");
    }
    else{
        QString color = isWhite ? "White" : "Black";
        ClientMessage("Hosting Game.. You are: " + color);
        playerName = color;

        createGamePage(isWhite, true, false);
    }
}
void MainWindow::joinGameClicked() {
    ui->mainStack->setCurrentWidget(ui->joinGamePage); // sets ui //
    connect(ui->joinGameEnter, &QLineEdit::returnPressed, this, [this]() {
        gameID = ui->joinGameEnter->text();
        if (gameID != ""){
            ui->joinGameEnter->clear();
            if(!client->joinGameSession(gameID)){
                onErrorMessage_C("Could not connect to join game");
            }
        }
    });
}
void MainWindow::backMenuClicked(){
    qDebug() << "Back menu clicked - starting cleanup";
    
    // Clean up AI bot first to prevent crashes
    if (aiBot) {
        qDebug() << "Cleaning up AI bot";
        try {
            // Stop the AI bot properly
            aiBot->stopGame();
            
            // Disconnect all AI bot signals to prevent crashes
            disconnect(aiBot.get(), nullptr, this, nullptr);
            
            // Reset the AI bot
            aiBot.reset();
            aiBot = nullptr;
            qDebug() << "AI bot cleaned up successfully";
        } catch (...) {
            qDebug() << "Error cleaning up AI bot";
            aiBot.reset();
            aiBot = nullptr;
        }
    }
    
    // Clean up game first (this might be using labelBoard)
    if (game) {
        qDebug() << "Cleaning up game";
        try {
            // Disconnect all game signals first
            disconnect(game.get(), nullptr, this, nullptr);
            game.reset();
            game = nullptr;
            qDebug() << "Game cleaned up successfully";
        } catch (...) {
            qDebug() << "Error cleaning up game";
            game.reset();
            game = nullptr;
        }
    }
    
    // Clean up UI - Don't manually delete labelBoard as it's managed by Qt's parent-child system
    if (labelBoard) {
        qDebug() << "Resetting labelBoard pointer (Qt will handle cleanup)";
        labelBoard = nullptr;
        qDebug() << "labelBoard pointer reset successfully";
    }
    
    // Reset state
    qDebug() << "Resetting state and switching to menu";
    ui->mainStack->setCurrentWidget(ui->menuPage);
    gameID = "";
    isWhite = true;
    isOnline = false;
    canChat = true;
    isAIGame = false;
    
    qDebug() << "Back menu cleanup completed";
}

///-------------------------------------- Client Functions  --------------------------------------///
void MainWindow::ClientMessage(const QString msg){
    ui->boardChat->append(msg);  // adds message and a newline //

    QTextCursor cursor = ui->boardChat->textCursor(); // scrolls //
    cursor.movePosition(QTextCursor::End);
    ui->boardChat->setTextCursor(cursor);
}

void MainWindow::onClientConnected_C() { ClientMessage("✅Server Status: Connected"); }
void MainWindow::onClientDisconnected_C() { ClientMessage("❌Server Status: Disconnected"); }

void MainWindow::onRegisterUser_C(QString code){
    if (code.at(0) == "✅"){
        ui->mainStack->setCurrentWidget(ui->menuPage);
    }
    ClientMessage(code);
}
void MainWindow::onLoginUser_C(QString code){
    if (code.at(0) == "✅"){
        ui->mainStack->setCurrentWidget(ui->menuPage);
    }
    ClientMessage(code);
}

void MainWindow::onCreateGameSession_C(QString ID){
    gameID = ID;
    isOnline = true;
    ClientMessage("Created Game Session. ID: " + gameID);
}
void MainWindow::onJoinGameSession_C(bool joined, bool w, int code, QString moveList){
    if (joined){
        QString color = w ? "White" : "Black";
        createGamePage(w, true);
        playerName = color;
        isOnline = true;

        if (code == 0){ client->sendPlayerMessage(gameID, playerName," has joined"); }
        else if (code == 1){
            client->sendPlayerMessage(gameID, playerName," has rejoined");
            game->receive_moveList(moveList);
        }

        ClientMessage("You are " + color);
    }
    else if (code == 0){
        ClientMessage("❌Unable to Join: Invalid Join Code");
    }
    else if (code == 1){
        ClientMessage("❌Unable to Join: Game is Full");
    }
    else {
        ClientMessage("❌Unable to Join: Unknown Error");
    }
}

void MainWindow::onReceiveMove_C(QString move){ game->receive_move(move); }
void MainWindow::onReceiveCheckmated_C(int code){
    if (code == 1){ ClientMessage("Black won: White Checkmated"); }
    else if (code == 2){ ClientMessage("White won: Black Checkmated"); }

    else if (code == 3){ ClientMessage("Draw: White Stalemated"); }
    else if (code == 4){ ClientMessage("Draw: Black Stalemated"); }

    else if (code == 5){ ClientMessage("Black won: White Flagged"); }
    else if (code == 6){ ClientMessage("White won: Black Flagged"); }

    else if (code == 7){ ClientMessage("Black Won: White Resigned"); }
    else if (code == 8){ ClientMessage("White: Black Resigned"); }
}

//-----  Helper Functions ------//
void MainWindow::createGamePage(bool w, bool isOnline, bool isAI){
    isWhite = w;
    isAIGame = isAI;
    ui->mainStack->setCurrentWidget(ui->boardPage); // sets ui //

    labelBoard = new QLabel(ui->boardPage);                    // example text
    labelBoard->setGeometry(0, 0, 640, 640);
    labelBoard->show();

    game = std::make_unique<chess_game>(labelBoard, isWhite, 10, 1, isOnline, isAI); // creates game + loads board //
    connect(game.get(), &chess_game::player_move, this, &MainWindow::onPlayerMove_G);
    connect(game.get(), &chess_game::player_checkmated, this, &MainWindow::onCheckmated_G);
    connect(game.get(), &chess_game::clientMessage, this, &MainWindow::ClientMessage);

    // Initialize AI Bot if this is an AI game
    if (isAI) {
        // Create simple AI bot
        aiBot = std::make_unique<SimpleAIBot>(this);
        
        // Connect AI signals
        connect(aiBot.get(), &SimpleAIBot::aiMoveReady, this, &MainWindow::onAIMove);
        connect(aiBot.get(), &SimpleAIBot::aiThinking, this, &MainWindow::onAIThinking);
        connect(aiBot.get(), &SimpleAIBot::aiMessage, this, &MainWindow::onAIMessage);
        
        // Configure AI
        aiBot->setDifficulty(aiDifficulty);
        aiBot->setThinkingTime(aiThinkingTime);
        aiBot->enableAI(true);
        aiBot->startGame();
        
        ClientMessage("AI Bot initialized. Difficulty: " + QString::number(aiDifficulty));
        qDebug() << "AI Bot created and configured - Difficulty:" << aiDifficulty << "Thinking time:" << aiThinkingTime;
    }

    connect(ui->gameChatEnter, &QLineEdit::returnPressed, this, [this, isOnline]() {
        QString playerMsg = ui->gameChatEnter->text();
        if (playerMsg != "" && canChat){
            ui->gameChatEnter->clear();
            if (isOnline) {
                client->sendPlayerMessage(gameID, playerName, playerMsg);
            } else {
                ClientMessage("You: " + playerMsg);
            }
            canChat = false;
            QTimer::singleShot(3000, this, [this]() { canChat = true; }); // timer for 3 seconds
        }
    });
    ui->gameChatEnter->setMaxLength(150); // max 150 characters per player message //
}

///--------------------------- MainWindow Slots from  GameLogic  ---------------------------///

void MainWindow::onPlayerMove_G(const QString move, const bool isWhite){ 
    qDebug() << "=== onPlayerMove_G called ===";
    qDebug() << "Move:" << move << "isWhite:" << isWhite << "isAIGame:" << isAIGame;
    qDebug() << "isOnline:" << isOnline << "aiBot available:" << (aiBot != nullptr);
    
    if (isOnline) {
        qDebug() << "Online game - sending move to server";
        client->sendMove(gameID, move, isWhite); 
    } else if (isAIGame) {
        qDebug() << "AI game detected";
        // In AI game, human is white, AI is black
        if (isWhite) {
            // Human (white) made a move, now it's AI's turn
            ClientMessage("Your move: " + move);
            qDebug() << "Human (white) moved, triggering AI (black)";
            
            if (aiBot) {
                qDebug() << "Calling AI bot onHumanMove with move:" << move;
                try {
                    aiBot->onHumanMove(move);
                    qDebug() << "AI bot onHumanMove called successfully";
                } catch (...) {
                    qDebug() << "Error calling AI bot onHumanMove";
                    ClientMessage("AI bot error occurred");
                }
            } else {
                qDebug() << "ERROR: AI bot not available!";
                ClientMessage("ERROR: AI bot not available!");
            }
        } else {
            // This shouldn't happen in AI game since human is always white
            qDebug() << "Unexpected: AI game but isWhite is false";
        }
    } else {
        qDebug() << "Offline game (not AI)";
    }
    qDebug() << "=== onPlayerMove_G completed ===";
}
void MainWindow::onCheckmated_G(int c){
    if (!isOnline){ onReceiveCheckmated_C(c); }
}

// AI Bot Slots
void MainWindow::onAIMove(QString move) {
    qDebug() << "Received AI (black) move:" << move << "Format: 4-digit (row/col, row/col)";
    try {
        ClientMessage("AI (Black) move: " + move);
        if (game) {
            qDebug() << "Executing AI move through game->receive_move()";
            // Execute AI move through the game as black player
            game->receive_move(move);
            qDebug() << "AI move executed successfully";
        } else {
            qDebug() << "Game object is null, cannot execute AI move";
        }
    } catch (...) {
        qDebug() << "Error handling AI (black) move";
        ClientMessage("Error executing AI (black) move");
    }
}

void MainWindow::onAIThinking(bool thinking) {
    if (thinking) {
        ui->aiStatusLabel->setText("AI (Black): Thinking...");
        ui->aiStatusLabel->setStyleSheet("QLabel { background-color: #ffeb3b; border: 1px solid #f57f17; border-radius: 5px; padding: 5px; }");
    } else {
        ui->aiStatusLabel->setText("AI (Black): Ready");
        ui->aiStatusLabel->setStyleSheet("QLabel { background-color: #4caf50; border: 1px solid #2e7d32; border-radius: 5px; padding: 5px; }");
    }
}

void MainWindow::onAIMessage(QString message) {
    ClientMessage("AI: " + message);
}

void MainWindow::onAIDifficultyChanged(int value) {
    aiDifficulty = value;
    ui->aiDifficultyValue->setText(QString::number(value));
    if (aiBot) {
        aiBot->setDifficulty(value);
    }
}

void MainWindow::onAIThinkingTimeChanged(int value) {
    aiThinkingTime = value;
    if (aiBot) {
        aiBot->setThinkingTime(value);
    }
}

void MainWindow::onPlayerMessage_C(QString playerName, QString msg){ ClientMessage(playerName + ": " + msg); }
void MainWindow::onErrorMessage_C(QString msg){ ClientMessage("❌ServerError ~ " + msg); }

///-------------------------------------- Server UI --------------------------------------///

void MainWindow::ServerMessage(const QString message) {
    ui->ServerLog->append(message);  // adds message and a newline //
    qDebug() << message;

    QTextCursor cursor = ui->ServerLog->textCursor(); // scrolls //
    cursor.movePosition(QTextCursor::End);
    ui->ServerLog->setTextCursor(cursor);
}

///-------------------------------------- Main Window --------------------------------------///
MainWindow::MainWindow(bool isServer, bool isRemote, QWidget *parent)
    : QMainWindow(parent) , ui(new Ui::ChessQt){
    ui->setupUi(this);

    /////////////////////   Client   /////////////////////
    if (!isServer){
        if (isRemote){ client = std::make_unique<Client>(this, remoteIP, port); }
        else         { client = std::make_unique<Client>(this, localIP, port); }

        ui->mainStack->setCurrentWidget(ui->loginPage);

        // UI & Buttons Setup //
        connect(ui->registerButton, &QPushButton::clicked, this, &MainWindow::registerButtonClicked);
        connect(ui->loginButton, &QPushButton::clicked, this, &MainWindow::loginButtonClicked);

        connect(ui->SingleplayerButton, &QPushButton::clicked, this, &MainWindow::singleplayerClicked);
        connect(ui->HostGameButton, &QPushButton::clicked, this, &MainWindow::hostGameClicked);
        connect(ui->JoinGameButton, &QPushButton::clicked, this, &MainWindow::joinGameClicked);
        connect(ui->PlayVsAIButton, &QPushButton::clicked, this, &MainWindow::playVsAIClicked);
        connect(ui->backMenuButton, &QPushButton::clicked, this, &MainWindow::backMenuClicked);
        
        // AI Settings connections
        connect(ui->aiDifficultySlider, &QSlider::valueChanged, this, &MainWindow::onAIDifficultyChanged);
        connect(ui->aiThinkingTimeSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::onAIThinkingTimeChanged);

        // Client-Server Network Setup //
        connect(client.get(), &Client::connectedToServer, this, &MainWindow::onClientConnected_C);
        connect(client.get(), &Client::disconnectedToServer, this, &MainWindow::onClientDisconnected_C);
        connect(client.get(), &Client::clientMessage, this, &MainWindow::ClientMessage);

        connect(client.get(), &Client::registerUser_S, this, &MainWindow::onRegisterUser_C);
        connect(client.get(), &Client::loginUser_S, this, &MainWindow::onLoginUser_C);

        connect(client.get(), &Client::createGameSession_S, this, &MainWindow::onCreateGameSession_C);
        connect(client.get(), &Client::joinGameSession_S, this, &MainWindow::onJoinGameSession_C);

        connect(client.get(), &Client::sendMove_S, this, &MainWindow::onReceiveMove_C);
        connect(client.get(), &Client::sendCheckmated_S, this, &MainWindow::onReceiveCheckmated_C);

        connect(client.get(), &Client::sendPlayerMessage_S, this, &MainWindow::onPlayerMessage_C);
        connect(client.get(), &Client::sendErrorMessage_S, this, &MainWindow::onErrorMessage_C);

        //client->serverStatus();
    }
    //////////////////////   Server   /////////////////////
    else{
        ui->mainStack->setCurrentWidget(ui->serverPage);
        ui->boardChatFrame->hide();
        ui->boardChat->hide();

        server = std::make_unique<Server>(this, port);

        connect(server.get(), &Server::newMessage, this, &MainWindow::ServerMessage);
        server->emitServerStatus();
        server->createDB();
    }
}

MainWindow::~MainWindow(){
    delete ui;
}



