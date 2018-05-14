#ifndef TANK_H
#define TANK_H

#include<QGraphicsItem>
#include<QRectF>
#include"constants.h"
class GameController;

class Tank:public QGraphicsItem
{
public:
    Tank(GameController& controller,qreal x,qreal y);
    ~Tank();
    Direction getHeadDirection();
    QPointF getPos();
    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);
    void setMoveDirection(Direction direction);
    void setHeadDirection(Direction direction);
    void fireBomb(int n);
    int getCount();
    void changeHealth(int n);
    int getHealth();
    void bulletP();
    void gainKey();
    bool openBox();
    void setpos(qreal x,qreal y);
    int getKeyCount();
    QVector<int> pack;
    bool gold;
    bool local;
protected:
    void advance(int step);
private:
    int keyCount;
    int bombCount;
    int health;
    void moveLeft();
    void moveRight();
    void moveUp();
    void moveDown();

    void handleCollisions();

    QPointF pos;
    GameController &controller;
    Direction moveDirection;
    Direction headDirection;
};

#endif // TANK_H
