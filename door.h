#ifndef DOOR_H
#define DOOR_H

#include<QGraphicsItem>
#include"constants.h"

class Door:public QGraphicsItem
{
public:
    Door(qreal x, qreal y, int t);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QPainterPath shape() const;
    int tar;
};

#endif // DOOR_H
