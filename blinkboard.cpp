#include<QPainter>

#include"blinkboard.h"

Blinkboard::Blinkboard(qreal x, qreal y, Direction d)
{
    setPos(x,y);
    setData(GD_Type,GD_BLINKBOARD);
    direction=d;
}

void Blinkboard::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->save();
    QPainterPath path2;
    path2.addRect(0,0,10,10);
    painter->fillPath(path2,QColor("#22DDDD"));
    QPainterPath path;
    if(direction==MoveUp)
    {
        path.addRect(4.5,0,1,10);
        path.addRect(4,1,2,1);
        path.addRect(3.5,2,3,1);
    }
    else if(direction==MoveDown)
    {
        path.addRect(4.5,0,1,10);
        path.addRect(4,8,2,1);
        path.addRect(3.5,7,3,1);
    }
    else if(direction==MoveLeft)
    {
        path.addRect(0,4.5,10,1);
        path.addRect(1,4,1,2);
        path.addRect(2,3.5,1,3);
    }
    else if(direction==MoveRight)
    {
        path.addRect(0,4.5,10,1);
        path.addRect(8,4,1,2);
        path.addRect(7,3.5,1,3);
    }
    painter->fillPath(path,QColor("#F350D2"));
    painter->restore();
}

QRectF Blinkboard::boundingRect() const
{
    return QRectF(0,0,10,10);
}

QPainterPath Blinkboard::shape() const
{
    QPainterPath path;
    path.addEllipse(0,0,10,10);
    return path;
}

Direction Blinkboard::getDirection()
{
    return direction;
}

void Blinkboard::setDirection(Direction d)
{
    direction = d;
}
