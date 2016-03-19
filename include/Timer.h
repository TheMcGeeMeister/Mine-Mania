#ifndef TIMER_H
#define TIMER_H
#include <time.h>

class Timer
{
    public:
        Timer(double TimeSeconds);
        Timer();
        virtual ~Timer();
        bool Update();
        void StartNewTimer(double TimeSeconds);
    protected:
    private:
        int Clock_End;
};

#endif // TIMER_H
