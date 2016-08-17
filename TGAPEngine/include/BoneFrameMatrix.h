#ifndef BONEFRAMEMATRIX_H
#define BONEFRAMEMATRIX_H
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
namespace engine{
typedef struct FrameStruct{
    glm::quat quaternion;
    glm::vec3 location;
    unsigned int rangeMin;
    unsigned int rangeMax;
    unsigned int index;
}FrameStruct;

class BoneFrameMatrix
{
    public:
        BoneFrameMatrix(glm::quat frameQuaternion,glm::vec3 frameLocation,unsigned int frameRangeStart,unsigned int frameRangeEnd,unsigned int index):quaternion(frameQuaternion),location(frameLocation),index(index),rangeStart(frameRangeStart),rangeEnd(frameRangeEnd){};
        virtual ~BoneFrameMatrix(){};
        glm::quat quaternion;
        glm::vec3 location;
        unsigned int index;
        bool inRange(unsigned int index){ if((index<=rangeEnd)&&(index>=rangeStart)) {
                                            return true;
                                        }else {
                                            return false;}
                                            }
    protected:
        unsigned int rangeStart,rangeEnd;
    private:
};
}
#endif // BONEFRAMEMATRIX_H
