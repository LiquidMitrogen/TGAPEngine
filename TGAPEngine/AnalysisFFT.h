#ifndef ANALYSISFFT_H
#define ANALYSISFFT_H

#include <iostream>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <portaudio.h>
#include "Utility.h"
#include "AudioBuffer.h"
#include "kissfft/kiss_fft.h"
#include "Config.h"

namespace engine{

class AnalysisFFT
{
    public:
        AnalysisFFT(AudioBuffer * audio,unsigned int equalizerBins,bool useMagnitude, bool useEqualizer);
        virtual ~AnalysisFFT();
        void calculateFFT();

        kiss_fft_cpx * fftCpx;
        float magnitude[512];
        float * equalizerMagnitude;
        float maxMag;
    protected:
        unsigned int equalizerBins;
        bool useMagnitude;
        bool useEqualizer;
        engine::AudioBuffer * audio;

    private:
};
}
#endif // ANALYSISFFT_H
