#include "piece_label.h"

PieceLabel::PieceLabel(QWidget* parent, int row, int col, int tileSize)
    : QLabel(parent), row(row), col(col) {
    setStyleSheet("");
    setScaledContents(true);
    setFixedSize(tileSize, tileSize);
    show();
    move_piece(row, col, tileSize);
}

PieceLabel::~PieceLabel(){}

void PieceLabel::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        emit clicked(this);
    }
}

void PieceLabel::select(){
    selected = true;
    setStyleSheet("background-color: rgba(100, 100, 100, 150);");
}
void PieceLabel::deselect() {
    setStyleSheet("");
    if (in_check){
        check_king();
    }
    selected = false;
}

void PieceLabel::set_capturable(){
    setStyleSheet("background-color: rgba(80, 80, 80, 150);");
}
void PieceLabel::move_piece(int r, int c, int tileSize){
    move(c * tileSize, r * tileSize); // Move flips col and row //
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

int PieceLabel::get_row(){return row;}
int PieceLabel::get_col(){return col;}





