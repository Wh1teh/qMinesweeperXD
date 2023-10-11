#ifndef TILE_H
#define TILE_H

#include <QDebug>
#include <QPushButton>
#include <QMouseEvent>
#include <QHoverEvent>
#include <QEvent>

class Tile : public QPushButton
{
    Q_OBJECT
public:
    explicit Tile(QWidget* parent = nullptr);

    //overrides
    void mousePressEvent(QMouseEvent *e);
    void enterEvent(QEnterEvent *e);
    void leaveEvent(QEvent *e);

    //flags
    bool hasMine = false;
    bool hasFlag = false;
    bool revealed = false;
    bool floodChecked = false;
    bool disabled = false;
    bool inCrosshair = false;

    //data
    int adjNum = 0;
    int coords[2] = {}; //row, col

    //functions
    void revealTile();
    void updateText();
    void putFlag();

signals:
    void rightClicked();
    void skipToEnd();
    void tileHovered();
    void tileUnhovered();
};

#endif // TILE_H
