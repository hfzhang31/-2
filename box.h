#ifndef BOX_H
#define BOX_H

#include<QGraphicsItem>
#include"constants.h"

class Box:public QGraphicsItem
{
public:
    Box(qreal x,qreal y);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QRectF boundingRect() const;
    QPainterPath shape() const;
    void open();
    bool opened();
private:
    bool ifOpen;
};

#endif // BOX_H
