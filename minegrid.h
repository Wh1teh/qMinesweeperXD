#ifndef MINEGRID_H
#define MINEGRID_H

#include "tile.h"

#include <QDebug>
#include <QWidget>
#include <QRandomGenerator>
#include <QPushButton>
#include <QScreen>
#include <QMouseEvent>

namespace Ui {
class MineGrid;
}

class MineGrid : public QWidget
{
    Q_OBJECT

public:
    explicit MineGrid(QWidget *parent = nullptr);
    ~MineGrid();

    //flags
    bool crosshair = false;

    int minesAmount = 0;
    int gridSize = 0;

    //pointers
    //QList<QList<QPushButton*>> buttons;
    QList<QList<Tile*>> tiles;
    QList<Tile*> mines;

    //functions
    void createGrid(int gridSize, int minesAmount);
    void freezeButtons();


private:
    Ui::MineGrid *ui;

signals:
    void gameStarted();
    void gridUpdated(int flagsAmount);
    void victory();
    void defeat();

    void skipToEnd();

public slots:
    void revealRandomMine();
    void revealOneGreenTile();

private slots:
    void buttonClicked();
    void buttonRightClicked();
    void buttonHovered();
    void buttonUnhovered();

private:
    //flags
    //bool firstReveal = false;
    int tilesRevealed = 0;
    int flagsAmount = 0;

    //functions
    void debugGrid();

    void putMines(int x, int y);
    void putAdjNums();
    void createButtons();

    void revealTile(int x, int y);
    void floodFill(int x, int y);

    void revealAround(Tile*);
};

#endif // MINEGRID_H
