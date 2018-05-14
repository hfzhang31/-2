#include"map.h"

Map::Map()
{
    for(int i = -20;i<=19;i++)
    {
        QVector<QChar> r;
        for(int j = -15;j<=14;j++)
        {
            char s = '0';
            r.push_back(s);
        }
        content.push_back(r);
    }
}

Map::~Map()
{

}

QString Map::getStory()
{
    return story;
}

void Map::set(int x, int y, QChar w)
{
    content[x][y] = w;
}

QChar Map::get(int x, int y)
{
    return content[x][y];
}

void Map::setStory(QString text)
{
    story=text;
}
