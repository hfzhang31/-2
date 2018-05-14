#ifndef BLINKBOARD_H
#define BLINKBOARD_H

#include<QGraphicsItem>
#include"constants.h"

class Blinkboard:public QGraphicsItem
{
public:
    Blinkboard(qreal x,qreal y,Direction d);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QRectF boundingRect() const;
    QPainterPath shape() const;
    Direction getDirection();
    void setDirection(Direction d);
private:
    Direction direction;

};

#endif // BLINKBOARD_H
