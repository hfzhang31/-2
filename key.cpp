#include"key.h"
#include<QPainter>

Key::Key(qreal x, qreal y)
{
    setPos(x,y);
    setData(GD_Type,GD_KEY);
}

QRectF Key::boundingRect() const
{
    return QRectF(0,0,10,10);
}

QPainterPath Key::shape() const
{
    QPainterPath path;
    path.addRect(0,0,4,4);
    path.addRect(4,4,2,2);
    path.addRect(6,6,2,2);
    path.addRect(8,8,2,2);
    return path;
}

void Key::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->save();

    painter->fillPath(shape(),QColor("#E89B29"));

    QPainterPath path;
    path.addRect(1.5,1.5,1,1);
    painter->fillPath(path,Qt::black);

    painter->restore();
}
