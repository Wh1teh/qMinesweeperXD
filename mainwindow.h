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

    void mineFluff();

    //generated
    void on_bxSizes_editTextChanged(const QString &arg1);

    void on_bxSizes_currentIndexChanged(int index);



private:
    Ui::MainWindow *ui;

    MineGrid * mineGrid = nullptr;
    QBoxLayout * gridWrapper = nullptr;

    QTimer * smallDelay;
};
#endif // MAINWINDOW_H
