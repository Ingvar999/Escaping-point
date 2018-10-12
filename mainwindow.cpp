#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QKeyEvent>
#include <QPainter>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    esc(new EscapePoint(this, 10, 30))
{
    ui->setupUi(this);
    setWindowIcon(QIcon(":/D:/git/Icon1.ico"));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete esc;
}

void MainWindow::paintEvent(QPaintEvent *event){
    Q_UNUSED(event);
    QPainter painter(this);
    esc->Refresh(&painter);
}

void MainWindow::keyPressEvent(QKeyEvent *event){
    switch (event->key()){
        case Qt::Key_W:
            esc->SetUpDir(true);
        break;
        case Qt::Key_S:
            esc->SetDownDir(true);
        break;
        case Qt::Key_A:
            esc->SetLeftDir(true);
        break;
        case Qt::Key_D:
            esc->SetRightDir(true);
        break;
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent *event){
    switch (event->key()){
        case Qt::Key_W:
            esc->SetUpDir(false);
        break;
        case Qt::Key_S:
            esc->SetDownDir(false);
        break;
        case Qt::Key_A:
            esc->SetLeftDir(false);
        break;
        case Qt::Key_D:
            esc->SetRightDir(false);
        break;
    }
}
