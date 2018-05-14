#include<QEvent>
#include<QGraphicsScene>
#include<QKeyEvent>
#include<QProcess>
#include<QtXml/QDomDocument>
#include<QMessageBox>
#include "gamecontroller.h"
#include "tank.h"
#include"door.h"
#include"bullet.h"
#include"wall.h"
#include"map.h"
#include"order.h"
#include"constants.h"
#include"mainwindow.h"
#include"ebullet.h"
#include"enemy.h"
#include"iron.h"
#include"win.h"
#include"forest.h"
#include"river.h"
#include"lock.h"
#include"thing.h"
#include"box.h"
#include"key.h"
#include"snake.h"
#include"blood.h"
#include"monster.h"
#include"blinkboard.h"
#include<QDebug>
#include<QTextCodec>

GameController::GameController(MainWindow* pw,QGraphicsScene &scene, QObject *parent):
    QObject(parent),
    scene(scene)
{
    w = pw;

    connectSuccess = false;
    id = 0;

    sender = new QUdpSocket(this);

    receiver = new QUdpSocket(this);

    timer.start(1000/33);

    bombCount = 40;
    health = 200;
    keyCount = 0;
    for(int i = 0;i<=3;i++)
    {
        pack.push_back(1);
    }
    order = new Order();
    things = new Things(this);
    scene.installEventFilter(this);
    mapRank = 0;
    readXml();
    loadMap();
    resume();
}

GameController::~GameController()
{

}
//===================================================================================
//CONNECT
//===================================================================================

void GameController::send(QByteArray datagram)
{
    sender->writeDatagram(datagram.data(),datagram.size(),
                          QHostAddress::Broadcast,10000);

}

void GameController::processPendingDataGram()
{
    while(receiver->hasPendingDatagrams())
    {
        QByteArray datagram;
        datagram.resize(receiver->pendingDatagramSize());
        receiver->readDatagram(datagram.data(), datagram.size());
        qDebug()<<datagram;
        QString data(datagram);
        if(data=="1 connect")
        {
            qDebug()<<"You have connect to the server,your id is 1";
            id=1;
        }
        else if(data=="2 connect")
        {
            qDebug()<<"You have connect to the server,your id is 2";
            id=2;
        }
        else if(data=="gs" && id > 0)
        {
            connectSuccess = true;
            qDebug()<<"Game start";
            qDebug()<<"Loading game";
            mapRank=7;
            loadMap();
            qDebug()<<"Game loads successfully";
            if(id==1)
            {
                tank = new Tank(*this,100,100);
                tank->local = true;
                scene.addItem(tank);
                ptank = new Tank(*this,-100,-100);
                ptank->local = false;
                lh=100;
                lb=20;
                eh=100;
                eb=20;
                scene.addItem(ptank);
                scene.update();
                qDebug()<<"Attention: your colour is red";
            }
            else if(id==2)
            {
                tank = new Tank(*this,-100,-100);
                tank->local = true;
                scene.addItem(tank);
                ptank = new Tank(*this,100,100);
                ptank->local = false;
                lh=100;
                lb=20;
                eh=100;
                eb=20;
                scene.addItem(ptank);
                scene.update();
                qDebug()<<"Attention: your colour is blue";
            }

        }
        else if(data=="up")
        {
            if(testWall(ptank->getPos().x(),ptank->getPos().y()-10)||
                    testEnemy(ptank->getPos().x(),ptank->getPos().y()-10)||
                    testIron(ptank->getPos().x(),ptank->getPos().y()-10)||
                    testRiver(ptank->getPos().x(),ptank->getPos().y()-10)||
                    testLock(ptank->getPos().x(),ptank->getPos().y()-10))
            {
                ptank->setHeadDirection(MoveUp);
                scene.update();
            }
            ptank->setMoveDirection(MoveUp);
        }
        else if(data=="down")
        {
            if(testWall(ptank->getPos().x(),ptank->getPos().y()+10)||
                    testEnemy(ptank->getPos().x(),ptank->getPos().y()+10)||
                    testIron(ptank->getPos().x(),ptank->getPos().y()+10)||
                    testRiver(ptank->getPos().x(),ptank->getPos().y()+10)||
                    testLock(ptank->getPos().x(),ptank->getPos().y()+10))
            {
                ptank->setHeadDirection(MoveDown);
                scene.update();
                break;
            }
            ptank->setMoveDirection(MoveDown);
        }
        else if(data=="left")
        {
            if(testWall(ptank->getPos().x()-10,ptank->getPos().y())||
                    testEnemy(ptank->getPos().x()-10,ptank->getPos().y())||
                    testIron(ptank->getPos().x()-10,ptank->getPos().y())||
                    testRiver(ptank->getPos().x()-10,ptank->getPos().y())||
                    testLock(ptank->getPos().x()-10,ptank->getPos().y()))
            {
                ptank->setHeadDirection(MoveLeft);
                scene.update();
                break;
            }
            ptank->setMoveDirection(MoveLeft);
        }
        else if(data=="right")
        {
            if(testWall(ptank->getPos().x()+10,ptank->getPos().y())||
                    testEnemy(ptank->getPos().x()+10,ptank->getPos().y())||
                    testIron(ptank->getPos().x()+10,ptank->getPos().y())||
                    testRiver(ptank->getPos().x()+10,ptank->getPos().y())||
                    testLock(ptank->getPos().x()+10,ptank->getPos().y()))
            {
                ptank->setHeadDirection(MoveRight);
                scene.update();
                break;
            }
            ptank->setMoveDirection(MoveRight);
        }
        else if(data=="fire")
        {
            fire(ptank);
        }
        else if(data.contains("point"))
        {
            bool ok;
            int x = data.section('|',1,1).toInt(&ok, 10);
            int y = data.section('|',2,2).toInt(&ok, 10);
            qDebug()<<x<<" "<<y;
            ptank->setpos(x,y);
            ptank->setPos(x,y);
        }
    }
}

