#ifndef BAMFILEREADER_H
#define BAMFILEREADER_H
#include <fstream>
#include <iostream>
#include <string>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include "VertexAttributes.h"
#include "Entity.h"
#include "Bone.h"
#include "BoneFrameMatrix.h"
#include "AnimatedActor.h"
#include "Config.h"
namespace engine{




class BamFileReader
{
    public:
        BamFileReader();
        virtual ~BamFileReader();
        AnimatedActor * loadFile(const char filepath[]);
		Entity * loadNoBoneFile(const char filepath[]);
    protected:
    private:
};
}
#endif // BAMFILEREADER_H
