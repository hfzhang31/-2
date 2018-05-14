#include<QPainter>

#include"constants.h"
#include"wall.h"

static const qreal WALL_SIZE = 10;

Wall::Wall(qreal x, qreal y)
{
    setPos(x,y);
    setData(GD_Type, GD_WALL);

}

QRectF Wall::boundingRect() const
{
    return QRectF(0,0,
                  WALL_SIZE, WALL_SIZE);
}

void Wall::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->save();
    QPainterPath path;

    for(int i = 1;i<10;i+=2)
    {
        path.addRect(0,i,10,1);
    }
    painter->fillPath(path,QColor("#CC8F33"));
    painter->fillPath(shape(),QColor("#A2503E"));
    painter->restore();
}

QPainterPath Wall::shape() const
{
    QPainterPath path;

    for(int i = 0;i<10;i+=2)
    {
        path.addRect(0,i,10,1);
    }
    for(int i = 0;i<10;i+=2)
    {
        path.addRect(i,0,1,10
                     );
    }
    return path;
}
