#ifndef GAMESESSION_H
#define GAMESESSION_H

#include <QObject>
#include "game_client/chess_logic/board_state.h"
#include "clientwrap.h"

class GameSession
{
    Q_OBJECT
public:
    GameSession(QString gameID, ClientWrap* player1, bool isWhite);
    ~GameSession();

    void validate_move(bool isWhite, QString move);

private:

    QString gameID;
    bool isWhite; // if player1 is white //
    ClientWrap* player1 = nullptr;
    ClientWrap* player2 = nullptr;

signals:

};

#endif // GAMESESSION_H
