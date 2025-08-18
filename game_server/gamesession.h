#ifndef GAMESESSION_H
#define GAMESESSION_H

#include <QObject>
#include "game_client/chess_logic/board_state.h"
#include "clientwrap.h"

class GameSession : public QObject {
    Q_OBJECT
public:
    GameSession(QString gameID, ClientWrap* player1, bool isWhite);
    ~GameSession();

    void validate_move(bool isWhite, QString move);
    QString flip_move(QString move);

    void setPlayer2(ClientWrap* player){ player2 = player; }

    bool player1_color(){ return isWhite; }
    bool player2_color(){ return !isWhite; }

    ClientWrap* getPlayer1(){ return player1; }
    ClientWrap* getPlayer2(){ return player2; }

private:
    QString gameID;
    bool isWhite; // if player1 is white //
    ClientWrap* player1 = nullptr;
    ClientWrap* player2 = nullptr;

    std::unique_ptr<board_state> server_game;

signals:

};

#endif // GAMESESSION_H
