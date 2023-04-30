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

    int minesAmount = 0;
    int gridSize = 0;

    //pointers
    //QList<QList<QPushButton*>> buttons;
    QList<QList<Tile*>> tiles;
    QList<Tile*> mines;

    //functions
    void createGrid(int gridSize, int minesAmount);


private:
    Ui::MineGrid *ui;

signals:
    void gridUpdated(int flagsAmount);
    void victory();
    void defeat();

public slots:
    void revealRandomMine();

private slots:
    void buttonClicked();
    void buttonRightClicked();

private:
    //flags
    bool firstReveal = false;
    int tilesRevealed = 0;
    int flagsAmount = 0;

    //functions
    void debugGrid();

    void putMines(int x, int y);
    void putAdjNums();
    void createButtons();

    void reposMine(Tile* tile);

    void revealTile(int x, int y);
    void floodFill(int x, int y);
};

#endif // MINEGRID_H
