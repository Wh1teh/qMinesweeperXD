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
    bool disabled = false;

    //data
    int adjNum = 0;
    int coords[2] = {};

    //functions
    void revealTile();
    void updateText();
    void putFlag();

signals:
    void rightClicked();
    void skipToEnd();

};

#endif // TILE_H
