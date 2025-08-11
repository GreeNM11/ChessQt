#include "clickable_tile_label.h"

ClickableTileLabel::ClickableTileLabel(int row, int col, QWidget *parent)
    : QLabel(parent), row_(row), col_(col) {
    setMouseTracking(true);
    setOverlayColor(QColor(0, 0, 0, 0));
}

void ClickableTileLabel::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        emit TileClicked(this);
    }
    QLabel::mousePressEvent(event);
}

void ClickableTileLabel::select(){ setOverlayColor(QColor(100, 100, 100, 100)); }
void ClickableTileLabel::deselect(){ setOverlayColor(QColor(0, 0, 0, 0)); }

void ClickableTileLabel::setOverlayColor(const QColor& color) {
    overlayColor = color;
    update();
}

void ClickableTileLabel::paintEvent(QPaintEvent* event) {
    QLabel::paintEvent(event);  // draw pixmap first

    if (overlayColor.alpha() > 0) {
        QPainter painter(this);
        painter.fillRect(rect(), overlayColor);
    }
}

