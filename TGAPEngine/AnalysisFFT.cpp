#include "AnalysisFFT.h"
namespace engine{
AnalysisFFT::AnalysisFFT(AudioBuffer * audio, unsigned int equalizerBins,bool useMagnitude, bool useEqualizer):useMagnitude(useMagnitude),useEqualizer(useEqualizer),equalizerBins(equalizerBins),audio(audio)
{
    equalizerMagnitude = new float[equalizerBins];
}

AnalysisFFT::~AnalysisFFT()
{
    //dtor
}
void AnalysisFFT::calculateFFT()
{
    audio->fft();
    fftCpx = audio->getFftResult();
    if(DEBUG_MODE == 1)
        std::cout<<"i got fft results"<<std::endl;
        //magnitude calculations
    int o,k;
    if(useMagnitude || useEqualizer){
    maxMag = 0;
    float minMag = 10000000;


    if(DEBUG_MODE == 1)
        std::cout<<"calculating magnitude"<<std::endl;

        for(o=1;o<513;o++){
            magnitude[o-1] =  sqrt(fftCpx[o].r * fftCpx[o].r + fftCpx[o].i * fftCpx[o].i);
            if(magnitude[o-1] < minMag)
                minMag = magnitude[o-1];

            if(magnitude[o-1] > maxMag)
                maxMag = magnitude[o-1];

            magnitude[o-1] = 20 * log10(magnitude[o-1]/0.2);
        }
    }
    //equalizer bin grouping calculations
    if(useEqualizer){
    if(DEBUG_MODE == 1)
        std::cout<<"grouping magnitude int equalizer"<<std::endl;
    float avgEq = 0;
    int numberOfMag = 512 / equalizerBins;
    for(o=0;o<9;o++){
        equalizerMagnitude[o] = 0;
        for(k=0;k<numberOfMag;k++){
            equalizerMagnitude[o] += magnitude[(o * numberOfMag) +k];
            if(equalizerMagnitude[o]<0.02) equalizerMagnitude[o] = 0.02;
        }
        avgEq += equalizerMagnitude[o];

    }
    }




}

}
