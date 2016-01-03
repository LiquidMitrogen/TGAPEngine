#ifndef ANALYSISTEMPOENVELOPE_H
#define ANALYSISTEMPOENVELOPE_H



#include "Utility.h"
#include "AudioBuffer.h"


namespace engine{
class AnalysisTempoEnvelope
{
    public:
        AnalysisTempoEnvelope(AudioBuffer * audio);
        virtual ~AnalysisTempoEnvelope(){};
        bool calculateTempo(float threshold);
        float * envelope;
    protected:
        float * sampleIn;
        float * lowPass10Hz;
        float * lowPass80Hz;
        float * lowPassSampleIn;
        unsigned int lastFrameIndex;
        AudioBuffer * audio;
        unsigned int lastFrame = 0;
    private:
};
}
#endif // ANALYSISTEMPOENVELOPE_H
