#ifndef PIECE_LABEL_H
#define PIECE_LABEL_H

#include <QLabel>
#include <QMouseEvent>
#include "chess_pieces.cpp"

class PieceLabel : public QLabel {
    Q_OBJECT

public:
    explicit PieceLabel(QWidget* parent = nullptr, Piece* p_object = nullptr, int row = 0, int col = 0);
    void deselect();
    void set_capturable();
    bool move_piece(int row, int col, int tileSize);

    Piece* get_object();
    int get_col();
    int get_row();

protected:
    void mousePressEvent(QMouseEvent* event) override;

private:
    Piece* p_object;
    bool selected = false;
    int col;
    int row;

signals:
    void clicked(PieceLabel* clicked_label);
};

#endif
