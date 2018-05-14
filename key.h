#ifndef KEY_H
#define KEY_H

#include"constants.h"
#include<QGraphicsItem>

class Key:public QGraphicsItem
{
public:
    Key(qreal x,qreal y);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QRectF boundingRect() const;
    QPainterPath shape() const;
};

#endif // KEY_H
