#ifndef THING_H
#define THING_H

#include"gamecontroller.h"
#include<QMap>
#include<QString>
#include"tank.h"
#include<QDebug>

class Things
{
public:
    Things(GameController* pg);
    void use(int n);
    QMap<int,QString>& getList();
private:
    GameController* g;
    QMap<int,QString> list;
};

#endif // THING_H
