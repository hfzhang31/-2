#ifndef MAP_H
#define MAP_H


#include<QVector>
#include<QChar>
#include<QString>
#include"constants.h"
#include"door.h"
class Map
{
public:
    Map();
    ~Map();
    void set(int x,int y,QChar w);
    QChar get(int x,int y);
    void setStory(QString text);
    QString getStory();
    QVector<QVector<int>> doors;
private:
    QVector<QVector<QChar>> content;
    QString story;

};

#endif // MAP_H
