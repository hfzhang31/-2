#ifndef WIN_H
#define WIN_H

#include"gamecontroller.h"
#include<QGraphicsItem>
class Win:public QGraphicsItem
{
public:
    Win(GameController & controller);
    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);


private:
    GameController &controller;
};

#endif // WIN_H
