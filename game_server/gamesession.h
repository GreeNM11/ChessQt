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

    void player_join(ClientWrap* player);

    bool validate_players();
    void validate_move(bool isWhite, QString move);

    void sendPlayerMessage(QString playerName, QString msg);
    void sendErrorMessage(QString msg);

    ClientWrap* getPlayer1(){ return player1; }
    ClientWrap* getPlayer2(){ return player2; }

private:
    QString gameID;
    bool isWhite; // if player1 is white //
    ClientWrap* player1 = nullptr;
    ClientWrap* player2 = nullptr;

    std::unique_ptr<board_state> server_game;
    QString moveList;

    void check_checkmated();
    QString flip_move(QString move);
    QString flip_moveList(QString moveList);

signals:

};

#endif // GAMESESSION_H
