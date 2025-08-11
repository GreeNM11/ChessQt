#include "piece_label.h"

PieceLabel::PieceLabel(QWidget* parent, Piece* p_object, int col, int row)
    : QLabel(parent), p_object(p_object), col(col), row(row) {
    setStyleSheet("");
}

PieceLabel::~PieceLabel(){
    if (p_object != nullptr){
        delete p_object;
    }
}

void PieceLabel::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        emit clicked(this);
    }
}
void PieceLabel::select(){
    selected = true;
    setStyleSheet("background-color: rgba(100, 100, 100, 200);");
}

void PieceLabel::deselect() {
    setStyleSheet("");
    if (in_check){
        check_king();
    }
    selected = false;
}

void PieceLabel::set_capturable(){
    setStyleSheet("background-color: rgba(80, 80, 80, 120);");
}

void PieceLabel::move_piece(int r, int c, int tileSize){
    move(c * tileSize, r * tileSize);
    row = r;
    col = c;
}

void PieceLabel::check_king(){
    setStyleSheet("background-color: rgba(100, 0, 0, 120);");
    in_check = true;
}
void PieceLabel::remove_check(){
    in_check = false;
    deselect();
}
Piece* PieceLabel::get_object(){
    return p_object;
}

int PieceLabel::get_row(){return row;}
int PieceLabel::get_col(){return col;}

bool PieceLabel::get_color(){return p_object->get_color();}





