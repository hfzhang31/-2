#ifndef LOCK_H
#define LOCK_H

#include<QGraphicsItem>
#include"constants.h"
 class Lock:public QGraphicsItem
 {
 public:
     Lock(qreal x,qreal y);
     QRectF boundingRect() const;
     void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
     QPainterPath shape() const;

};


#endif // LOCK_H
