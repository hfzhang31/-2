#ifndef BULLET_H
#define BULLET_H

#include<QGraphicsItem>
#include<QRectF>
#include"constants.h"

class GameController;

class Bullet: public QGraphicsItem
{
public:
    Bullet(GameController & controller, Tank *tank);
    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* ,QWidget*);
    void setMoveDirection(Direction direction);
    QPointF getPos();
    Direction getDirection();
    void hitBlinkboard();
protected:
    void advance(int step);
private:
    void moveLeft();
    void moveRight();
    void moveUp();
    void moveDown();

    void handleCollisions();
    QPointF pos;
    int speed;
    Direction moveDireciton;
    GameController &controller;
};

#endif // BULLET_H
