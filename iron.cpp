#include<QPainter>

#include"constants.h"
#include"iron.h"

static const qreal IRON_SIZE = 10;

Iron::Iron(qreal x, qreal y)
{
    setPos(x,y);
    setData(GD_Type, GD_IRON);

}

QRectF Iron::boundingRect() const
{
    return QRectF(0,0,
                  IRON_SIZE, IRON_SIZE);
}

void Iron::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->save();

    painter->fillPath(shape(),QColor("#999999"));
    for(int i = 1;i<=4;i++)
    {
        QPainterPath path;
        path.addRect(i,i,10-2*i,10-2*i);
        painter->fillPath(path,QColor(QString("#%1%2%3%4%5%6").arg(QString::number(9-i),
                                                                   QString::number(9-i),
                                                                   QString::number(9-i),
                                                                   QString::number(9-i),
                                                                   QString::number(9-i),
                                                                   QString::number(9-i))));
    }


    painter->restore();
}

QPainterPath Iron::shape() const
{
    QPainterPath path;
    path.addRect(0,0,10,10);
    return path;
}

