#include "AnalysisTempoEnvelope.h"
namespace engine{
AnalysisTempoEnvelope::AnalysisTempoEnvelope(AudioBuffer * audio): audio(audio)
{
    this->sampleIn = new float[1024];
    this->lowPass10Hz = new float[201];
    this->lowPass80Hz = new float[201];
    this->envelope = new float[1024 + 201];
    this->lowPassSampleIn = new float[1024+201];
    lowPassGen(lowPass10Hz,201,0.0005); // filtr o czestotliwosci odciecia rownej okolo 10hz przy czestotliwosci probkowania 22050 */
    lowPassGen(lowPass80Hz,201,0.01); // ~80HZ
    this->lastFrameIndex = audio->getFrameIndex();
}
bool AnalysisTempoEnvelope::calculateTempo(float threshold)
{
    unsigned int i;
    unsigned int currentFrame = audio->getFrameIndex();
    unsigned int a = 1023;
    if(this->lastFrameIndex <= currentFrame - 1024){
        for(i=0;i<1024;i++){
                if(currentFrame - (a  - i)<0 || currentFrame - (a  - i)>audio->getTotalFrames()){
                    sampleIn[i] = 0;
                }else{
                    //std::cout<<currentFrame - (a - i)<<std::endl;
                    sampleIn[i] = audio->samples[currentFrame - (a - i)];
                }
        }
        engine::convolute(sampleIn,1024,lowPass80Hz,201,lowPassSampleIn);
        engine::envelopeRectifier(&lowPassSampleIn[100],1024,lowPass10Hz,201,envelope);
        for(i = 100; i<1024+100;i++){

            if (envelope[i] >= threshold) return true;
        }
        return false;


    }else{
        return false;
}

}
}
