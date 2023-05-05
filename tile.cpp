#include "tile.h"

Tile::Tile(QWidget* parent)
    : QPushButton{parent}
{
    setAttribute(Qt::WA_Hover);
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

void Tile::enterEvent(QEnterEvent *e)
{
    Q_UNUSED(e)
    emit tileHovered();
}

void Tile::leaveEvent(QEvent *e)
{
    Q_UNUSED(e)
    emit tileUnhovered();
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

    QString style;
    if(inCrosshair)
    {
        style.append("border: 1px solid #F00;");
    }

    if(!revealed)
    {
        style.append("background-color: #CCC; color: #F00;");
        setStyleSheet(style);

        if(hasFlag)
        {
            setText("🚩");
        }

        return;
    }

    if(hasMine)
    {
        style.append("background-color: #F00; color: #000;");
        setStyleSheet(style);
        setText("💣");

        return;
    }

    //setFlat(true);

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
    style.append("font-weight: bold;"
                  "font-size: 16px;"
                  "background-color: #EEE;"
                  "color: " + clr + ";");

    if(!inCrosshair)
    {
        style.append("border-top: 1px solid #DDD;"
                     "border-left: 1px solid #DDD;"
                     "border-bottom: 1px solid #FFF;"
                     "border-right: 1px solid #FFF;");

    }

    setStyleSheet(style);

    if(adjNum > 0)
        setText(QString::number(adjNum));

    if(hasFlag)
    {
        hasFlag = false;
    }
}