//====================================================================================
//gettank and enterdoor
//=====================================================================================
Tank* GameController::getTank()
{
    return tank;
}

void GameController::enterDoor(Tank *tank, Door *door)
{
    if(door->tar<=1000)
    {
        mapRank=door->tar;
        QTimer::singleShot(10, this, SLOT(loadMap()));
    }
    else
    {
        QTimer::singleShot(10, this, SLOT(win()));
    }
}
//==========================================================
//get new thing
//=======================================================
void GameController::openBox(Tank *t, Box *b)
{
    if(!b->opened())
    {
        if(keyCount>0)
        {
            b->open();
            scene.update();
            int s = qrand()%100;
            switch(mapRank)
            {
            case 0:
                if(s>=0&&s<=35)
                {
                    pack[0]++;
                    qDebug()<<"You get a "<<things->getList()[0];
                }
                if(s>=30&&s<=50)
                {
                    pack[1]++;
                    qDebug()<<"You get a "<<things->getList()[1];
                }
                if(s>=50&&s<=70)
                {
                    pack[2]++;
                    qDebug()<<"You get a "<<things->getList()[2];
                }
                if(s>=75&&s<=80)
                {
                    pack[3]++;
                    qDebug()<<"You get a "<<things->getList()[3];
                }
                break;
            case 1:
                if(s>=0&&s<=25)
                {
                    pack[0]++;
                    qDebug()<<"You get a "<<things->getList()[0];
                }
                if(s>=30&&s<=60)
                {
                    pack[1]++;
                    qDebug()<<"You get a "<<things->getList()[1];
                }
                if(s>=50&&s<=70)
                {
                    pack[2]++;
                    qDebug()<<"You get a "<<things->getList()[2];
                }
                if(s>=70&&s<=80)
                {
                    pack[3]++;
                    qDebug()<<"You get a "<<things->getList()[3];
                }
                break;
            case 2:
                if(s>=0&&s<=35)
                {
                    pack[0]++;
                    qDebug()<<"You get a "<<things->getList()[0];
                }
                if(s>=30&&s<=60)
                {
                    pack[1]++;
                    qDebug()<<"You get a "<<things->getList()[1];
                }
                if(s>=50&&s<=70)
                {
                    pack[2]++;
                    qDebug()<<"You get a "<<things->getList()[2];
                }
                if(s>=70&&s<=80)
                {
                    pack[3]++;
                    qDebug()<<"You get a "<<things->getList()[3];
                }
                break;
            case 3:
                if(s>=0&&s<=15)
                {
                    pack[0]++;
                    qDebug()<<"You get a "<<things->getList()[0];
                }
                if(s>=30&&s<=70)
                {
                    pack[1]++;
                    qDebug()<<"You get a "<<things->getList()[1];
                }
                if(s>=50&&s<=60)
                {
                    pack[2]++;
                    qDebug()<<"You get a "<<things->getList()[2];
                }
                if(s>=70&&s<=85)
                {
                    pack[3]++;
                    qDebug()<<"You get a "<<things->getList()[3];
                }
                break;
            case 4:
                if(s>=0&&s<=35)
                {
                    pack[0]++;
                    qDebug()<<"You get a "<<things->getList()[0];
                }
                if(s>=30&&s<=50)
                {
                    pack[1]++;
                    qDebug()<<"You get a "<<things->getList()[1];
                }
                if(s>=50&&s<=90)
                {
                    pack[2]++;
                    qDebug()<<"You get a "<<things->getList()[2];
                }
                if(s>=70&&s<=80)
                {
                    pack[3]++;
                    qDebug()<<"You get a "<<things->getList()[3];
                }
                break;
            case 5:
                if(s>=0&&s<=30)
                {
                    pack[0]++;
                    qDebug()<<"You get a "<<things->getList()[0];
                }
                if(s>=30&&s<=50)
                {
                    pack[1]++;
                    qDebug()<<"You get a "<<things->getList()[1];
                }
                if(s>=50&&s<=75)
                {
                    pack[2]++;
                    qDebug()<<"You get a "<<things->getList()[2];
                }
                if(s>=70&&s<=80)
                {
                    pack[3]++;
                    qDebug()<<"You get a "<<things->getList()[3];
                }
                break;
            }
        }
    }
    else
    {
        qDebug()<<"This box has been opened";
    }
}

