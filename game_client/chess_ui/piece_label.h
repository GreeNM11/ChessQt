#ifndef PIECE_LABEL_H
#define PIECE_LABEL_H

#include <QLabel>
#include <QMouseEvent>
#include "game_client/chess_logic/chess_pieces.h"

class PieceLabel : public QLabel {
    Q_OBJECT

public:
    explicit PieceLabel(QWidget* parent = nullptr, Piece* p_object = nullptr, int row = 0, int col = 0);
    ~PieceLabel();

    void select();
    void deselect();
    void set_capturable();
    void move_piece(int row, int col, int tileSize);
    void check_king();
    void remove_check();

    Piece* get_object();
    int get_col();
    int get_row();
    bool get_color();

protected:
    void mousePressEvent(QMouseEvent* event) override;

private:
    Piece* p_object;
    bool selected = false;
    int col;
    int row;
    bool in_check = false;

signals:
    void clicked(PieceLabel* clicked_label);
};

#endif
