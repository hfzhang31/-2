#ifndef EBULLET_H
#define EBULLET_H

#include<QGraphicsItem>
#include<QRectF>
#include"constants.h"

class GameController;
class Enemy;

class EBullet: public QGraphicsItem
{
public:
    EBullet(GameController & controller,qreal x,qreal y);
    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* ,QWidget*);
    QPointF getPos();
    //setMoveDireciton(Direction direction);
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

#endif // EBULLET_H