void GameController::pickKey(Tank *t, Key *k)
{
    qDebug()<<"You get a key";
    t->gainKey();
    keyCount++;
    scene.removeItem(k);
    k=NULL;
    delete k;
}

void GameController::blink(Tank *t, Blinkboard *b)
{
    t->setMoveDirection(b->getDirection());
}

void GameController::bulletBlink(Bullet *bullet, Blinkboard *board)
{
    bullet->hitBlinkboard();
    bullet->setMoveDirection(board->getDirection());
}

//===========================================================
//MAP
//============================================================
void GameController::loadMap()
{
    scene.clear();
    wallList.clear();
    enemyList.clear();
    ironList.clear();
    riverList.clear();
    forestList.clear();
    Map m = mapArray[mapRank];
    for(int i = 0;i <=39;i++)
    {

        for(int j = 0;j<=29;j++)
        {
            if(m.get(i,j)==QChar('w'))
            {
                addWall(10*(i-20),10*(j-15));
            }
            else if(m.get(i,j) ==QChar('b'))
            {
                tank=new Tank(*this,10*(i-20),10*(j-15));
                scene.addItem(tank);
                tank->setPos(10*(i-20),10*(j-15));
            }
            else if(m.get(i,j) ==QChar('e'))
            {
                addEnemy(10*(i-20),10*(j-15));
            }
            else if(m.get(i,j) ==QChar('i'))
            {
                addIron(10*(i-20),10*(j-15));
            }
            else if(m.get(i,j)==QChar('f'))
            {
                addForest(10*(i-20),10*(j-15));
            }
            else if(m.get(i,j)==QChar('r'))
            {
                addRiver(10*(i-20),10*(j-15));
            }
            else if(m.get(i,j)==QChar('l'))
            {
                addLock(10*(i-20),10*(j-15));
            }
            else if(m.get(i,j)==QChar('m'))
            {
                Monster* m = new Monster(*this,10*(i-20),10*(j-15));
                scene.addItem(m);
                m->setPos(10*(i-20),10*(j-15));
            }
            else if(m.get(i,j)==QChar('x'))
            {
                addBox(10*(i-20),10*(j-15));
            }
            else if(m.get(i,j)==QChar('1'))
            {
                addBlinkboard(10*(i-20),10*(j-15),MoveUp);
            }
            else if(m.get(i,j)==QChar('2'))
            {
                addBlinkboard(10*(i-20),10*(j-15),MoveDown);
            }
            else if(m.get(i,j)==QChar('3'))
            {
                addBlinkboard(10*(i-20),10*(j-15),MoveLeft);
            }
            else if(m.get(i,j)==QChar('4'))
            {
                addBlinkboard(10*(i-20),10*(j-15),MoveRight);
            }
        }
    }
    for(int i=0;i<m.doors.size();i++)
    {
        Door* d = new Door(m.doors[i][0],m.doors[i][1],m.doors[i][2]);
        scene.addItem(d);
    }
    qDebug()<<mapArray[mapRank].getStory()<<endl;
    if(mapRank == 6)
    {
        snake = new Snake(*this);
        scene.addItem(snake);
        blood = new Blood(190,-140,snake);
        scene.addItem(blood);
        //qDebug()<<"blood";
    }
}

