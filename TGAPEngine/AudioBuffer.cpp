#include "AudioBuffer.h"
namespace engine{
AudioBuffer::AudioBuffer()
{
    unsigned int i = 0;
    if(USE_FILTER ==1){
        this->sincBlackmann = new float[FILTER_SIZE];
        this->blackmann = new float[FFT_FRAME_SIZE];
        lowPassGen(this->sincBlackmann,FILTER_SIZE,FILTER_CUTOFF);
        blackmanWindowGen(this->blackmann,FFT_FRAME_SIZE);
    }
    //-------------------------------------------kissfft init
    #ifndef DONT_USE_FFT
    in = new kiss_fft_scalar[FFT_FRAME_SIZE];
    out = new kiss_fft_cpx[FFT_FRAME_SIZE /2 + 1];
    config = kiss_fftr_alloc(FFT_FRAME_SIZE,0,NULL,NULL);
    #endif
    //-----------------------------------------------

    frameIndex = new int;
    prevBlockEnd = new int;
    *prevBlockEnd = 0;
    *frameIndex = 0;
    totalFrames = NUM_SECONDS * SAMPLE_RATE; /* Record for a few seconds. */

    samples = new SAMPLE[totalFrames];
    //std::cout<<samples<<" audio"<<std::endl;
    if( samples == NULL )
    {
        printf("Could not allocate record array.\n");

    }
    for( i=0; i<totalFrames; i++ ) samples[i] = 0;

}

AudioBuffer::~AudioBuffer()
{
    delete in;
    delete out;
    delete prevBlockEnd;
    delete frameIndex;
    delete samples;
}
SAMPLE * AudioBuffer::getDataBlockIndex()
{
    if(*this->prevBlockEnd + FFT_FRAME_SIZE > this->totalFrames){
        return this->samples;
    }else{
        return &this->samples[*this->prevBlockEnd];
    }
}
void AudioBuffer::fft()
{
    unsigned int le,k;
    int delta = (*this->frameIndex) - (*this->prevBlockEnd);
    //std::cout<<delta<<std::endl;
    //std::cout<<*this->frameIndex<<std::endl;
    if( delta < 0||delta >= FFT_FRAME_SIZE){

        if(USE_FILTER ==1){
                float output[FFT_FRAME_SIZE + FILTER_SIZE];
                float sampleIn[FFT_FRAME_SIZE];
                for(le=0;le<FFT_FRAME_SIZE;le++){
                    if((*this->prevBlockEnd + le) > this->totalFrames) {
                            //sampleIn[le] = 0;
                            int diff = *this->prevBlockEnd + le - this->totalFrames;
                            sampleIn[le] = this->samples[diff];
                    }else{

                        sampleIn[le] = this->samples[*this->prevBlockEnd+le];
                    }
                }
                for(k=0;k<FFT_FRAME_SIZE;k++){
                    sampleIn[k] = sampleIn[k] * this->blackmann[k];
                }
                engine::convolute(sampleIn,FFT_FRAME_SIZE,this->sincBlackmann,FILTER_SIZE,output);
                for(le=0;le<FFT_FRAME_SIZE;le++){
                    this->in[le] = output[le + (FILTER_SIZE / 2)];
                }

        }else
        for(le=0;le<FFT_FRAME_SIZE;le++){
            if((*this->prevBlockEnd + le) > this->totalFrames) {
                    this->in[le] = 0;
            }else{

                this->in[le] = this->samples[*this->prevBlockEnd+le];
            }
        }
        if(DEBUG_MODE == 1)
        std::cout<<"doing fft"<<std::endl;
        if(DEBUG_MODE == 1)
        std::cout.flush();
        kiss_fftr(config,in,out);//fft sie dzieje tutaj
        if(DEBUG_MODE == 1)
        std::cout<<in[128]<<"<in out>"<<out[128].r<<" at block "<<*this->prevBlockEnd<<" to "<<*this->prevBlockEnd + FFT_FRAME_SIZE<<" max = "<< this->totalFrames<<std::endl;
        if(DEBUG_MODE == 1)
        std::cout.flush();

        *this->prevBlockEnd = *this->prevBlockEnd + FFT_FRAME_SIZE;
    }
    if(*this->prevBlockEnd > this->totalFrames){
        *this->prevBlockEnd = 0;
    }
}
}
