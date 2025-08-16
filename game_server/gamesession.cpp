#include "gamesession.h"


void GameSession::validate_move(bool white_move, QString move){
    int from_row = move.at(0).digitValue();
    int from_col = move.at(1).digitValue();
    int to_row = move.at(2).digitValue();
    int to_col = move.at(3).digitValue();

    bool validMove = false;
    QString flipMove;
    // Validates Move //
    if ((from_row >= 0 && from_row <= 7) && (from_col >= 0 && from_col <= 7) &&
        (to_row >= 0 && to_row <= 7) && (to_col >= 0 && to_col <= 7)) {
        // need to also check with security for legal moves //
        validMove = true;
        // '7 -' to flip coordinates since opponent has flipped board //
        int flipInt = (7-from_row) * 1000 + (7-from_col) * 100 + (7-to_row) * 10 + (7-to_col);
        flipMove = QString::number(flipInt);
        if (flipInt < 1000){ move = "0" + move; } // no 0 in front of int //
    }

    QString ErrorMessage = "";
    if (!validMove){
        if (isWhite){ ErrorMessage = "Whites move was out of bounds"; }
        else { ErrorMessage = "Blacks move was out of bounds"; }
        player1->sendErrorMessage_S(ErrorMessage);
        player2->sendErrorMessage_S(ErrorMessage);
        return;
    }
    else if(player1 == nullptr) {
        ErrorMessage = "White player not found";
        player2->sendErrorMessage_S(ErrorMessage);
        return;
    }
    else if(player2 == nullptr) {
        ErrorMessage = "Black player not found";
        player1->sendErrorMessage_S(ErrorMessage);
        return;
    }

    // If doesn't Error //
    if (isWhite){
        player1->sendMove_S(move);
        player2->sendMove_S(flipMove); // black flips white move //
    }
    else if (!isWhite){
        player1->sendMove_S(flipMove); // white flips black move //
        player2->sendMove_S(move);
    }
}


//---------------------------------// Class Defaults //---------------------------------//

GameSession::GameSession(QString gameID, ClientWrap* player1, bool isWhite)
    : gameID(gameID), player1(player1), isWhite(isWhite) {}

GameSession::~GameSession(){}
