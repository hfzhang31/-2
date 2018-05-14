#include<QPainter>
#include"constants.h"

#include"forest.h"

static const qreal FOREST_SIZE = 10;

Forest::Forest(qreal x, qreal y)
{
    setPos(x,y);
    setData(GD_Type,GD_FOREST);
}

QRectF Forest::boundingRect() const
{
    return QRectF(0,0,
                  FOREST_SIZE,FOREST_SIZE);
}

void Forest::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->save();
    painter->fillPath(shape(),QColor("#309D30"));
    painter->restore();
}

QPainterPath Forest::shape() const
{
    QPainterPath path;
    for(int i=0;i<10;i+=2)
    {
        for(int j=0;j<9;j+=2)
        {
            path.addRect(i,j,1,1);
        }
    }
    for(int i=1;i<10;i+=2)
    {
        for(int j=i;j<10;j+=2)
        {
            path.addRect(i,j,1,1);
        }
    }
    return path;
}