void GameController::readXml()
{
    QDomDocument doc;
    QFile file(":/game.xml");
    if(!file.open(QIODevice::ReadOnly))
    {
        //qDebug()<<"open file"<<endl;
        exit(1);
    }
    if(!doc.setContent(&file))
    {
        //qDebug()<<"error content"<<endl;
        exit(1);
    }
    file.close();
    QDomElement docElem = doc.documentElement();
    QDomElement maps = docElem.firstChildElement(); // maps
    QDomNodeList mapList = maps.childNodes();
    mapCount = mapList.count();
    for(int i = 0;i < mapList.count();i++)
    {
        Map map;
        QDomNode node = mapList.at(i);
        if(node.isElement())
        {
            QDomElement mapElement = node.toElement();
            QDomNodeList walls = mapElement.childNodes();
            for(int j = 0;j<walls.count();j++)
            {
                QDomNode q = walls.at(j);
                if(q.isElement())
                {
                    QDomElement l = q.toElement();
                    if(l.tagName() == QString("walls"))
                    {
                        bool ok;
                        if(l.attribute("line")!=NULL)
                        {
                            int line = l.attribute("line").toInt(&ok,10);
                            QString s = l.text();
                            for(int z = 0;z<s.length();z++)
                            {
                                map.set(line+20,z,s[z]);
                            }
                        }
                        else if(l.attribute("row")!=NULL)
                        {
                            int row = l.attribute("row").toInt(&ok,10);
                            QString s = l.text();
                            for(int z = 0;z<s.length();z++)
                            {
                                map.set(z,row+15,s[z]);
                            }
                        }
                    }
                    else if(l.tagName()==QString("story"))
                    {
                        //qDebug()<<"story";
                        map.setStory(l.text());
                    }
                    else if(l.tagName()==QString("door"))
                    {
                        bool ok;
                        if(l.attribute("line")!=NULL && l.attribute("row")!=NULL)
                        {
                            int line = l.attribute("line").toInt(&ok,10);
                            int row = l.attribute("row").toInt(&ok,10);
                            int tar = l.text().toInt(&ok,10);
                            QVector<int> d;
                            d.push_back(10*line);
                            d.push_back(10*row);
                            d.push_back(tar);
                            map.doors.push_back(d);
                        }
                    }
                    /*
                    else if(l.tagName()==QString("enemy"))
                    {
                        bool ok;
                        if(l.attribute("line")!=NULL && l.attribute("row")!=NULL)
                        {
                            int line = l.attribute("line").toInt(&ok,10);
                            int row = l.attribute("row").toInt(&ok,10);
                            map.set(line+20,row+15,QChar('e'));
                        }
                    }
                    */
                }
            }
        }
        mapArray.push_back(map);
    }
}
//=======================================================================================
//HIT
//=======================================================================================
void GameController::bulletHitIron(Bullet *bullet, Iron *iron)
{
    scene.removeItem(bullet);
    bullet=NULL;
    delete bullet;
}

void GameController::bulletHitWall(Bullet* bullet, Wall *wall)
{
    //qDebug()<<"bu1"<<endl;
    int x = wall->pos().x();
    int y = wall->pos().y();
    for(int i = 0;i<wallList.size();i++)
    {
        if(wallList[i] == QPair<int,int>(x,y))
        {
            wallList.removeAt(i);
        }
    }
    scene.removeItem(wall);
    wall = NULL;
    delete wall;
    scene.removeItem(bullet);
    bullet=NULL;
    delete bullet;
}

void GameController::bulletHitEnemy(Bullet *bullet, Enemy *enemy)
{
    int x = enemy->pos().x();
    int y = enemy->pos().y();
    for(int i = 0;i<enemyList.size();i++)
    {
        if(enemyList[i] == QPair<int,int>(x,y))
        {
            enemyList.removeAt(i);
        }
    }
    scene.removeItem(enemy);
	enemy=NULL;
    delete enemy;
    scene.removeItem(bullet);
    bullet=NULL;
    delete bullet;
    if(ifClearAll())
    {
        for(int i = 0;i<=39;i++)
        {
            for(int j=0;j<=29;j++)
            {
                if(mapArray[mapRank].get(i,j)==QChar('k'))
                {
                    addKey(10*(i-20),10*(j-15));
                }
            }
        }
    }
}

void GameController::bulletHitMonster(Bullet* b,Monster* m)
{
    scene.removeItem(b);
    b=NULL;
    delete b;
    scene.removeItem(m);
    m=NULL;
    delete m;
}

void GameController::bulletHitSnake(Bullet* bullet)
{
    n = snake->hit();
    scene.removeItem(bullet);
    bullet=NULL;
    delete bullet;
    scene.update();
    if(n == true)
    {
        QTimer::singleShot(10,this,&GameController::win);
    }
}

