#ifndef FFTTIMEREVENT_H
#define FFTTIMEREVENT_H

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include "TimerEvent.h"
#include "Utility.h"
#include "Entity.h"
#include "MyTimerEventTest.h"
#include "MainEngine.h"
#include "MicrophoneDataSource.h"


class FftTimerEvent : public engine::TimerEvent
{
    public:
        FftTimerEvent(float ltime,float * fftout,engine::MicrophoneDataSource * mic) : engine::TimerEvent(ltime,"fft"),buffer(fftout),mic(mic){};
        FftTimerEvent(float ltime,float * fftout,engine::MicrophoneDataSource * mic, bool lcallOnce) : engine::TimerEvent(ltime,lcallOnce,"fft"),buffer(fftout),mic(mic){};
        FftTimerEvent(float ltime,float * fftout,engine::MicrophoneDataSource * mic, bool lcallOnce, bool lcallAnyway)
                                                                        : engine::TimerEvent(ltime,lcallOnce,lcallAnyway,"fft"),buffer(fftout),mic(mic){};
        virtual ~FftTimerEvent(){};
        virtual void callMe();
    protected:
    private:
        float * buffer;
        unsigned int lastIndex = SAMPLE_RATE * NUM_SECONDS * NUM_CHANNELS + 1;
        engine::MicrophoneDataSource * mic;
};

#endif // FFTTIMEREVENT_H
