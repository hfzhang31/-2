#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include<QObject>
#include<QTimer>
#include<QVector>
#include<QPair>
#include<QUdpSocket>
#include<QHostAddress>
#include"constants.h"
#include<QIODevice>
#include<QFile>
class QGraphicsScene;
class QKeyEvent;

class Tank;

class Bullet;
class Wall;
class Map;
class Order;
class MainWindow;
class Door;
class Enemy;
class Iron;
class EBullet;
class Win;
class River;
class Forest;
class Lock;
class Things;
class Box;
class Key;
class Blinkboard;
class Monster;
class Snake;
class Blood;

class GameController:public QObject
{
    Q_OBJECT
public:
    GameController(MainWindow* pw,QGraphicsScene &scene,QObject* parent=0);
    ~GameController();
    int tankHeadDireciton();
    QPointF tankPos();

    bool ifClearAll();

    void enterDoor(Tank*,Door*);
    void openBox(Tank*,Box*);
    void pickKey(Tank*,Key*);
    void blink(Tank*, Blinkboard*);
    void bulletHitWall(Bullet* bullet, Wall* wall);
    void bulletHitIron(Bullet* bullet,Iron* iron);
    void bulletBlink(Bullet*,Blinkboard*);
    void bulletHitMonster(Bullet*b, Monster*m);
    void bulletHitSnake(Bullet* bullet);
    void bulletHitTank(Bullet* bullet, Tank* t);

    void ebulletHitWall(EBullet* ebullet,Wall* wall);
    void ebulletHitEnemy(EBullet* ebullet,Enemy* enemy);
    void ebulletHitIron(EBullet* ebullet,Iron* iron);
    void ebulletHitTank(EBullet* ebullet, Tank* t);


    void snakeHitTank();

    int lh;
    int lb;
    int eh;
    int eb;

    void monsterHitTank(Monster* m,Tank* tank);
    Tank* getTank();

    bool connectSuccess;
    int id;
    QUdpSocket* sender;
    QUdpSocket* receiver;
    Tank* ptank;

    int keyCount;
    int bombCount;
    int health;
    QVector<int> pack;

    void send(QByteArray datagram);
    void processPendingDataGram();

public slots:
    void win();
    void pause();
    void resume();
    void gameOver();
    void loadMap();
    void enemyShoot();
    void bulletHitEnemy(Bullet* bullet,Enemy* enemy);
    void endWin();
protected:
    bool eventFilter(QObject* object,QEvent* event);
private:

    int n;
    void handleKeyPressed(QKeyEvent* event);
    void addNewTank();

    void fire(Tank *tank);
    void quad(Tank *tank);
    void self();
    void help();
    void restart();
    void createRoom(int x);
    void searchRoom(int x);
    void save();
    void load();

    void addEnemy(int x,int y);
    void addWall(int x,int y);
    void addIron(int x,int y);
    void addForest(int x,int y);
    void addRiver(int x,int y);
    void addLock(int x,int y);
    void addBox(int x,int y);
    void addKey(int x,int y);
    void addBlinkboard(int x,int y,Direction d);
    bool testWall(qreal x,qreal y);
    bool testEnemy(qreal x,qreal y);
    bool testIron(qreal x,qreal y);
    bool testRiver(qreal x,qreal y);
    bool testLock(qreal x,qreal y);
    void readXml();
    void dealOrder(int n);

    int mapRank;
    int mapCount;

    Blood* blood;
    Snake* snake;
    Things* things;
    Door* door;
    MainWindow* w;
    QString input;
    QString warn;
    Order* order;
    QVector<QPair<int,int>> wallList;
    QVector<QPair<int,int>> ironList;
    QVector<QPair<int,int>> enemyList;
    QVector<QPair<int,int>> enemyList2;
    QVector<QPair<int,int>> forestList;
    QVector<QPair<int,int>> riverList;
    QVector<QPair<QPair<int,int>,Lock*>> lockList;
    QVector<Map> mapArray;
    QTimer timer;
    QGraphicsScene &scene;
    Tank* tank;
};


#endif // GAMECONTROLLER_H
