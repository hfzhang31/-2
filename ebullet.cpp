#include<QPainter>

#include"constants.h"
#include"gamecontroller.h"
#include"ebullet.h"
#include"enemy.h"
#include<QDebug>

static const qreal EBULLET_SIZE = 2;
EBullet::EBullet(GameController &controller,qreal x,qreal y):
    controller(controller),
    speed(3)
{
    moveDireciton = Direction(qrand()%5);
    setPos(x,y);
    pos = QPointF(x,y);
}

QPointF EBullet::getPos()
{
    return pos;
}

QRectF EBullet::boundingRect() const
{
    return QRectF(0,0,
                  TILE_SIZE * 2,TILE_SIZE * 2);
}

QPainterPath EBullet::shape() const
{
    QPainterPath path;
    path.addEllipse(QPointF(TILE_SIZE/2,TILE_SIZE/2),EBULLET_SIZE,EBULLET_SIZE);

    return path;
}

void EBullet::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->save();
    painter->fillPath(shape(), Qt::red);
    painter->restore();
}

void EBullet::advance(int step)
{
    if(!step)
    {
        return;
    }
    switch(moveDireciton)
    {
    case MoveLeft:
        moveLeft();
        break;
    case MoveRight:
        moveRight();
        break;
    case MoveUp:
        moveUp();
        break;
    case MoveDown:
        moveDown();
        break;
    }

    setPos(pos);
    handleCollisions();
}

void EBullet::moveLeft()
{
    pos.rx() -= speed * EBULLET_SIZE;
    if(pos.rx() < -200)
    {
        delete this;
    }
}

void EBullet::moveRight()
{
    pos.rx() += speed * EBULLET_SIZE;
    if(pos.rx() > 190)
    {
        delete this;
    }
}

void EBullet::moveUp()
{
    pos.ry() -= speed * EBULLET_SIZE;
    if(pos.ry() < -150)
    {
        delete this;
    }
}

void EBullet::moveDown()
{
    pos.ry() += speed * EBULLET_SIZE;
    if(pos.ry() > 140)
    {
        delete this;
    }
}

void EBullet::handleCollisions()
{
    QList<QGraphicsItem*> collisions = collidingItems();
    foreach(QGraphicsItem *collidingItem, collisions)
    {
        if(collidingItem->data(GD_Type) == GD_WALL)
        {
            controller.ebulletHitWall(this,(Wall*) collidingItem);;
        }
        if(collidingItem->data(GD_Type) == GD_ENEMY)
        {
            //controller.ebulletHitEnemy(this,(Enemy*) collidingItem);;
        }
        if(collidingItem->data(GD_Type) == GD_IRON)
        {
            controller.ebulletHitIron(this,(Iron*) collidingItem);;
        }
        if(collidingItem->data(GD_Type) == GD_TANK)
        {
            controller.ebulletHitTank(this,(Tank*) collidingItem);;
        }
    }
}
