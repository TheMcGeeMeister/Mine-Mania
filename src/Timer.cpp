#include "Timer.h"

Timer::Timer(double TimeSeconds)
{
    Clock_End=clock()+(TimeSeconds * 1000);
}

Timer::Timer()
{
    Clock_End=clock();
    return;
}

Timer::~Timer()
{
    //dtor
}

bool Timer::Update()
{
    if(clock()>=Clock_End)
    {
        return true;
    }else
    {
        return false;
    }
}

void Timer::StartNewTimer(double TimeSeconds)
{
    Clock_End=clock()+(TimeSeconds * 1000);
}
