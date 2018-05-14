#ifndef IRON_H
#define IRON_H

#include<QGraphicsItem>
#include"constants.h"
class Iron:public QGraphicsItem
{
public:
    Iron(qreal x,qreal y);

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QPainterPath shape() const;

};

#endif // IRON_H
