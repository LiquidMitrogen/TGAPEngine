#include "TimerEvent.h"
namespace engine{
TimerEvent::TimerEvent(float ltime,std::string name):name(name)
{
    this->callOnce = false;
    this->callOnce = false;
    this->time = ltime;
    this->tLeft = ltime;
    //ctor
}

TimerEvent::TimerEvent(float ltime,bool lcallOnce,std::string name):name(name)
{
    this->callOnce = lcallOnce;
    this->callOnce = false;
    this->time = ltime;
    this->tLeft = ltime;
    //ctor
}
TimerEvent::TimerEvent(float ltime,bool lcallOnce,bool lcallAnyway,std::string name):name(name)
{
    this->callOnce = lcallOnce;
    this->callAnyway = lcallAnyway;
    this->time = ltime;
    this->tLeft = ltime;
    //ctor
}
void TimerEvent::callMe()
{
    std::cout<<"called Base"<<this->time<<std::endl;
}






}
