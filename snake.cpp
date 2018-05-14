#include <QPainter>

#include "constants.h"
#include "gamecontroller.h"
#include "snake.h"
#include<QDebug>
static const qreal SNAKE_SIZE = 10;

Snake::Snake(GameController &controller):
    head(0,0),
    growing(10),
    speed(6),
    moveDirection(NoMove),
    controller(controller)
{
    setData(GD_Type,GD_SNAKE);
    health=100;
}

QRectF Snake::boundingRect() const
{
    qreal minX = head.x();
    qreal minY = head.y();
    qreal maxX = head.x();
    qreal maxY = head.y();

    foreach(QPointF p,tail){
        maxX = p.x() > maxX ? p.x() : maxX;
        maxY = p.y() > maxY ? p.y() : maxY;
        minX = p.x() < minX ? p.x() : minX;
        minY = p.y() < minY ? p.y() : minY;
    }
    QPointF tl = mapFromScene(QPointF(minX,minY));
    QPointF br = mapFromScene(QPointF(maxX,maxY));

    QRectF  bound = QRectF(tl.x(),
                           tl.y(),
                           br.x() - tl.x() + SNAKE_SIZE,
                           br.y() - tl.y() + SNAKE_SIZE
                           );
    return bound;
}

QPainterPath Snake::shape() const
{
    QPainterPath path;
    path.setFillRule(Qt::WindingFill);

    path.addRect(QRectF(0, 0, SNAKE_SIZE, SNAKE_SIZE));

    foreach (QPointF p, tail)
    {
        QPointF itemp = mapFromScene(p);
        path.addRect(QRectF(itemp.x(), itemp.y(), SNAKE_SIZE, SNAKE_SIZE));
    }

    return path;
}


void Snake::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->save();
    painter->fillPath(shape(), QColor("#4DBE30"));
    painter->restore();
}

void Snake::setMoveDirection()
{
    qreal tx = controller.tankPos().x();
    qreal ty = controller.tankPos().y();

    qreal x = tx - head.rx();
    qreal y = ty - head.ry();

    if(y < x && x+y>=0)
    {
        moveDirection = MoveRight;
    }
    else if(y >= x && x+y > 0)
    {
        moveDirection = MoveDown;
    }
    else if(y > x && x+y <=0)
    {
        moveDirection = MoveLeft;
    }
    else if(y <= x && y+x <0)
    {
        moveDirection = MoveUp;
    }
}

bool Snake::hit()
{
    health-=10;
    if(health<=20)
    {
        speed+=2;
    }
    if(health <= 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

int Snake::getHealth()
{
    return health;
}

void Snake::advance(int step)
{
    setMoveDirection();
    if (!step) {
        return;
    }
    if (tickCounter++ % speed != 0) {
        return;
    }
    if (moveDirection == NoMove) {
        return;
    }

    if (growing > 0) {
        QPointF tailPoint = head;
        tail << tailPoint;
        growing -= 1;
    } else {
        tail.takeFirst();
        tail << head;
    }

    switch (moveDirection) {
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

    setPos(head);
    handleCollisions();
}

void Snake::moveLeft()
{
    head.rx() -= speed*2;
    if (head.rx() < -200) {
        head.rx() = -200;
    }
}

void Snake::moveRight()
{
    head.rx() += speed*2;
    if (head.rx() > 190) {
        head.rx() = 190;
    }
}

void Snake::moveUp()
{
    head.ry() -= speed*2;
    if (head.ry() < -150) {
        head.ry() = -150;
    }
}

void Snake::moveDown()
{
    head.ry() += speed*2;
    if (head.ry() > 140) {
        head.ry() = 140;
    }
}

Direction Snake::getDirection()
{
    return moveDirection;
}

void Snake::handleCollisions()
{
    QList<QGraphicsItem *> collisions = collidingItems();
    foreach (QGraphicsItem *collidingItem, collisions)
    {
        if (collidingItem->data(GD_Type) == GD_TANK)
        {
            controller.snakeHitTank();
            //growing += 1;
        }
    }
}
