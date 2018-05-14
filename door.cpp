

#include<QPainter>

#include"constants.h"
#include"door.h"
static const qreal DOOR_SIZE= 10;
Door::Door(qreal x, qreal y,int t)
{
    setPos(x,y);
    setData(GD_Type,GD_DOOR);
    tar=t;
}

QRectF Door::boundingRect() const
{
    return QRectF(0,0,DOOR_SIZE,DOOR_SIZE);
}

void Door::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->save();
    painter->fillPath(shape(),QColor("#41F1F1"));
    painter->restore();
}
QPainterPath Door::shape() const
{
    QPainterPath path;
    path.addEllipse(QPointF(DOOR_SIZE/2,DOOR_SIZE/2),DOOR_SIZE/2,DOOR_SIZE/2);
    return path;
}
