#ifndef FFTEVENT_H
#define FFTEVENT_H


#include "AnalysisFFT.h"

#include <iostream>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <portaudio.h>
#include "TimerEvent.h"
#include "Utility.h"
#include "Entity.h"
#include "MyTimerEventTest.h"

#include "AudioBuffer.h"
#include "Image.h"
#include "kissfft/kiss_fft.h"
#include "Config.h"
#include "MainEngine.h"



class FftEvent : public engine::TimerEvent
{
    public:
        FftEvent(float ltime,engine::Image * image,engine::Image * image2,engine::AnalysisFFT * fft,ActorTextureMatrixMaterial * material) :
                                                                engine::TimerEvent(ltime,"fft"),image(image),image2(image2),fft(fft),material(material){};
        FftEvent(float ltime,engine::Image * image,engine::Image * image2,engine::AnalysisFFT * fft,ActorTextureMatrixMaterial * material, bool lcallOnce) :
                                                                engine::TimerEvent(ltime,lcallOnce,"fft"),image(image),image2(image2),fft(fft),material(material){};
        FftEvent(float ltime,engine::Image * image,engine::Image * image2,engine::AnalysisFFT * fft,ActorTextureMatrixMaterial * material, bool lcallOnce, bool lcallAnyway)
                                                                : engine::TimerEvent(ltime,lcallOnce,lcallAnyway,"fft"),image(image),image2(image2),fft(fft),material(material){};
        virtual ~FftEvent(){};
        virtual void callMe();
        void setTex(bool tex) {
            this->tex = tex;
        }
    protected:
    private:
        ActorTextureMatrixMaterial * material;
        engine::Image * image;
        engine::Image * image2;
        bool first  = true;
        bool tex;
        engine::AnalysisFFT * fft;
        float oldEqualizer[9] = {0,0,0,0,0,0,0,0,0};
        float oldMax = 0;
};



#endif // FFTEVENT_H
