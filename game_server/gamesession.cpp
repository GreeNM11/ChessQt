#include "gamesession.h"


void GameSession::validate_move(bool white_move, QString move){
    QString ErrorMessage = "";
    bool validMove = false;

    if(player1 == nullptr) {
        ErrorMessage = "White player not found";
        player2->sendErrorMessage_S(ErrorMessage, false);
        return;
    }
    else if(player2 == nullptr) {
        ErrorMessage = "Black player not found";
        player1->sendErrorMessage_S(ErrorMessage, true);
        return;
    }
    else {
        QString w_move = move;
        if (!white_move){ w_move = flip_move(move); }

        int returnCode = server_game->validate_move(w_move);

        if (returnCode == 0){
            server_game->server_move(w_move);
            validMove = true;
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

        if (!validMove){
            player1->sendErrorMessage_S(ErrorMessage, white_move); // sends off the error code to each player //
            player2->sendErrorMessage_S(ErrorMessage, white_move);
            return;
        }
        player1->sendMove_S(w_move);
        player2->sendMove_S(flip_move(w_move));
    }
 // black flips white move //
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


//---------------------------------// Class Defaults //---------------------------------//

GameSession::GameSession(QString gameID, ClientWrap* player1, bool isWhite)
    : gameID(gameID), player1(player1), isWhite(isWhite) {
    server_game = std::make_unique<board_state>(true, false, true); // Server runs its own version of the game //
    server_game->setup_board();
}

GameSession::~GameSession(){

}
