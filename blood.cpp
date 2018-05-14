#include"blood.h"
#include"snake.h"
#include<QPainter>
#include<QDebug>
Blood::Blood(int x, int y, Snake *s)
{
    setPos(x,y);
    snake = s;
}

void Blood::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->save();
    painter->fillPath(shape(),Qt::red);
    painter->restore();
}

QPainterPath Blood::shape() const
{
    QPainterPath path;
    int h = snake->getHealth();
    //qDebug()<<h;
    for(int i = 0;i<=h;i+=10)
    {
        path.addRect(0,2*i,10,20);
    }
    return path;
}

QRectF Blood::boundingRect() const
{
    return QRectF(-200,-160,400,400);
}
