#include<QPainter>

#include"constants.h"
#include"gamecontroller.h"
#include"bullet.h"
#include"tank.h"
#include<QDebug>

static const qreal BULLET_SIZE = 1;
Bullet::Bullet(GameController &controller,Tank* tank = NULL):
    controller(controller),
    speed(3)
{
    moveDireciton = Direction(tank->getHeadDirection());
    QPointF p = tank->getPos();
    switch(moveDireciton)
    {
    case MoveUp:
        p.ry()-=5;
        break;
    case MoveDown:
        p.ry()+=5;
        break;
    case MoveLeft:
        p.rx()-=5;
        break;
    case MoveRight:
        p.rx()+=5;
        break;
    }

    setPos(p);
    pos = p;
    setData(GD_Type,GD_BULLET);
    //qDebug()<<"bullet "<<pos.x()<<" "<<pos.y()<<endl;
}

void Bullet::setMoveDirection(Direction direction)
{
    moveDireciton=direction;
}

QPointF Bullet::getPos()
{
    return pos;
}

Direction Bullet::getDirection()
{
    return moveDireciton;
}

QRectF Bullet::boundingRect() const
{
    return QRectF(0,0,
                  TILE_SIZE * 2,TILE_SIZE * 2);
}

QPainterPath Bullet::shape() const
{
    QPainterPath path;
    path.addEllipse(QPointF(TILE_SIZE/2,TILE_SIZE/2),BULLET_SIZE,BULLET_SIZE);

    return path;
}

void Bullet::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->save();
    painter->fillPath(shape(), Qt::white);
    painter->restore();
}

void Bullet::advance(int step)
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

void Bullet::moveLeft()
{
    pos.rx() -= speed * BULLET_SIZE;
    if(pos.rx() < -200)
    {
        delete this;
    }
}

void Bullet::moveRight()
{
    pos.rx() += speed * BULLET_SIZE;
    if(pos.rx() > 190)
    {
        delete this;
    }
}

void Bullet::moveUp()
{
    pos.ry() -= speed * BULLET_SIZE;
    if(pos.ry() < -150)
    {
        delete this;
    }
}

void Bullet::moveDown()
{
    pos.ry() += speed * BULLET_SIZE;
    if(pos.ry() > 140)
    {
        delete this;
    }
}

void Bullet::hitBlinkboard()
{
    switch(moveDireciton)
    {
    case MoveUp:
        pos.ry()-=5;
        break;
    case MoveDown:
        pos.ry()+=5;
        break;
    case MoveRight:
        pos.rx()+=5;
        break;
    case MoveLeft:
        pos.rx()-=5;
        break;
    }
}

void Bullet::handleCollisions()
{
    QList<QGraphicsItem*> collisions = collidingItems();
    foreach(QGraphicsItem *collidingItem, collisions)
    {
        if(collidingItem->data(GD_Type) == GD_WALL)
        {
            controller.bulletHitWall(this,(Wall*) collidingItem);;
        }
        if(collidingItem->data(GD_Type) == GD_ENEMY)
        {
            controller.bulletHitEnemy(this,(Enemy*) collidingItem);;
        }
        if(collidingItem->data(GD_Type) == GD_IRON)
        {
            controller.bulletHitIron(this,(Iron*) collidingItem);;
        }
        if(collidingItem->data(GD_Type)==GD_BLINKBOARD)
        {
            controller.bulletBlink(this,(Blinkboard*) collidingItem);
        }
        if(collidingItem->data(GD_Type)==GD_MONSTER)
        {
            controller.bulletHitMonster(this,(Monster*)collidingItem);
        }
        if(collidingItem->data(GD_Type) == GD_SNAKE)
        {
            controller.bulletHitSnake(this);
        }
        if(collidingItem->data(GD_Type) == GD_TANK)
        {
            controller.bulletHitTank(this,(Tank*) collidingItem);
        }
    }
}

