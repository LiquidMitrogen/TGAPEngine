#ifndef TIMERCONTROLLER_H
#define TIMERCONTROLLER_H
#include "TimerEvent.h"
#include <vector>
#include <list>
#include <iostream>
#include <memory>
#define GLFW_DLL
#include <GLFW/glfw3.h>
#include "Config.h"

namespace engine{
class TimerController
{
    public:
        TimerController();
        virtual ~TimerController();
        void check();
        void addEvent(TimerEvent * event);
    protected:
    private:
        int frame = 0;
        float frameAvg = 0;
        double lastTime;
        std::list<std::unique_ptr<TimerEvent>> events;
};
}

#endif // TIMERCONTROLLER_H
