#include"order.h"
#include"constants.h"
Order::Order()
{

}

Order::~Order()
{

}

int Order::check(QString s)
{
    if(s==QString("fire"))
    {
        return FIRE;
    }
    else if(s==QString("quad"))
    {
        return QUAD;
    }
    else if(s==QString("help"))
    {
        return HELP;
    }
    else if(s==QString("self"))
    {
        return SELF;
    }
    else if(s==QString("restart"))
    {
        return RESTART;
    }
    else if(s==QString("use medicine"))
    {
        return USE;
    }
    else if(s==QString("use bullet package"))
    {
        return USE+1;
    }
    else if(s==QString("use shield"))
    {
        return USE+2;
    }
    else if(s==QString("gold"))
    {
        return USE+3;
    }
    else if(s.contains("create "))
    {
        return CREATE;
    }
    else if(s.contains("search "))
    {
        return SEARCH;
    }
    else if(s==QString("save"))
    {
        return SAVE;
    }
    else if(s==QString("load"))
    {
        return LOAD;
    }
    return NONE;
}

