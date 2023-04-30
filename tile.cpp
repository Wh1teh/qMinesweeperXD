#include "tile.h"

Tile::Tile(QWidget* parent)
    : QPushButton{parent}
{

}

void Tile::mousePressEvent(QMouseEvent *e)
{
    qDebug() << Q_FUNC_INFO;

    if(e->isInputEvent())
    {
        if(e->button() == Qt::LeftButton)
        {
            emit clicked();
        }
        if(e->button() == Qt::RightButton)
        {
            emit rightClicked();
        }
    }
}
