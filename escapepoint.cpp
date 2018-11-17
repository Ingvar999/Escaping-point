#include "escapepoint.h"
#include <QRect>
#include <cmath>

using namespace std;

int sign(int x){
    if (x == 0){
        return 0;
    }
    else{
        return (x < 0 ? -1 : 1);
    }
}

EscapePoint::EscapePoint(QWidget *window, int speed, int diameter, int victimsCount):
    window(window),
    speed(speed),
    diameter(diameter),
    radius(diameter/2),
    xSpeed(0),
    ySpeed(0),
    players( QVector<QPoint*>(victimsCount + 1)),
    directions( QVector<QPoint*>(victimsCount + 1)),
    timer(new QTimer()),
    upDir(false),
    downDir(false),
    leftDir(false),
    rightDir(false)
{
    for (int i = 0; i < players.size(); ++i){
        players[i] = new QPoint(i * diameter + radius, i * diameter + radius);
        directions[i] = new QPoint();
    }
    connect(timer, SIGNAL(timeout()), this, SLOT(TimerSlot()));
    timer->start(50);
}

EscapePoint::~EscapePoint(){
    delete timer;
    for (int i = 0; i < players.size(); ++i){
        delete players[i];
        delete directions[i];
    }
}

void EscapePoint::Refresh(QPainter *painter){
    QRect rect = {players[0]->x()-radius, players[0]->y()-radius, diameter, diameter};
    painter->setPen(Qt::green);
    painter->setBrush(QBrush(Qt::green));
    painter->drawEllipse(rect);

    for (int i = 1; i < players.size(); ++i){
        QPoint* victim = players.at(i);
        rect = {victim->x()-radius, victim->y()-radius, diameter, diameter};
        painter->setPen(Qt::red);
        painter->setBrush(QBrush(Qt::red));
        painter->drawEllipse(rect);
    }
}

void EscapePoint::TimerSlot(){
    if(upDir)
        ySpeed = -speed;
    if(downDir)
        ySpeed = speed;
    if(leftDir)
        xSpeed = -speed;
    if(rightDir)
        xSpeed = speed;
    directions[0]->rx() = xSpeed;
    directions[0]->ry() = ySpeed;
    xSpeed -= sign(xSpeed);
    ySpeed -= sign(ySpeed);

    for (int i = 1; i < players.size(); ++i){
        Escaping(i);
    }

    MoveWithoutOverlapping();

    window->repaint();
}

void EscapePoint::Escaping(int i){
    static const int priority = 5;
    static const double sensitivity = 0.1;

    int x = players[i]->x(), y = players[i]->y();
    int width = window->width()-radius, height = window->height()-radius;
    if (players[0]->x() == x)
        players[0]->rx()++;
    if (players[0]->y() == y)
        players[0]->ry()++;

    vector<vector<double>> vectors(5);
    for (size_t i = 0; i < 5; ++i)
        vectors[i].resize(2);
    vectors = {
        {1.0 / (x-radius), 1.0 / (y-radius)},
        {1.0 / (x-radius), 1.0 / (y - height)},
        {1.0 / (x - width), 1.0 / (y - height)},
        {1.0 / (x - width), 1.0 / y},
        {1.0 / (x - players[0]->x()) * priority, 1.0 / (y - players[0]->y()) * priority}
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
    directions[i]->rx() = resultVector[0] / temp * currentSpeed;
    directions[i]->ry() = resultVector[1] / temp * currentSpeed;
}

void EscapePoint::MoveWithoutOverlapping(){
    int tempx, tempy, dx, dy;
    bool valid;
    for (int k = 0; k < speed; ++k){
        for (int i = 0; i < players.size(); ++i){
            tempx = players[i]->x() + sign(directions[i]->x());
            tempy = players[i]->y() + sign(directions[i]->y());
            directions[i]->rx() -= sign(directions[i]->x());
            directions[i]->ry() -= sign(directions[i]->y());
            valid = true;
            for(int j = 0; j < players.size(); ++j){
                if (j != i){
                    dx = tempx - players[j]->x();
                    dy = tempy - players[j]->y();
                    if (sqrt(dx*dx + dy*dy) < diameter){
                        valid = false;
                    }
                }
            }
            if (valid){
                players[i]->rx() = tempx;
                players[i]->ry() = tempy;
                CheckBorders(players[i]);
            }
        }
    }
}

void EscapePoint::CheckBorders(QPoint *point){
    if (point->x() <= radius)
        point->rx() = radius+1;
    if (point->x() >= window->width() - radius)
        point->rx() = window->width() - radius - 1;
    if (point->y() <= radius)
        point->ry() = radius+1;
    if (point->y() >= window->height() - radius)
        point->ry() = window->height() - radius - 1;

}
