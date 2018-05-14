#ifndef BLOOD_H
#define BLOOD_H

#include<QGraphicsItem>
#include"constants.h"
class Snake;

class Blood:public QGraphicsItem
{
public:
    Blood(int x,int y,Snake* s);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QRectF boundingRect() const;
    QPainterPath shape() const;
private:
    Snake* snake;
};

#endif // BLOOD_H
