#ifndef TILE_H
#define TILE_H

#include <QPushButton>
#include <QMouseEvent>

class Tile : public QPushButton
{
    Q_OBJECT
public:
    explicit Tile(QWidget* parent = nullptr);

    //overrides
    void mousePressEvent(QMouseEvent *e);

    //flags
    bool hasMine = false;
    bool hasFlag = false;
    bool revealed = false;
    bool floodChecked = false;

    //data
    int adjNum = 0;
    int coords[2] = {};

    //functions

signals:
    void rightClicked();

};

#endif // TILE_H
