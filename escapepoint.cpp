#include "escapepoint.h"
#include <QRect>

using namespace std;

EscapePoint::EscapePoint(QWidget *window, int speed, int diameter, int victimsCount):
    window(window),
    speed(speed),
    diameter(diameter),
    player( new QPoint(0,0)),
    victims( QVector<QPoint*>(victimsCount)),
    timer(new QTimer()),
    upDir(false),
    downDir(false),
    leftDir(false),
    rightDir(false)
{
    for (int i = 0; i < victims.size(); ++i){
        victims[i] = new QPoint(i * diameter, i * diameter);
    }
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

    for (int i = 0; i < victims.size(); ++i){
        QPoint* victim = victims.at(i);
        rect = {victim->x(), victim->y(), diameter, diameter};
        painter->setPen(Qt::red);
        painter->setBrush(QBrush(Qt::red));
        painter->drawEllipse(rect);
    }
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

    for (int i = 0; i < victims.size(); ++i){
        Escaping(victims[i]);
    }

    window->repaint();
}

void EscapePoint::Escaping(QPoint *victim){
    static const int priority = 5;
    static const double sensitivity = 0.1;

    int x = victim->x(), y = victim->y();
    int width = window->width(), height = window->height();
    if (player->x() == x)
        player->rx()++;
    if (player->y() == y)
        player->ry()++;

    vector<vector<double>> vectors(5);
    for (size_t i = 0; i < 5; ++i)
        vectors[i].resize(2);
    vectors = {
        {1.0 / x, 1.0 / y},
        {1.0 / x, 1.0 / (y - height + diameter)},
        {1.0 / (x - width + diameter), 1.0 / (y - height + diameter)},
        {1.0 / (x - width + diameter), 1.0 / y},
        {1.0 / (x - player->x()) * priority, 1.0 / (y - player->y()) * priority}
    };

    double temp, maxPriority = 0;
    size_t k, m;
    for (size_t i = 0; i < 4; ++i)
        if ((temp = sqrt(pow(vectors[i][0], 2) + pow(vectors[i][1], 2))) > maxPriority){
           maxPriority = temp;
           k = i;
        }
    vectors[k][0] *= priority;
    vectors[k][1] *= priority;
    if ((int)k - 2 < 0)
        m = k + 2;
    else
        m = k - 2;
    vectors[m][0] = vectors[m][1] = 0;

    vector<double> resultVector = {0,0};
    for (size_t i = 0; i < 5; ++i){
        resultVector[0] += vectors[i][0];
        resultVector[1] += vectors[i][1];
    }
    temp = max(abs(resultVector[0]), abs(resultVector[1]));
    int currentSpeed;
    if (temp < sensitivity)
        currentSpeed = (int)((temp / sensitivity) * speed);
    else
        currentSpeed = speed;
    victim->rx() += resultVector[0] / temp * currentSpeed;
    victim->ry() += resultVector[1] / temp * currentSpeed;
    CheckBorders(victim);
}

void EscapePoint::CheckBorders(QPoint *point){
    if (point->x() <= 0)
        point->rx() = 1;
    if (point->x() >= window->width() - diameter)
        point->rx() = window->width() - diameter - 1;
    if (point->y() <= 0)
        point->ry() = 1;
    if (point->y() >= window->height() - diameter)
        point->ry() = window->height() - diameter - 1;

}
