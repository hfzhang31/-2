#include"thing.h"

Things::Things(GameController *pg):
    g(pg)
{
    list.insert(0,"medicine");
    list.insert(1,"bullet package");
    list.insert(2,"shield");
    list.insert(3,"gold");
}

QMap<int,QString>& Things::getList()
{
    return list;
}

void Things::use(int n)
{
    switch(n)
    {
    case 0:
        if(g->pack[0]>0)
        {
            qDebug()<<"You use medicine.";
            int n = g->health;
            if(n<=200)
            {
                g->health=200;
            }
            g->pack[0]--;
        }
        else
        {
            qDebug()<<"You have no medicine";
        }
        break;
    case 1:
        if(g->pack[1]>0)
        {
            qDebug()<<"You use bullet package";
            g->bombCount+=20;
            g->pack[1]--;
        }
        else
        {
            qDebug()<<"You have no bullet package";
        }

        break;
    case 2:
        if(g->pack[2]>0)
        {
            qDebug()<<"You use shield";
            g->getTank()->changeHealth(20);
            g->health+=80;
            g->pack[2]--;
        }
        else
        {
            qDebug()<<"You have no shield";
        }
        break;
    case 3:
        if(g->pack[3]>0)
        {
            qDebug()<<"GOLD ON";
            g->getTank()->gold = true;
            g->pack[3]--;
        }
        else
        {
            qDebug()<<"You have no gold";
        }
    }
}
