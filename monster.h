#ifndef MONSTER_H
#define MONSTER_H

#include<QGraphicsItem>
#include"constants.h"

class GameController;

class Monster:public QGraphicsItem
{
public:
    Monster(GameController& pg,qreal x,qreal y);
    Direction getDirection();
    QRectF boundingRect() const;
    QPainterPath  shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void setDirection();
protected:
    void advance(int step);
private:
    void moveLeft();
    void moveRight();
    void moveUp();
    void moveDown();
    int speed;
    void handleCollisions();
    QPointF pos;
    GameController &controller;
    Direction direction;
};

#endif // MONSTER_H
