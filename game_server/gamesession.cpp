#include "gamesession.h"
#include <QDebug>

//-----------------------------// Player Management //---------------------------------//

void GameSession::player_join(ClientWrap* player){
    if (!player2){
        player2 = player;
        player->joinGameSession_S(true, !isWhite, 0, ""); // gameFound | p2 isWhite parameters //
    }
    else if (player1->getUser() == player->getUser()){
        if (!isWhite){ player->joinGameSession_S(true, isWhite, 1, flip_moveList(moveList)); }
        else{ player->joinGameSession_S(true, isWhite, 1, moveList); }
        player1 = player;
    }
    else if (player2->getUser() == player->getUser()){
        if (isWhite){ player->joinGameSession_S(true, !isWhite, 1, flip_moveList(moveList)); }
        else{ player->joinGameSession_S(true, !isWhite, 1, moveList); }
        player2 = player;
    }
    else{
        player->joinGameSession_S(false, false, 1, "");
    }
}

//-----------------------------// Game Validation //---------------------------------//

bool GameSession::validate_players(){
    if (!player1 && !player2){ return false; }
    if(!player1){
        player2->sendErrorMessage_S("❌White Player Missing");
        return false;
    }
    else if(!player2) {
        player1->sendErrorMessage_S("❌Black Player Missing");
        return false;
    }
    return true;
}
void GameSession::validate_move(bool white_move, const QString &move){
    QString ErrorMessage = "";
    if (move.size() != 4){ sendErrorMessage("❌Received empty move"); }
    if (!validate_players() || !server_game){ return; }

    QString w_move = move;
    if (!white_move){ w_move = flip_move(move); }
    int returnCode = server_game->validate_move(w_move);

    if (returnCode == 0){
        server_game->server_move(w_move);
        player1->sendMove_S(w_move);
        player2->sendMove_S(flip_move(w_move));
        moveList += w_move;
        check_checkmated();
        return;
    }
    else if (returnCode == 1){
        ErrorMessage = "Received move: out of bounds: " + w_move;
    }
    else if (returnCode == 2){
        ErrorMessage = "Received move: no piece found: " + w_move;
    }
    else if (returnCode == 3){
        ErrorMessage = "Received move: not players turn";
    }
    else if (returnCode == 4){
        ErrorMessage = "Received move: not legal";
    }
    else if (returnCode == 5){
        ErrorMessage = "Received move: missing coordinate: " + w_move;
    }
    else if (returnCode == -1){
        ErrorMessage = "Received move: Unknown Error";
    }
    sendErrorMessage(ErrorMessage);

}
void GameSession::check_checkmated(){
    if (!validate_players()){ return; }
    int code = server_game->validate_checkmated();
    if ( code != 0){
        player1->sendCheckmated_S(QString::number(code));
        player2->sendCheckmated_S(QString::number(code));
    }
}

QString GameSession::flip_move(const QString &move){
    QString m = move;
    int from_row = m.at(0).digitValue();
    int from_col = m.at(1).digitValue();
    int to_row = m.at(2).digitValue();
    int to_col = m.at(3).digitValue();

    int flipInt = (7-from_row) * 1000 + (7-from_col) * 100 + (7-to_row) * 10 + (7-to_col);
    m = QString::number(flipInt);
    while (m.size() < 4){ m = "0" + m; } // no 0 in front of int //
    return m;
}
QString GameSession::flip_moveList(const QString &moveList){
    QString flippedList = "";
    QString ml = moveList;

    while (ml.size() >= 4){
        flippedList += flip_move(ml.left(4));
        ml = ml.mid(4);
    }

    return flippedList;
}

//-----------------------------// Player Communication //---------------------------------//

void GameSession::sendPlayerMessage(const QString &playerName, const QString &msg){
    // sends to both players so message only shows if received by server //
    if (player1 != nullptr){ player1->sendPlayerMessage_S(playerName, msg); }
    if (player2 != nullptr){ player2->sendPlayerMessage_S(playerName, msg); }
}
void GameSession::sendErrorMessage(const QString &msg){
    // sends off the error code to each player //
    if (player1 != nullptr){ player1->sendErrorMessage_S(msg); }
    if (player2 != nullptr){ player2->sendErrorMessage_S(msg); }
}

//-----------------------------// Class Defaults //---------------------------------//

GameSession::GameSession(const QString &gameID, ClientWrap* player1, bool isWhite)
    : gameID(gameID), player1(player1), isWhite(isWhite) {
    server_game = std::make_unique<board_state>(true, false); // Server runs its own version of the game //
    server_game->setup_board();
}

GameSession::~GameSession(){

}
