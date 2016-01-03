#ifndef TIMEREVENT_H
#define TIMEREVENT_H
#include <iostream>
#include <string>
#include "Config.h"

namespace engine{
class TimerEvent
{
    public:
        TimerEvent(float ltime,bool lcallOnce,bool lcallAnyway,std::string name);
        TimerEvent(float ltime,bool lcallOnce,std::string name);
        TimerEvent(float ltime,std::string name);
        virtual ~TimerEvent() { if(DEBUG_MODE==1)std::cout<<"base destructor"<<std::endl;};
        void decTime(float value);
        void restoreTime();
        float getTime();
        virtual void callMe();
        bool getCallOnce();
        bool getCallAnyway();
        std::string name;
    protected:
        float time;
        float tLeft;
    private:
        bool callOnce;
        bool callAnyway;

};

inline void TimerEvent::decTime(float value)
{
    this->tLeft-=value;
}
inline void TimerEvent::restoreTime()
{
    this->tLeft=this->time;
}

inline float TimerEvent::getTime()
{
    return this->tLeft;
}
inline bool TimerEvent::getCallOnce()
{
    return this->callOnce;
}
inline bool TimerEvent::getCallAnyway()
{
    return this->callAnyway;
}
}
#endif // TIMEREVENT_H
