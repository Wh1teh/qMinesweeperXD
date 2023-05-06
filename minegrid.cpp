#include "minegrid.h"
#include "ui_minegrid.h"

MineGrid::MineGrid(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MineGrid)
{
    ui->setupUi(this);
}

MineGrid::~MineGrid()
{
    delete ui;
}

void MineGrid::createGrid(int gridSize, int minesAmount)
{
    qDebug() << Q_FUNC_INFO << "start";

    this->gridSize = gridSize;
    this->minesAmount = minesAmount;

    createButtons();

    qDebug() << Q_FUNC_INFO << "end";
}

void MineGrid::freezeButtons()
{
    qDebug() << Q_FUNC_INFO;
    for (int i = 0; i < gridSize; ++i) {
        for (int j = 0; j < gridSize; ++j) {
            tiles[i][j]->disabled = true;
        }
    }
}

void MineGrid::revealRandomMine()
{
    qDebug() << Q_FUNC_INFO;

    for (int i = 0; i < mines.count(); ++i) {
        if(mines[i]->revealed)
            continue;
        else
        {
            mines[i]->revealed = true;

            mines[i]->setStyleSheet("background-color: #F40; color: #000;");
            mines[i]->setText("💣");

            break;
        }
    }
}

void MineGrid::revealOneGreenTile()
{
    static int i = 0;
    static int j = 0;

    if(i >= gridSize)
    {
        i = 0;
        return;
    }

    tiles[i][j]->setFlat(false);
    if(tiles[i][j]->revealed)
    {
        tiles[i][j]->setStyleSheet("background-color: #AFA; color: #AFA");
    }
    else
    {
        tiles[i][j]->setStyleSheet("background-color: #0F0; color: #0F0");
    }

    j++;
    if(j >= gridSize)
    {
        j = 0;
        i++;
    }
}

void MineGrid::buttonClicked()
{
    qDebug() << Q_FUNC_INFO << QObject::sender();

    Tile * tile = qobject_cast<Tile*>(QObject::sender());

    emit gameStarted();

    revealTile(tile);
}

void MineGrid::buttonRightClicked()
{
    qDebug() << Q_FUNC_INFO << QObject::sender();

    Tile * tile = qobject_cast<Tile*>(QObject::sender());

    //don't do anything to revealed
    if(tile->revealed)
        return;

    if(tile->hasFlag)
    {
        tile->hasFlag = false;
        flagsAmount--;
    }
    else
    {
        tile->hasFlag = true;
        flagsAmount++;
    }

    tile->updateText();

    emit gridUpdated(flagsAmount);
}

void MineGrid::buttonHovered()
{
    if(!crosshair)
        return;

    Tile * tile = qobject_cast<Tile*>(QObject::sender());

    iterate3x3(tile, &MineGrid::renderCrosshair);
}

void MineGrid::buttonUnhovered() //maybe unite with hover slot
{
    if(!crosshair)
        return;

    Tile * tile = qobject_cast<Tile*>(QObject::sender());

    iterate3x3(tile, &MineGrid::unrenderCrosshair);
}

void MineGrid::renderCrosshair(Tile * tile)
{
    tile->inCrosshair = true;
    tile->updateText();
}

void MineGrid::unrenderCrosshair(Tile * tile)
{
    tile->inCrosshair = false;
    tile->updateText();
}

void MineGrid::putMines(Tile * tile)
{
    qDebug() << Q_FUNC_INFO;

    mines.clear();

    int x = tile->coords[1];
    int y = tile->coords[0];

    int ranX = 0;
    int ranY = 0;
    int minesPlaced = 0;
    while(minesPlaced < minesAmount)
    {
        ranX = QRandomGenerator::global()->bounded(gridSize);
        ranY = QRandomGenerator::global()->bounded(gridSize);

        //check that mine placement wouldn't be inside clicked region
        if((ranX >= x - 1 && x + 1 >= ranX) && (ranY >= y - 1 && y + 1 >= ranY))
            continue;

        //check that tile doesn't have a mine already
        if(!tiles[ranY][ranX]->hasMine)
        {
            tiles[ranY][ranX]->hasMine = true;
            minesPlaced++;

            //put mines to list
            mines.append(tiles[ranY][ranX]);
        }
    }
}

