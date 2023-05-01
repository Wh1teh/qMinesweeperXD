#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    smallDelay = new QTimer(this);

    timer = new QTimer(this);

    connect(timer, SIGNAL(timeout()),
            this, SLOT(updateClock()));

    //initGrid();

    qDebug() << Q_FUNC_INFO;
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_bxSizes_editTextChanged(const QString &arg1)
{
    qDebug() << Q_FUNC_INFO;

    //get size (of one side) from combo box
    int gridSize = (arg1.split("x")[0].toInt());
    int totalSize = gridSize * gridSize;

    //update mines to 20% of grid size
    ui->leMines->setText(QString::number((float)totalSize * 0.2));
}

void MainWindow::on_bxSizes_currentIndexChanged(int index)
{
    qDebug() << Q_FUNC_INFO;

    //get size (of one side) from combo box
    int gridSize = ui->bxSizes->currentText().split("x")[0].toInt();
    int totalSize = gridSize * gridSize;

    //update mines to ~20% of grid size
    int minesAmount = (float)totalSize * 0.2;
    minesAmount -= minesAmount % 10;
    ui->leMines->setText(QString::number(minesAmount));

    initGrid();
}

void MainWindow::updateCounters(int flagsAmount)
{
    ui->lbFlags->setText(QString::number(mineGrid->minesAmount - flagsAmount));
}

void MainWindow::victory()
{
    qDebug() << Q_FUNC_INFO;

    status = Victory;

    mineGrid->freezeButtons();
}

void MainWindow::defeat()
{
    qDebug() << Q_FUNC_INFO;

    status = Defeat;

    mineGrid->freezeButtons();

    smallDelay->start(1000);

    connect(smallDelay, SIGNAL(timeout()),
            this, SLOT(mineFluff()));
}

void MainWindow::mineFluff()
{
    static float delay = 500.0;
    //int count = mineGrid->minesAmount - 1;
    mineGrid->minesAmount--;

    qDebug() << Q_FUNC_INFO << "delay, count:" << delay << mineGrid->minesAmount;

    if(mineGrid->minesAmount <= 0)
    {
        smallDelay->stop();
        delay = 1000.0;

        disconnect(smallDelay, SIGNAL(timeout()),
                this, SLOT(mineFluff()));

        return;
    }

    if(delay > 0)
    {
        float a = (float)mineGrid->gridSize;
        delay *= 0.9 + (a / (100.0 + a) / 10);

        if(delay <= 0)
            delay = 0;
    }

    smallDelay->start((int)delay);

    mineGrid->revealRandomMine();
}

void MainWindow::updateClock()
{
    if(status == Defeat)
    {
        timer->stop();
        return;
    }

    if(status == Victory)
    {
        static int i = 0;
        timer->start(100 / (mineGrid->gridSize));

        mineGrid->revealOneTile();

        i++;
        if(i > mineGrid->gridSize * mineGrid->gridSize)
        {
            timer->stop();
            i = 0;

            status = Idle;
        }

        return;
    }

    if(status == Idle)
    {
        status = Running;

        qDebug() << Q_FUNC_INFO << "start timer" << QObject::sender();
        timer->start(10);
    }
    centiseconds++;

    if(centiseconds > 999999)
    {
        timer->stop();
        return;
    }

    QString time;
    QString seconds = QString::number(centiseconds / 100);
    for (int i = 0; i < 4 - seconds.length(); ++i) {
        time.append("0");
    }
    time.append(seconds + ":");

    if(QString::number(centiseconds % 100).length() < 2)
        time.append("0");
    time.append(QString::number(centiseconds % 100));

    ui->lbTime->setText(time);
}

void MainWindow::initGrid()
{
    qDebug() << Q_FUNC_INFO;

    //get size (of one side) from combo box
    int gridSize = ui->bxSizes->currentText().split("x")[0].toInt();

    //get mines from line edit
    int minesAmount = ui->leMines->text().toInt();

    //delete existing grid if any
    delete mineGrid;
    mineGrid = nullptr;

    qDebug() << Q_FUNC_INFO << "initing grid: size, mines:"
             << gridSize << "x" << gridSize << "," << minesAmount;

    //create grid
    mineGrid = new MineGrid(this);

    ui->layoutWrapper->addWidget(mineGrid);

    mineGrid->createGrid(gridSize, minesAmount);

    mineGrid->show();

    //connections to grid
    connect(mineGrid, SIGNAL(gameStarted()),
            this, SLOT(updateClock()));
    connect(mineGrid, SIGNAL(gridUpdated(int)),
            this, SLOT(updateCounters(int)));
    connect(mineGrid, SIGNAL(victory()),
            this, SLOT(victory()));
    connect(mineGrid, SIGNAL(defeat()),
            this, SLOT(defeat()));

    //adjust screen size
    float screenH = QGuiApplication::primaryScreen()->geometry().height();
    int rectangle = (screenH * 0.8) / (32 / (float)gridSize);

    this->resize(rectangle, rectangle);
}
