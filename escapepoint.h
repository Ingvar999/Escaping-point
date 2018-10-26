#ifndef ESCAPEPOINT_H
#define ESCAPEPOINT_H

#include <QObject>
#include <QPoint>
#include <QWidget>
#include <QTimer>
#include <QPainter>
#include <QVector>

class EscapePoint : public QObject
{
    Q_OBJECT

public:
    EscapePoint(QWidget *window, int speed, int diameter, int victimsCount);
    ~EscapePoint();

    void SetUpDir(bool val){upDir = val;}
    void SetDownDir(bool val){downDir = val;}
    void SetLeftDir(bool val){leftDir = val;}
    void SetRightDir(bool val){rightDir = val;}
    void Refresh(QPainter *painter);

private:
    QWidget *window;
    int speed, diameter;
    QPoint *player;
    QVector<QPoint*> victims;
    QTimer *timer;
    bool upDir, downDir, leftDir, rightDir;

    void CheckBorders(QPoint *point);
    void Escaping(QPoint *victim);

public slots:
    void TimerSlot();
};

#endif // ESCAPEPOINT_H