void GameController::bulletHitTank(Bullet *bullet, Tank *t)
{
    if(connectSuccess==false)
    {
        if(t->gold==false)
        {
            health-=20;
        }
        else
        {
            t->gold=false;
            scene.update();
        }
        if(health<=0)
        {
            gameOver();
        }
    }
    else
    {
        if(t->local==false)
        {
            if(t->gold==false)
            {
                eh-=20;
            }
            else
            {
                t->gold=false;
                scene.update();
            }
            if(eh<=0)
            {
                qDebug()<<"you win";
                scene.removeItem(t);
                t = NULL;
                delete t;
                QTimer::singleShot(10,&scene,&QGraphicsScene::clear);
            }
        }
        else
        {
            if(t->gold==false)
            {
                lh-=20;
            }
            else
            {
                t->gold=false;
                scene.update();
            }
            if(lh<=0)
            {
                qDebug()<<"you lose";
                scene.removeItem(t);
                t = NULL;
                delete t;
                QTimer::singleShot(10,&scene,&QGraphicsScene::clear);
            }
        }
    }
}

void GameController::ebulletHitWall(EBullet* ebullet,Wall* wall)
{
    scene.removeItem(ebullet);
    ebullet=NULL;
    delete ebullet;
}

void GameController::ebulletHitEnemy(EBullet* ebullet,Enemy* enemy)
{
    scene.removeItem(ebullet);
    ebullet=NULL;
    delete ebullet;
}

void GameController::ebulletHitIron(EBullet* ebullet,Iron* iron)
{
    scene.removeItem(ebullet);
    ebullet=NULL;
    delete ebullet;
}

void GameController::ebulletHitTank(EBullet* ebullet,Tank* t)
{
    for(int i = 0;i<enemyList2.size();i++)
    {
        if(enemyList2[i].first==ebullet->getPos().rx()&&enemyList2[i].second==ebullet->getPos().ry())
        {
            return ;
        }
    }
    scene.removeItem(ebullet);
    ebullet=NULL;
    delete ebullet;
    if(tank->gold==false)
    {
        health-=20;
    }
    else
    {
        tank->gold=false;
        scene.update();
    }
    if(health<=0)
    {
        gameOver();
    }
}

void GameController::monsterHitTank(Monster *m, Tank *tank)
{
    tank->changeHealth(-40);
    if(tank->gold)
    {
        tank->gold=false;
        scene.update();
    }
    else
    {
        health-=40;
    }
    if(health<=0)
    {
        gameOver();
    }
    scene.removeItem(m);
    m=NULL;
    delete m;
}

void GameController::snakeHitTank()
{
    int di = 100; //distance
    if(tank->gold)
    {
        tank->gold=false;
    }
    else
    {
        health-=80;
        if(health<=0)
        {
            gameOver();
        }
    }
    int x = tank->getPos().rx();
    int y = tank->getPos().ry();
    int tx,ty;
    Direction d = snake->getDirection();
    switch(d)
    {
    case MoveUp:
		tx = x;
		ty = y-di;
		if(ty<-140)
		{
			ty=-140;
		}
        tank->setPos(tx,ty);
        tank->setpos(tx,ty);
        scene.update();
        break;
    case MoveDown:
        tx = x;
		ty = y+di;
		if(ty>130)
		{
			ty=130;
		}
        tank->setPos(tx,ty);
        tank->setpos(tx,ty);
        scene.update();
        break;
    case MoveRight:
        tx = x+di;
		ty = y;
		if(tx>180)
		{
			ty=180;
		}
        tank->setPos(tx,ty);
        tank->setpos(tx,ty);
        scene.update();
        break;
    case MoveLeft:
        tx = x-di;
		ty = y;
		if(tx<-190)
		{
			tx=-190;
		}
        tank->setPos(tx,ty);
        tank->setpos(tx,ty);
        scene.update();
        break;
    }

}

//===================================================================
//TEST
//===================================================================
bool GameController::testWall(qreal x, qreal y)
{
    QPair<int,int> t(x,y);
    for(int i = 0; i < wallList.size();i++)
    {
        if(wallList[i] == t)
        {
            return true;
        }
    }
    return false;
}

bool GameController::testIron(qreal x, qreal y)
{
    QPair<int,int> t(x,y);
    for(int i = 0; i < ironList.size();i++)
    {
        if(ironList[i] == t)
        {
            return true;
        }
    }
    return false;
}

bool GameController::testEnemy(qreal x,qreal y)
{
    QPair<int,int> t(x,y);
    for(int i = 0; i < enemyList.size();i++)
    {
        if(enemyList[i] == t)
        {
            return true;
        }
    }
    return false;
}

bool GameController::testRiver(qreal x, qreal y)
{
    QPair<int,int> t(x,y);
    for(int i = 0; i < riverList.size();i++)
    {
        if(riverList[i] == t)
        {
            return true;
        }
    }
    return false;
}

bool GameController::testLock(qreal x, qreal y)
{
    QPair<int,int> t(x,y);
    for(int i = 0; i < lockList.size();i++)
    {
        if(lockList[i].first == t)
        {
            return true;
        }
    }
    return false;
}

