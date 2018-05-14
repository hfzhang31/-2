#include<QPainter>

#include"constants.h"
#include"enemy.h"

static const qreal ENEMY_SIZE = 10;

Enemy::Enemy(qreal x, qreal y)
{
    setPos(x,y);
    setData(GD_Type, GD_ENEMY);

}

QRectF Enemy::boundingRect() const
{
    return QRectF(0,0,
                  ENEMY_SIZE, ENEMY_SIZE);
}

void Enemy::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->save();

    painter->fillPath(shape(),Qt::red);

    painter->restore();
}

QPainterPath Enemy::shape() const
{
    QPainterPath path;
    path.addEllipse(QPointF(ENEMY_SIZE/2,ENEMY_SIZE/2),ENEMY_SIZE/2,ENEMY_SIZE/2);
    return path;
}

