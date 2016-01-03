#ifndef AUDIOBUFFER_H
#define AUDIOBUFFER_H

#include<iostream>
#include "kissfft/kiss_fft.h"
#include "kissfft/kiss_fftr.h"
#include "Utility.h"
#include "Config.h"



/* Select sample format. */
#if 1
#define PA_SAMPLE_TYPE  paFloat32
typedef float SAMPLE;
#define SAMPLE_SILENCE  (0.0f)
#define PRINTF_S_FORMAT "%.8f"
#elif 1
#define PA_SAMPLE_TYPE  paInt16
typedef short SAMPLE;
#define SAMPLE_SILENCE  (0)
#define PRINTF_S_FORMAT "%d"
#elif 1
#define PA_SAMPLE_TYPE  paInt8
typedef char SAMPLE;
#define SAMPLE_SILENCE  (0)
#define PRINTF_S_FORMAT "%d"
#else
#define PA_SAMPLE_TYPE  paUInt8
typedef unsigned char SAMPLE;
#define SAMPLE_SILENCE  (128)
#define PRINTF_S_FORMAT "%d"
#endif

#define USE_FILTER 0
#define SAMPLE_RATE  (22050)
#define FFT_FRAME_SIZE (1024)
#define FILTER_SIZE (201)
#define FILTER_CUTOFF (0.5f)
#define NUM_SECONDS     (3)




namespace engine{
class AudioBuffer
{
    public:
        AudioBuffer();
        virtual ~AudioBuffer();
        kiss_fft_cpx * getFftResult(){return out;};
        SAMPLE * getDataBlockIndex();
        void fft();
        int getFrameIndex(){
            return *this->frameIndex;
        };
        SAMPLE maxs;
        int * frameIndex;//indeks w ktorym nastepuje zapis
        SAMPLE * samples;//próbki
        int getTotalFrames(){
            return totalFrames;
        };
    protected:
        kiss_fftr_cfg config;
        kiss_fft_scalar * in;
        kiss_fft_cpx * out;
        float * sincBlackmann;
        float * blackmann;



        int * prevBlockEnd;//koniec ostatnio przetworzonego bloku fft

        int totalFrames;//wielkosc bufora na próbki
    private:
};
}
#endif // AUDIOBUFFER_H
