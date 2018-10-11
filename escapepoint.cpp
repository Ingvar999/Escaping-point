#include "escapepoint.h"
#include <QRect>

EscapePoint::EscapePoint(QWidget *window, int speed, int diameter):
    window(window),
    speed(speed),
    diameter(diameter),
    player( new QPoint(0,0)),
    victim( new QPoint(1,1)),
    timer(new QTimer()),
    upDir(false),
    downDir(false),
    leftDir(false),
    rightDir(false)
{
    connect(timer, SIGNAL(timeout()), this, SLOT(TimerSlot()));
    timer->start(50);
}

EscapePoint::~EscapePoint(){
    delete timer;
}

void EscapePoint::Refresh(QPainter *painter){
    QRect rect = {player->x(), player->y(), diameter, diameter};
    painter->setPen(Qt::green);
    painter->setBrush(QBrush(Qt::green));
    painter->drawEllipse(rect);

    rect = {victim->x(), victim->y(), diameter, diameter};
    painter->setPen(Qt::red);
    painter->setBrush(QBrush(Qt::red));
    painter->drawEllipse(rect);
}

void EscapePoint::TimerSlot(){
    if(upDir)
        player->ry() -= speed;
    if(downDir)
        player->ry() += speed;
    if(leftDir)
        player->rx() -= speed;
    if(rightDir)
        player->rx() += speed;
    CheckBorders(player);

    //victim logic

    window->repaint();
}

void EscapePoint::CheckBorders(QPoint *point){
    if (point->x() < 0)
        point->rx() = 0;
    if (point->x() > window->width() - diameter)
        point->rx() = window->width() - diameter;
    if (point->y() < 0)
        point->ry() = 0;
    if (point->y() > window->height() - diameter)
        point->ry() = window->height() - diameter;

}
