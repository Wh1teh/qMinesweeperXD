#include "tile.h"

Tile::Tile(QWidget* parent)
    : QPushButton{parent}
{

}

void Tile::mousePressEvent(QMouseEvent *e)
{
    qDebug() << Q_FUNC_INFO << "disabled:" << disabled;

    if(disabled == true)
    {
        emit skipToEnd();
        return;
    }

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

void Tile::revealTile()
{
    qDebug() << Q_FUNC_INFO;

    revealed = true;

    updateText();
}

void Tile::updateText()
{
    qDebug() << Q_FUNC_INFO;

    setText("");

    if(!revealed)
    {
        setStyleSheet("background-color: #CCC; color: #F00");

        if(hasFlag)
        {
            setText("🚩");
        }

        return;
    }

    if(hasMine)
    {
        setStyleSheet("background-color: #F00; color: #000;");
        setText("💣");

        return;
    }

    setFlat(true);

    QString clr;
    switch (adjNum) {
    case 0:
        clr.append("#000");
        break;

    case 1:
        clr.append("#00F");
        break;

    case 2:
        clr.append("#080");
        break;

    case 3:
        clr.append("#F00");
        break;

    case 4:
        clr.append("#0ff");
        break;

    case 5:
        clr.append("#800");
        break;

    case 6:
        clr.append("#088");
        break;

    case 7:
        clr.append("#000");
        break;

    case 8:
        clr.append("#888");
        break;

    default:
        break;
    }
    setStyleSheet("background-color: #eee; color: " + clr);

    if(adjNum > 0)
        setText(QString::number(adjNum));
}
