#ifndef RIVER_H
#define RIVER_H

#include<QGraphicsItem>
#include"constants.h"

class River:public QGraphicsItem
{
public:
    River(qreal x,qreal y);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QPainterPath shape() const;
};

#endif // RIVER_H
