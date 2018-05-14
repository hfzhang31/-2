#include<QPainter>

#include"monster.h"
#include"gamecontroller.h"
#include"QDebug"
Monster::Monster(GameController& pg, qreal x, qreal y):
    controller(pg)
{
    pos.rx()=x;
    pos.ry()=y;
    setData(GD_Type,GD_MONSTER);
    direction=NoMove;
    speed =2;
}

Direction Monster::getDirection()
{
    return direction;
}

QRectF Monster::boundingRect() const
{
    return QRectF(0,0,10,10);
}

QPainterPath Monster::shape() const
{
    QPainterPath path;
    path.addEllipse(0,0,10,10);
    return path;
}

void Monster::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->save();

    painter->fillPath(shape(),Qt::black);
    QPainterPath path;
    for(double i = 0;i<=2.5;i+=0.5)
    {
        path.addRect(2+i,3+i,0.5,3-i);
    }
    for(double i=0;i<=2.5;i+=0.5)
    {
        path.addRect(5.5+i,5.5-i,0.5,0.5+i);
    }
    painter->fillPath(path,Qt::red);

    painter->restore();
}

void Monster::setDirection()
{
    qreal tx = controller.tankPos().x();
    qreal ty = controller.tankPos().y();

    qreal x = tx - pos.rx();
    qreal y = ty - pos.ry();

    if(y < x && x+y>=0)
    {
        direction = MoveRight;
    }
    else if(y >= x && x+y > 0)
    {
        direction = MoveDown;
    }
    else if(y > x && x+y <=0)
    {
        direction = MoveLeft;
    }
    else if(y <= x && y+x <0)
    {
        direction = MoveUp;
    }
}

void Monster::advance(int step)
{
    setDirection();
    if(!step)
    {
        return ;
    }
    if(direction == NoMove)
    {
        return ;
    }

    switch(direction)
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

void Monster::moveLeft()
{
    pos.rx()-=speed;
    if(pos.rx() < -200)
    {
        pos.rx() = -200;
    }
    direction=NoMove;
}

void Monster::moveRight()
{
    pos.rx() += speed;
    if(pos.rx() > 190)
    {
        pos.rx() = 190;
    }
    direction = NoMove;
}

void Monster::moveUp()
{
    pos.ry() -= speed;
    if(pos.ry()<-150)
    {
        pos.ry() =-150;
    }
    direction = NoMove;
}

void Monster::moveDown()
{
    pos.ry() += speed;
    if(pos.ry() > 140)
    {
        pos.ry() = 140;
    }
    direction = NoMove;
}

void Monster::handleCollisions()
{
    QList<QGraphicsItem *> collisions = collidingItems();
    foreach(QGraphicsItem *collidingItem, collisions)
    {
        if(collidingItem->data(GD_Type) == GD_TANK)
        {
            controller.monsterHitTank(this,(Tank*) collidingItem);
        }

    }
}
