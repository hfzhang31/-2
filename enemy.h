#ifndef ENEMY_H
#define ENEMY_H

#include<QGraphicsItem>
#include"constants.h"
class Enemy:public QGraphicsItem
{
public:
    Enemy(qreal x,qreal y);

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QPainterPath shape() const;

};

#endif // ENEMY_H
