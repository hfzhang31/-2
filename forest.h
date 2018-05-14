#ifndef FOREST_H
#define FOREST_H

#include<QGraphicsItem>
#include"constants.h"

class Forest:public QGraphicsItem
{
public:
    Forest(qreal x,qreal y);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QPainterPath shape() const;
};

#endif // FOREST_H