void MineGrid::createAdjNums(Tile * tile)
{
    qDebug() << Q_FUNC_INFO;

    //raise adj score around mine
    if(tile->hasMine)
    {
        iterate3x3(tile, &MineGrid::incAdjNum);
    }
}

void MineGrid::incAdjNum(Tile * tile)
{
    tile->adjNum++;
}

void MineGrid::createButtons()
{
    qDebug() << Q_FUNC_INFO;

    tiles.clear();

    QList<Tile*> row;
    for (int i = 0; i < gridSize; ++i) {
        row.clear();
        for (int j = 0; j < gridSize; ++j) {
            row.append(new Tile(this));
            row[j]->coords[0] = i; row[j]->coords[1] = j;

            connect(row[j], SIGNAL(rightClicked()),
                    this, SLOT(buttonRightClicked()));

            connect(row[j], SIGNAL(clicked()),
                    this, SLOT(buttonClicked()));

            connect(row[j], SIGNAL(skipToEnd()),
                    this, SIGNAL(skipToEnd()));

            connect(row[j], SIGNAL(tileHovered()),
                    this, SLOT(buttonHovered()));
            connect(row[j], SIGNAL(tileUnhovered()),
                    this, SLOT(buttonUnhovered()));
        }
        tiles.append(row);

        for (int j = 0; j < gridSize; ++j) {
            ui->loGrid->addWidget(tiles[i][j], i, j);

            //set button sizes and styles
            float screenH = QGuiApplication::primaryScreen()->geometry().height();
            int rectangle = (screenH * 0.8) /
                    (32 / (float)gridSize) / (float)gridSize;

            tiles[i][j]->setMinimumSize(rectangle, rectangle);
            tiles[i][j]->setMaximumSize(rectangle, rectangle);

            tiles[i][j]->setStyleSheet("background-color: #ccc;");
        }
    }
}

void MineGrid::revealTile(Tile * tile)
{
    qDebug() << Q_FUNC_INFO;

    if(tile->hasFlag)
    {
        return;
    }

    //prevent first click from triggering mine and init adjNums
    if(tilesRevealed == 0)
    {
        putMines(tile);

        iterateGrid(&MineGrid::createAdjNums);
    }

    if(tile->hasMine)
    {
        qDebug() << Q_FUNC_INFO << "mine!";

        tile->revealTile();

        emit defeat();
        return;
    }

    if(!tile->revealed)
    {
        tile->revealTile();
        tilesRevealed++;
        qDebug() << Q_FUNC_INFO << "tilesRevealed:" << tilesRevealed;

        iterateGrid(&MineGrid::floodFill);
    }
    else
    {
        iterate3x3(tile, &MineGrid::revealAround);
    }

    //game is won
    if(tilesRevealed + minesAmount == gridSize * gridSize)
        emit victory();
}

void MineGrid::floodFill(Tile * tile)
{
    //mark current tile as checked
    if(tile->adjNum > 0) tile->floodChecked = true;

    //reveal empty adjacent tiles around revealed
    if(tile->revealed && !tile->floodChecked)
    {
        iterate3x3(tile, &MineGrid::floodReveal);
    }
}

void MineGrid::floodReveal(Tile * tile)
{
    //skip mines and already revealed
    if(tile->hasMine) return;
    if(tile->revealed) return;

    //proceed
    revealTile(tile);
}

void MineGrid::revealAround(Tile * tile)
{
    if(tile->revealed == false)
    {
        qDebug() << Q_FUNC_INFO << "revealing y,x" << tile->coords[0] <<  tile->coords[1];
        revealTile(tile);
    }
}

void MineGrid::iterateGrid(void (MineGrid::*tileFunc)(Tile *))
{
    qDebug() << Q_FUNC_INFO;

    for (int row = 0; row < gridSize; ++row)
    {
        for (int col = 0; col < gridSize; ++col)
        {
            (this->*tileFunc)(tiles[row][col]);
        }
    }
}

void MineGrid::iterate3x3(Tile * tile, void (MineGrid::*tileFunc)(Tile *))
{
    int row = tile->coords[0];
    int col = tile->coords[1];
    for (int i = row - 1; i < row + 2; ++i)
    {
        //prevent invalid index
        if(i < 0 || i >= gridSize) continue;

        for (int j = col - 1; j < col + 2; ++j)
        {
            //prevent invalid index
            if(j < 0 || j >= gridSize) continue;

            (this->*tileFunc)(tiles[i][j]);
        }
    }
}
