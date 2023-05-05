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

    //tiles.clear();

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

    emit gameStarted();

    for (int i = 0; i < tiles.count(); ++i) {
        if(tiles[i].contains(QObject::sender()))
        {
            qDebug() << Q_FUNC_INFO << "coords(x,y):"
                     << tiles[i].indexOf(QObject::sender()) << i;

            revealTile(tiles[i].indexOf(QObject::sender()), i);
            break;
        }
    }
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
    //qDebug() << Q_FUNC_INFO << QObject::sender();
    if(!crosshair)
        return;

    Tile * tile = qobject_cast<Tile*>(QObject::sender());

    int row = tile->coords[0];
    int col = tile->coords[1];
    for (int i = row - 1; i < row + 2; ++i) {
        //prevent invalid index
        if(i < 0 || i >= gridSize) continue;

        for (int j = col - 1; j < col + 2; ++j) {
            //prevent invalid index
            if(j < 0 || j >= gridSize) continue;

            tiles[i][j]->inCrosshair = true;
            tiles[i][j]->updateText();
        }
    }
}

void MineGrid::buttonUnhovered() //maybe unite with hover slot
{
    //qDebug() << Q_FUNC_INFO << QObject::sender();
    if(!crosshair)
        return;

    Tile * tile = qobject_cast<Tile*>(QObject::sender());

    int row = tile->coords[0];
    int col = tile->coords[1];
    for (int i = row - 1; i < row + 2; ++i) {
        //prevent invalid index
        if(i < 0 || i >= gridSize) continue;

        for (int j = col - 1; j < col + 2; ++j) {
            //prevent invalid index
            if(j < 0 || j >= gridSize) continue;

            tiles[i][j]->inCrosshair = false;
            tiles[i][j]->updateText();
        }
    }
}

void MineGrid::debugGrid()
{
    //debug
    QString huutista;
    for (int i = 0; i < gridSize; ++i) {
        huutista.clear();
        for (int j = 0; j < gridSize; ++j) {
            if(tiles[i][j]->hasMine) huutista.append("[x]");
            else if(tiles[i][j]->adjNum > 0)
            {
                huutista.append("[");
                huutista.append(QString::number(tiles[i][j]->adjNum));
                huutista.append("]");
            }

            else huutista.append("[ ]");
        }
        qDebug() << huutista;
    }
}

void MineGrid::putMines(int x, int y)
{
    qDebug() << Q_FUNC_INFO << "start";

    mines.clear();

    int ranX = 0;
    int ranY = 0;
    int minesPlaced = 0;
    while(minesPlaced < minesAmount)
    {
        ranX = QRandomGenerator::global()->bounded(gridSize);
        ranY = QRandomGenerator::global()->bounded(gridSize);

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

    //debug
    debugGrid();

    qDebug() << Q_FUNC_INFO << "end";
}

void MineGrid::putAdjNums()
{
    qDebug() << Q_FUNC_INFO << "start";

    for (int row = 0; row < gridSize; ++row) {
        for (int col = 0; col < gridSize; ++col) {

            //raise adj score around mine
            if(tiles[row][col]->hasMine)
            {
                for (int i = row - 1; i < row + 2; ++i) {
                    //prevent invalid index
                    if(i < 0 || i >= gridSize) continue;

                    for (int j = col - 1; j < col + 2; ++j) {
                        //prevent invalid index
                        if(j < 0 || j >= gridSize) continue;

                        tiles[i][j]->adjNum = tiles[i][j]->adjNum + 1;
                    }
                }
            }
        }
    }

    //debug
    debugGrid();

    qDebug() << Q_FUNC_INFO << "end";
}

void MineGrid::createButtons()
{
    qDebug() << Q_FUNC_INFO << "start";

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

    qDebug() << Q_FUNC_INFO << "end";
}

void MineGrid::revealTile(int x, int y)
{
    Tile * tile = tiles[y][x];

    if(tile->hasFlag)
    {
        flagsAmount--;
        emit gridUpdated(flagsAmount);
    }

    //prevent first click from triggering mine and init adjNums
    if(tilesRevealed == 0)
    {
        putMines(x, y);

        putAdjNums();
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

        floodFill(x, y);
    }

    //game is won
    if(tilesRevealed + minesAmount == gridSize * gridSize)
        emit victory();
}

void MineGrid::floodFill(int x, int y)
{
    if(tiles[y][x]->adjNum > 0) tiles[y][x]->floodChecked = true;

    for (int row = 0; row < gridSize; ++row) {
        for (int col = 0; col < gridSize; ++col) {

            //reveal empty adjacent tiles around revealed
            if(tiles[row][col]->revealed && !tiles[row][col]->floodChecked)
            {
                for (int i = row - 1; i < row + 2; ++i) {
                    //prevent invalid index
                    if(i < 0 || i >= gridSize) continue;

                    for (int j = col - 1; j < col + 2; ++j) {
                        //prevent invalid index
                        if(j < 0 || j >= gridSize) continue;

                        //skip mines and already revealed
                        if(tiles[i][j]->hasMine) continue;
                        if(tiles[i][j]->revealed) continue;

                        //proceed
                        tiles[y][x]->floodChecked = true;
                        revealTile(j, i);
                    }
                }
            }
        }
    }
}
