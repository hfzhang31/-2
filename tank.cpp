#include<QPainter>
#include<QPainterPath>
#include"constants.h"
#include"gamecontroller.h"
#include"tank.h"
#include<QDebug>
static const qreal TANK_SIZE = 10;

Tank::Tank(GameController &controller,qreal x,qreal y):
    moveDirection(NoMove),
    controller(controller),
    headDirection(MoveUp)
{
    pos.rx() = x;
    pos.ry() = y;
    setData(GD_Type,GD_TANK);
    bombCount=50;
    health=100;
    keyCount=0;
    for(int i = 0;i<=2;i++)
    {
        pack.push_back(1);
    }
    gold=false;
}

void Tank::gainKey()
{
    keyCount++;
}

bool Tank::openBox()
{
    if(keyCount>0)
    {
        keyCount--;
        return true;
    }
    else
    {
        qDebug()<<"You have no key";
        return false;
    }
}

void Tank::bulletP()
{
    bombCount+=10;
}

void Tank::changeHealth(int n)
{
    health+=n;
}

void Tank::setpos(qreal x, qreal y)
{
    pos.rx()=x;
    pos.ry()=y;
}

int Tank::getHealth()
{
    return health;
}

int Tank::getCount()
{
    return bombCount;
}

void Tank::fireBomb(int n)
{
    bombCount-=n;
}

Tank::~Tank()
{
    qDebug()<<"~";
}

QRectF Tank::boundingRect() const
{
    QPointF tl = mapFromScene(QPointF(pos.x(),pos.y()));
    QPointF br = mapFromScene(QPointF(pos.x(),pos.y()));

    QRectF bound = QRectF(tl.x(),
                          tl.y(),
                          br.x() - tl.x() + TANK_SIZE,
                          br.y() - tl.x() + TANK_SIZE
                          );

    return bound;
}

QPainterPath Tank::shape() const
{
    //qDebug()<<"shape"<<endl;
    QPainterPath path;
    path.setFillRule(Qt::WindingFill);
    if(headDirection == MoveUp)
    {
        path.addRect(QRectF(TANK_SIZE/4,0,TANK_SIZE/2,TANK_SIZE/2));
        path.addRect(QRectF(0,TANK_SIZE/2,TANK_SIZE,TANK_SIZE/2));
    }
    if(headDirection == MoveDown)
    {
        path.addRect(QRectF(0,0,TANK_SIZE,TANK_SIZE/2));
        path.addRect(QRectF(TANK_SIZE/4,TANK_SIZE/2,TANK_SIZE/2,TANK_SIZE/2));
    }
    if(headDirection == MoveRight)
    {
        path.addRect(QRectF(0,0,TANK_SIZE/2,TANK_SIZE));
        path.addRect(QRectF(TANK_SIZE/2,TANK_SIZE/4,TANK_SIZE/2,TANK_SIZE/2));
    }
    if(headDirection == MoveLeft)
    {
        path.addRect(QRectF(0,TANK_SIZE/4,TANK_SIZE/2,TANK_SIZE/2));
        path.addRect(QRectF(TANK_SIZE/2,0,TANK_SIZE/2,TANK_SIZE));
    }

    return path;
}

void Tank::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    //qDebug()<<"paint"<<endl;
    painter->save();
    if(gold)
    {
        painter->fillPath(shape(),Qt::yellow);
    }
    else
    {
        if(controller.id==0)
        {
             painter->fillPath(shape(),Qt::green);
        }
        else if(controller.id == 1&&controller.connectSuccess)
        {
            if(local)
            {
                painter->fillPath(shape(),Qt::red);
            }
            else
            {
                painter->fillPath(shape(),Qt::blue);
            }

        }
        else if(controller.id==2&&controller.connectSuccess)
        {
            if(local)
            {
                painter->fillPath(shape(),Qt::blue);
            }
            else
            {
                painter->fillPath(shape(),Qt::red);
            }
        }
    }

    painter->restore();
}

void Tank::setMoveDirection(Direction direction)
{
    moveDirection = direction;
}

void Tank::setHeadDirection(Direction direction)
{
    headDirection = direction;
}

Direction Tank::getHeadDirection()
{
    return headDirection;
}

QPointF Tank::getPos()
{
    //qDebug()<<pos.x()<<" "<<pos.y()<<endl;
    return pos;
}

void Tank::advance(int step)
{
    if(!step)
    {
        return ;
    }
    if(moveDirection == NoMove)
    {
        return ;
    }

    switch(moveDirection)
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
    if(controller.connectSuccess&&local)
    {
        int x = pos.x();
        int y = pos.y();
        QString s = QString("id%1 point|%2|%3").arg(controller.id).arg(x).arg(y);
        qDebug()<<"send"<<s;
        controller.send(s.toLatin1());
    }

    handleCollisions();
}

void Tank::moveLeft()
{
    pos.rx() -= TANK_SIZE;
    if(pos.rx() < -200)
    {
        pos.rx() = -200;
    }
    headDirection = MoveLeft;
    moveDirection = NoMove;
}

void Tank::moveRight()
{
    pos.rx() += TANK_SIZE;
    if(pos.rx() > 190)
    {
        pos.rx() = 190;
    }
    headDirection = MoveRight;
    moveDirection = NoMove;
}

void Tank::moveUp()
{
    pos.ry() -= TANK_SIZE;
    if(pos.ry()<-150)
    {
        pos.ry() =-150;
    }
    headDirection = MoveUp;
    moveDirection = NoMove;
}

void Tank::moveDown()
{
    pos.ry() += TANK_SIZE;
    if(pos.ry() > 140)
    {
        pos.ry() = 140;
    }
    headDirection = MoveDown;
    moveDirection = NoMove;
}

int Tank::getKeyCount()
{
    return keyCount;
}

void Tank::handleCollisions()
{

    QList<QGraphicsItem *> collisions = collidingItems();

    foreach(QGraphicsItem *collidingItem, collisions)
    {
        if(collidingItem->data(GD_Type) == GD_DOOR)
        {
            //qDebug()<<"door"<<endl;
            controller.enterDoor(this,(Door*) collidingItem);
        }
        if(collidingItem->data(GD_Type)==GD_BOX)
        {
            controller.openBox(this,(Box*) collidingItem);
        }
        if(collidingItem->data(GD_Type)==GD_KEY)
        {
            controller.pickKey(this,(Key*) collidingItem);
        }
        if(collidingItem->data(GD_Type)==GD_BLINKBOARD)
        {
            controller.blink(this,(Blinkboard*) collidingItem);
        }
    }

}
