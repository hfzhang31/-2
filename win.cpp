#include<QPainter>

#include"constants.h"
#include"gamecontroller.h"
#include"win.h"
#include<QDebug>

Win::Win(GameController &controller):
    controller(controller)
{
    setPos(-80,-80);
}

QRectF Win::boundingRect() const
{
    return QRectF(0,0,200,200);
}

QPainterPath Win::shape() const
{
    QPainterPath path;
    path.addRect(0,0,10,10);
    path.addRect(10,10,10,10);
    path.addRect(20,20,10,10);
    path.addRect(20,30,10,10);
    path.addRect(20,40,10,10);
    path.addRect(30,10,10,10);
    path.addRect(40,0,10,10);
    path.addRect(60,10,10,30);
    path.addRect(70,0,20,10);
    path.addRect(70,40,20,10);
    path.addRect(90,10,10,30);
    path.addRect(110,0,10,40);
    path.addRect(140,0,10,40);
    path.addRect(110,40,40,10);

    path.addRect(0,60,10,40);
    path.addRect(20,60,10,40);
    path.addRect(40,60,10,40);
    path.addRect(10,100,30,10);
    path.addRect(70,60,10,50);
    path.addRect(100,60,10,50);
    path.addRect(110,70,10,10);
    path.addRect(120,80,10,10);
    path.addRect(130,90,10,10);
    path.addRect(140,60,10,50);
    return path;
}

void Win::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->save();
    painter->fillPath(shape(),QColor("#F7F709"));
    painter->restore();
}