bool GameController::ifClearAll()
{
    if(enemyList.isEmpty())
    {
        return true;
    }
    else return false;
}

//=================================================================
//KEY PRESS
//=================================================================
void GameController::handleKeyPressed(QKeyEvent *event)
{
    int word=0;
    switch(event->key())
    {
    case Qt::Key_Left:
            if(connectSuccess)
            {
                QString d = QString("id%1 left").arg(id);
                send(d.toLatin1());
            }
            if(testWall(tank->getPos().x()-10,tank->getPos().y())||
                    testEnemy(tank->getPos().x()-10,tank->getPos().y())||
                    testIron(tank->getPos().x()-10,tank->getPos().y())||
                    testRiver(tank->getPos().x()-10,tank->getPos().y())||
                    testLock(tank->getPos().x()-10,tank->getPos().y()))
            {
                tank->setHeadDirection(MoveLeft);
                scene.update();
                break;
            }
            tank->setMoveDirection(MoveLeft);
            break;
    case Qt::Key_Right:
        if(connectSuccess)
        {
            QString d = QString("id%1 right").arg(id);
            send(d.toLatin1());
        }
            if(testWall(tank->getPos().x()+10,tank->getPos().y())||
                    testEnemy(tank->getPos().x()+10,tank->getPos().y())||
                    testIron(tank->getPos().x()+10,tank->getPos().y())||
                    testRiver(tank->getPos().x()+10,tank->getPos().y())||
                    testLock(tank->getPos().x()+10,tank->getPos().y()))
            {
                tank->setHeadDirection(MoveRight);
                scene.update();
                break;
            }
            tank->setMoveDirection(MoveRight);
            break;
    case Qt::Key_Up:
        if(connectSuccess)
        {
            QString d = QString("id%1 up").arg(id);
            send(d.toLatin1());
        }
            if(testWall(tank->getPos().x(),tank->getPos().y()-10)||
                    testEnemy(tank->getPos().x(),tank->getPos().y()-10)||
                    testIron(tank->getPos().x(),tank->getPos().y()-10)||
                    testRiver(tank->getPos().x(),tank->getPos().y()-10)||
                    testLock(tank->getPos().x(),tank->getPos().y()-10))
            {
                tank->setHeadDirection(MoveUp);
                scene.update();
                break;
            }
            tank->setMoveDirection(MoveUp);
            break;
    case Qt::Key_Down:
        if(connectSuccess)
        {
            QString d = QString("id%1 down").arg(id);
            send(d.toLatin1());
        }
        if(testWall(tank->getPos().x(),tank->getPos().y()+10)||
                testEnemy(tank->getPos().x(),tank->getPos().y()+10)||
                testIron(tank->getPos().x(),tank->getPos().y()+10)||
                testRiver(tank->getPos().x(),tank->getPos().y()+10)||
                testLock(tank->getPos().x(),tank->getPos().y()+10))
        {
            tank->setHeadDirection(MoveDown);
            scene.update();
            break;
        }
        tank->setMoveDirection(MoveDown);
        break;
    case Qt::Key_Space:
        input.append(" ");
        w->setLabel(input);
        break;
    case Qt::Key_Backspace:
        input.chop(1);
        w->setLabel(input);
        break;
    case Qt::Key_Return:
    case Qt::Key_Enter:
    {
        //qDebug()<<"enter"<<endl;
        int n = order->check(input);
        dealOrder(n);
        w->setLabel(input);
        w->setTell(warn);
        break;
    }
    }

    if(event->key()>= Qt::Key_A && event->key()<=Qt::Key_Z)
    {
        input.append(QChar(event->key()-Qt::Key_A+97));
        w->setLabel(input);
        //qDebug()<<"pd"<<endl;
        //qDebug()<<input<<endl;
    }
    if(event->key()>=Qt::Key_0 && event->key()<=Qt::Key_9)
    {
        input.append(QChar(event->key()-Qt::Key_0+48));
        w->setLabel(input);
    }

}

//==================================================================
//ORDER
//==================================================================
void GameController::dealOrder(int n)
{
    warn.clear();
    if(n == FIRE)
    {
        fire(tank);
    }
    else if(n == QUAD)
    {
        quad(tank);
    }
    else if(n == NONE)
    {
        warn = QString("wrong order");
    }
    else if(n==SELF)
    {
        self();
    }
    else if(n==HELP)
    {
        help();
    }
    else if(n>=USE)
    {
        things->use(n-USE);
    }
    else if(n==RESTART)
    {
        restart();
    }
    else if(n==CREATE)
    {
        bool ok;
        createRoom(input.section(' ',1,1).toInt(&ok,10));
    }
    else if(n==SEARCH)
    {
        bool ok;
        searchRoom(input.section(' ',1,1).toInt(&ok,10));
    }
    else if(n==SAVE)
    {
        save();
    }
    else if(n==LOAD)
    {
        load();
    }
    input.clear();
    scene.update();
}

