#ifndef WALL_H
#define WALL_H

#include<QGraphicsItem>
#include"constants.h"
class Wall:public QGraphicsItem
{
public:
    Wall(qreal x,qreal y);

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QPainterPath shape() const;

};

#endif // WALL_H
