#include "timer.h"
#include <iostream>

using namespace mechanics;
Timer::Timer(int ttl) : Action(), tickLeft(ttl),startTickConst(ttl)
{
}

Timer::~Timer()
{

}


int Timer::execute()
{
    tickLeft--;
    if (tickLeft > 0)
    {
        OnTick();
        Action::call(this);
        deleteMe = false;
        return 0;
    }
    if (tickLeft <= 0)
    {
        deleteMe = true;
        try{
            return OnExpire();
        }catch(exception e)
        {
            std::cerr << e.what() << "\n";
            deleteMe = true;
            return -1;
        }
    }
    return 0;
}

void Timer::OnTick()
{
    std::cerr << "called\n";
}