void GameController::help()
{
    qDebug()<<"======================================================================";
    qDebug()<<"||                        TIPS OF THE GAME                          ||";
    qDebug()<<"||After you input an order you need to press the enter key to use it||";
    qDebug()<<"||                                                                  ||";
    qDebug()<<"||-----------------------------ORDERS-------------------------------||";
    qDebug()<<"||   fire: shoot a bullet forward                                   ||";
    qDebug()<<"||   quad: shoot in four directions                                 ||";
    qDebug()<<"||   use [thing]:use thing called [thing]                           ||";
    qDebug()<<"||   self: show the tank information                                ||";
    qDebug()<<"||   gold: consume a \"gold\" to protect you from next attack.        ||";
    qDebug()<<"||                                                                  ||";
    qDebug()<<"||----------------------------THINGS--------------------------------||";
    qDebug()<<"||   medicine: if your HP is less than 100, change it to 100        ||";
    qDebug()<<"||   bullet package: gain 20 bullets                                ||";
    qDebug()<<"||   shield: gain 50 HP                                             ||";
    qDebug()<<"======================================================================";
}

void GameController::restart()
{
    gameOver();
}

void GameController::self()
{
    qDebug()<<"bullet: "<<bombCount;
    qDebug()<<"health: "<<health;
    qDebug()<<"keys:   "<<keyCount;
    qDebug()<<"medicine:      "<<pack[0];
    qDebug()<<"bullet package:"<<pack[1];
    qDebug()<<"shield:        "<<pack[2];
    qDebug()<<"gold:          "<<pack[3];
}

void GameController::fire(Tank *tank)
{
    if(connectSuccess&&tank->local)
    {
        send(QString("id%1 fire").arg(id).toLatin1());
        if(lb>0)
        {
            lb-=1;
            Bullet *bullet = new Bullet(*this,tank);
            scene.addItem(bullet);
            warn=QString("");
            lh+=20;
        }
        else
        {
            qDebug()<<"You have no bullet";
            warn=QString("You have no bullet");
        }
    }
    else if(connectSuccess&&!tank->local)
    {
        if(eb>0)
        {
            qDebug()<<"enemy fire";
            eb-=1;
            Bullet *bullet = new Bullet(*this,tank);
            scene.addItem(bullet);
            warn=QString("");
            eh+=20;
        }
    }
    else
    {
        if(bombCount>0)
        {
            tank->fireBomb(1);
            bombCount-=1;
            Bullet *bullet = new Bullet(*this,tank);
            scene.addItem(bullet);
            warn=QString("");
        }
        else
        {
            qDebug()<<"You have no bullet";
            warn=QString("You have no bullet");
        }
    }

}

void GameController::quad(Tank *tank)
{
    if(bombCount>2)
    {
        tank->fireBomb(3);
        bombCount-=3;
        Direction d = tank->getHeadDirection();
        tank->setHeadDirection(MoveUp);
        Bullet *bullet1 = new Bullet(*this,tank);
        scene.addItem(bullet1);
        tank->setHeadDirection(MoveDown);
        Bullet *bullet2 = new Bullet(*this,tank);
        scene.addItem(bullet2);
        tank->setHeadDirection(MoveRight);
        Bullet *bullet3 = new Bullet(*this,tank);
        scene.addItem(bullet3);
        tank->setHeadDirection(MoveLeft);
        Bullet *bullet4 = new Bullet(*this,tank);
        scene.addItem(bullet4);
        tank->setHeadDirection(d);
    }
    else
    {
        qDebug()<<"You don't have 3 bombs";
        warn=QString("You don't have 3 bombs");
    }

}

QPointF GameController::tankPos()
{
    return tank->getPos();
}

int GameController::tankHeadDireciton()
{
    return tank->getHeadDirection();
}

void GameController::createRoom(int x)
{
    qDebug()<<"createRoom"<<x;
    QByteArray datagram = QString("%1").arg(x).toLatin1();
    sender->writeDatagram(datagram.data(),datagram.size(),
                          QHostAddress::Broadcast,10000);
    receiver->bind(10000+x,QUdpSocket::ShareAddress);
    QObject::connect(receiver,&QUdpSocket::readyRead,
                     this,&GameController::processPendingDataGram);
}

void GameController::searchRoom(int x)
{

}

