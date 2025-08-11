#ifndef clickable_tile_label_H
#define clickable_tile_label_H

#include <QLabel>
#include <QMouseEvent>
#include <QPainter>

class ClickableTileLabel : public QLabel
{
    Q_OBJECT

public:
    explicit ClickableTileLabel(int row, int col, QWidget *parent = nullptr);

    int row() const { return row_; }
    int col() const { return col_; }

    void select();
    void deselect();

signals:
    void TileClicked(ClickableTileLabel* tile);

protected:
    void mousePressEvent(QMouseEvent *event) override;

private:
    int row_;
    int col_;

    QColor overlayColor;
    void setOverlayColor(const QColor& color);
    void paintEvent(QPaintEvent* event) override;
};

#endif // CLICKABLE_Tile_LABEL_H

