#include "gamesession.h"
#include <QDebug>
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
void GameSession::validate_move(bool white_move, QString move){
    QString ErrorMessage = "";

    if (!validate_players()){ return; qDebug() << "3";}
    else {
        QString w_move = move;
        if (!white_move){ w_move = flip_move(move); }
        if (!server_game){ return; }
        int returnCode = server_game->validate_move(w_move);

        if (returnCode == 0){
            qDebug() << "1";
            server_game->server_move(w_move);
            qDebug() << "2";
            player1->sendMove_S(w_move);
            player2->sendMove_S(flip_move(w_move));
            qDebug() << "3";
            check_checkmated();
            return;
        }
        else if (returnCode == 1){
            ErrorMessage = "❌Received move out of bounds | " + move;
        }
        else if (returnCode == 2){
            ErrorMessage = "❌Received move no piece found: " + w_move;
        }
        else if (returnCode == 3){
            ErrorMessage = "❌Received move not players turn";
        }
        else if (returnCode == 4){
            ErrorMessage = "❌Received move not legal";
        }
        else if (returnCode == -1){
            ErrorMessage = "❌Unknown move error: Could not accept move";
        }
        sendErrorMessage(ErrorMessage);
    }
    qDebug() << "done";
}
void GameSession::check_checkmated(){
    if (!validate_players()){ return; }
    int code = server_game->validate_checkmated();
    qDebug() << "9";
    if ( code != 0){
        player1->sendCheckmated_S(QString::number(code));
        player2->sendCheckmated_S(QString::number(code));
        qDebug() << "10a";
    }
}

QString GameSession::flip_move(QString move){
    int from_row = move.at(0).digitValue();
    int from_col = move.at(1).digitValue();
    int to_row = move.at(2).digitValue();
    int to_col = move.at(3).digitValue();

    int flipInt = (7-from_row) * 1000 + (7-from_col) * 100 + (7-to_row) * 10 + (7-to_col);
    move = QString::number(flipInt);
    if (flipInt < 1000){ move = "0" + move; } // no 0 in front of int //
    return move;
}

void GameSession::sendPlayerMessage(QString playerName, QString msg){
    // sends to both players so message only shows if received by server //
    if (player1 != nullptr){ player1->sendPlayerMessage_S(playerName, msg); }
    if (player2 != nullptr){ player2->sendPlayerMessage_S(playerName, msg); }
}
void GameSession::sendErrorMessage(QString msg){
    // sends off the error code to each player //
    if (player1 != nullptr){ player1->sendErrorMessage_S(msg); }
    if (player2 != nullptr){ player2->sendErrorMessage_S(msg); }
}
//---------------------------------// Class Defaults //---------------------------------//

GameSession::GameSession(QString gameID, ClientWrap* player1, bool isWhite)
    : gameID(gameID), player1(player1), isWhite(isWhite) {
    server_game = std::make_unique<board_state>(true, false); // Server runs its own version of the game //
    server_game->setup_board();
}

GameSession::~GameSession(){

}
