#include<QPainter>

#include"box.h"

Box::Box(qreal x, qreal y)
{
    setPos(x,y);
    setData(GD_Type,GD_BOX);
    ifOpen=false;
}

void Box::open()
{
    ifOpen=true;
}

bool Box::opened()
{
    return ifOpen;
}

void Box::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->save();
    painter->fillPath(shape(),QColor("#7D4D2D"));
    if(ifOpen==false)
    {
        QPainterPath path;
        path.addRect(0,0,10,1);
        path.addRect(0,0,1,10);
        path.addRect(0,9,10,1);
        path.addRect(9,0,1,10);
        path.addRect(0,4,10,1);
        path.addRect(4,3,2,2);
        painter->fillPath(path,QColor("#FFCF11"));
        QPainterPath path2;
        path2.addRect(4.5,3.25,1,1.5);
        painter->fillPath(path2,Qt::black);
    }
    else
    {
        QPainterPath path;
        path.addRect(0,0,10,1);
        path.addRect(0,0,1,10);
        path.addRect(0,9,10,1);
        path.addRect(9,0,1,10);
        path.addRect(0,2,10,1);
        path.addRect(0,7,10,1);
        painter->fillPath(path,QColor("#FFCF11"));
    }


    painter->restore();
}

QRectF Box::boundingRect() const
{
    return QRectF(0,0,10,10);
}

QPainterPath Box::shape() const
{
    QPainterPath path;
    path.addRect(0,0,10,10);
    return path;
}