void GameController::save()
{
    if(connectSuccess)
    {
        qDebug()<<"you cannot use this in PvP modle";
        return ;
    }
    QFile infile("C:\\Users\\HY\\Desktop\\tank\\load.txt");
    if(!infile.open(QIODevice::WriteOnly|QIODevice::Text))
    {
        qDebug()<<"load fail";
        return ;
    }
    QTextStream file(&infile);
    QString infor;
    infor.append(QString::number(health)).append(" ");
    infor.append(QString::number(bombCount)).append(" ");
    infor.append(QString::number(keyCount)).append(" ");
    for(int i = 0;i<=3;i++)
    {
        infor.append(QString::number(pack[i])).append(" ");
    }
    //qDebug()<<infor;
    file<<infor;
    infile.close();
    file.flush();
}

void GameController::load()
{
    if(connectSuccess)
    {
        qDebug()<<"you cannot use this in PvP modle";
    }
    QFile infile("C:\\Users\\HY\\Desktop\\tank\\load.txt");
    if(!infile.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        qDebug()<<"load fail";
        return ;
    }
    QTextStream file(&infile);
    //qDebug()<<1;
    while(!file.atEnd())
    {
        bool ok;
        QString infor = file.readLine();
        //qDebug()<<infor;
        health = infor.section(' ',0,0).toInt(&ok,10);
        bombCount = infor.section(' ',1,1).toInt(&ok,10);
        keyCount = infor.section(' ',2,2).toInt(&ok,10);
        for(int i = 0;i<=3;i++)
        {
            pack[i]=infor.section(' ',i+3,i+3).toInt(&ok,10);
        }
    }
    infile.close();
    file.flush();
}

//=============================================================
//WIN OVER
//=============================================================
void GameController::win()
{
    enemyList.clear();
    wallList.clear();
    ironList.clear();
    scene.clear();
    endWin();
}

void GameController::endWin()
{
    Win* win=NULL;
    win = new Win(*this);
    scene.addItem(win);
}

void GameController::gameOver()
{
    qDebug()<<"You are killed";
    bombCount = 40;
    health = 100;
    keyCount = 0;
    for(int i = 0;i<=3;i++)
    {
        pack.push_back(1);
    }
    mapRank = 0;
    QTimer::singleShot(100,this,SLOT(loadMap()));
}
//===============================================================
//ADD
//===============================================================
void GameController::addWall(int x,int y)
{
    Wall *wall = new Wall(x,y);
    scene.addItem(wall);
    wallList.push_back(QPair<int,int>(x,y));
}

void GameController::addIron(int x, int y)
{
    Iron *iron = new Iron(x,y);
    scene.addItem(iron);
    ironList.push_back(QPair<int,int>(x,y));
}

void GameController::addEnemy(int x,int y)
{
    Enemy* enemy = new Enemy(x,y);
    scene.addItem(enemy);
    enemyList.push_back(QPair<int,int>(x,y));
    enemyList2.push_back(QPair<int,int>(x,y));
}

void GameController::addForest(int x, int y)
{
    Forest* forest = new Forest(x,y);
    scene.addItem(forest);
    forestList.push_back(QPair<int,int>(x,y));
}

void GameController::addRiver(int x, int y)
{
    River* river = new River(x,y);
    scene.addItem(river);
    riverList.push_back(QPair<int,int>(x,y));
}

void GameController::addLock(int x, int y)
{
    Lock* lock = new Lock(x,y);
    scene.addItem(lock);
    lockList.push_back(QPair<QPair<int,int>,Lock*>(QPair<int,int>(x,y),lock));
}

void GameController::addBox(int x, int y)
{
    Box* box = new Box(x,y);
    scene.addItem(box);
}

void GameController::addKey(int x, int y)
{
    Key* key=new Key(x,y);
    scene.addItem(key);
}

void GameController::addBlinkboard(int x, int y, Direction d)
{
    Blinkboard* b = new Blinkboard(x,y,d);
    scene.addItem(b);
}

void GameController::pause()
{
    disconnect(&timer, SIGNAL(timeout()),
               &scene, SLOT(advance()));
}

void GameController::resume()
{
    connect(&timer, SIGNAL(timeout()),
               &scene, SLOT(advance()));
    connect(&timer,SIGNAL(timeout()),
            this,SLOT(enemyShoot()));
}

void GameController::enemyShoot()
{
    for(int i = 0;i < enemyList.size();i++)
    {
        n = qrand()%10;
        if(n==1)
        {
            //Bullet* bullet = new Bullet(*this);
            EBullet* ebullet=new EBullet(*this,enemyList[i].first,enemyList[i].second);
            scene.addItem(ebullet);
        }
    }
}

bool GameController::eventFilter(QObject *object, QEvent *event)
{
    if(event->type() == QEvent::KeyPress)
    {
        handleKeyPressed((QKeyEvent*) event);
        return true;
    }
    else
    {
        return QObject::eventFilter(object, event);
    }
}
