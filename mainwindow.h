#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "escapepoint.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

protected:
     void keyPressEvent(QKeyEvent * e) override;
     void keyReleaseEvent(QKeyEvent * e) override;
     void paintEvent(QPaintEvent *event) override;

private:
    Ui::MainWindow *ui;
    EscapePoint *esc;
};

#endif // MAINWINDOW_H
