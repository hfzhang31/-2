#ifndef SNAKE_H
#define SNAKE_H

#include<QGraphicsItem>
#include<QRectF>
#include"constants.h"

class GameController;

class Snake:public QGraphicsItem
{
public:
    Snake(GameController & controller);

    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    bool hit();
    void setMoveDirection();
    int getHealth();
    Direction getDirection();
protected:
    void advance(int step);
private:
    void moveLeft();
    void moveRight();
    void moveUp();
    void moveDown();

    void handleCollisions();
    int health;
    QPointF head;
    int growing;
    int speed;
    QList<QPointF> tail;
    int tickCounter;
    Direction moveDirection;
    GameController &controller;

};

#endif // SNAKE_H
