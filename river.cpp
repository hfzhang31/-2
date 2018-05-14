#include<QPainter>

#include"constants.h"
#include"river.h"

static const qreal RIVER_SIZE = 10;

River::River(qreal x, qreal y)
{
    setPos(x,y);
    setData(GD_Type,GD_RIVER);
}

QRectF River::boundingRect() const
{
    return QRectF(0,0,
                  RIVER_SIZE,RIVER_SIZE);
}

void River::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->save();
    painter->fillPath(shape(),QColor("#0968F7"));
    painter->restore();
}

QPainterPath River::shape() const
{
    QPainterPath path;
    path.addRect(0,0,RIVER_SIZE,RIVER_SIZE);
    return path;
}
