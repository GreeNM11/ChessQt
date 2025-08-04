#include "clickable_tile_label.h"

ClickableTileLabel::ClickableTileLabel(int row, int col, QWidget *parent)
    : QLabel(parent), row_(row), col_(col) {
    setMouseTracking(true);
}

void ClickableTileLabel::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        emit TileClicked(this);
    }
    QLabel::mousePressEvent(event);
}

void ClickableTileLabel::select(){
    setStyleSheet("background-color: rgba(100, 100, 100, 150);");
}
void ClickableTileLabel::deselect(){
    setStyleSheet("");
}
