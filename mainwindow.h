#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "minegrid.h"
#include <QMainWindow>
#include <QDebug>
#include <QRect>
#include <QBoxLayout>
#include <QScreen>
#include <QTimer>
#include <QThread>
#include <QTime>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    //functions
    void initGrid();

private slots:
    //
    void updateCounters(int flagsAmount);
    void victory();
    void defeat();

    void skipEndSequence();

    void mineFluff();
    void updateClock();

    //generated
    void on_bxSizes_currentIndexChanged(int index);

    void on_leMines_textEdited(const QString &arg1);

private:
    Ui::MainWindow *ui;

    //pointers
    MineGrid * mineGrid = nullptr;
    QBoxLayout * gridWrapper = nullptr;

    QTimer * smallDelay;
    QTimer * timer;
    float delay = 1000.0;

    //flags
    enum Status {
        Idle,
        Running,
        Defeat,
        Victory
    }status = Idle;

    //variables
    int centiseconds = 0;

    //functions
    void disableButtons();
    void enableButtons();
};
#endif // MAINWINDOW_H
