#include "piece_label.h"

PieceLabel::PieceLabel(QWidget* parent, Piece* p_object, int col, int row)
    : QLabel(parent), p_object(p_object), col(col), row(row) {
    setStyleSheet("");
}

void PieceLabel::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        emit clicked(this);
        selected = true;
        setStyleSheet("background-color: rgba(100, 100, 100, 200);");
    }
    //QLabel::mousePressEvent(event);
}

void PieceLabel::deselect() {
    if (selected) {
        setStyleSheet("");
        selected = false;
    }
}

bool PieceLabel::move_piece(int r, int c, int tileSize){
    if (1 == 1){ //make if r and c = moveset[r][c]
        move(c * tileSize, r * tileSize);
        row = r;
        col = c;

        return true;
    }
    else{
        return false;
    }
}

Piece* PieceLabel::get_object(){
    return p_object;
}

int PieceLabel::get_row(){return row;}
int PieceLabel::get_col(){return col;}
