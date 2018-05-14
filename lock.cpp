#include"lock.h"
#include<QPainter>

Lock::Lock(qreal x, qreal y)
{
    setPos(x,y);
    setData(GD_Type,GD_LOCK);
}


QRectF Lock::boundingRect() const
{
    return QRectF(0,0,10,10);
}


void Lock::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->save();

    painter->fillPath(shape(),Qt::gray);

    QPainterPath path;
    path.addRect(2.5,5,5,5);
    path.addRect(3,2,0.5,3);
    path.addRect(6.5,2,0.5,3);
    path.addRect(3,2,4,0.5);
    painter->fillPath(path,Qt::yellow);

    painter->restore();
}

QPainterPath Lock::shape() const
{
    QPainterPath path;
    path.addRect(0,0,10,10);
    return path;
}

