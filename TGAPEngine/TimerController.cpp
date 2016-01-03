#include "TimerController.h"
namespace engine{
TimerController::TimerController()
{
    //this->events.reserve(10);
    this->lastTime = glfwGetTime();
    //ctor
}

TimerController::~TimerController()
{
    //dtor
}

void TimerController::addEvent(TimerEvent * event)
{

    this->events.emplace_back(event);
}


void TimerController::check()
{
double newTime = glfwGetTime();
double frameTime = newTime - this->lastTime;
//std::cout<<"frame time " << frameTime<<std::endl; //fpsy
frame++;
frameAvg+=frameTime;
if(frame>=1000){
    std::cout<<"sredni czas renderowania klatki przez 1000 klatek = " << frameAvg / 1000.0<<std::endl;
    frame = 0;
    frameAvg = 0;
}

//std::cout<<this->events.size()<<std::endl;
for (std::list<std::unique_ptr<TimerEvent>>::iterator it = this->events.begin() ; it != this->events.end(); ++it)

{
    (*it)->decTime(frameTime);
    if((*it)->getTime() <= 0.0)
    {
        if(DEBUG_MODE == 1)
        std::cout<<"calling "<<(*it)->name<<std::endl;
        (*it)->callMe();
        if(DEBUG_MODE == 1)
        std::cout<<"leaving "<<(*it)->name<<std::endl;
        if(!((*it)->getCallOnce())){
            (*it)->restoreTime();
        }else{
            //std::vector<std::unique_ptr<TimerEvent>>::iterator ip = it;
            //--it;
            it = this->events.erase(it);
            if(it == this->events.end()){
                break;
            }
        }
    }else{
        if((*it)->getCallAnyway()){
            if(DEBUG_MODE == 1)
            std::cout<<"calling "<<(*it)->name<<std::endl;
            (*it)->callMe();
            if(DEBUG_MODE == 1)
            std::cout<<"leaving "<<(*it)->name<<std::endl;
        }
    }
}

this->lastTime=newTime;
}

}
