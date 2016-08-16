#ifndef BONE_H
#define BONE_H
#define GLM_FORCE_RADIANS
#include <string>
#include <glm/glm.hpp>
#include <glm/gtx/matrix_interpolation.hpp>
#include "BoneFrameMatrix.h"
#include <vector>
#include <list>
#include <memory>
#include <iostream>
#define GLEW_STATIC
#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include "Config.h"
namespace engine{

typedef struct BoneStruct{
    unsigned int childrenCount;
    unsigned int * childrenIndices;
    unsigned int framesCount;
    glm::mat4 * invMat;
    FrameStruct * frames;
	char * name;
}BoneStruct;

typedef struct QuatLoc{
    glm::quat quaternion;
    glm::vec3 location;
}QuatLoc;
class Bone
{
    public:
        Bone(BoneStruct bones[], unsigned int thisIndex);
        virtual ~Bone(){};
        QuatLoc * getMatrixForFrame(unsigned int frame);
        void modifyBoneUnif(GLint * boneUnifArray, glm::mat4 * parentMatrix,glm::quat parentQuaternion,unsigned int frame);
		std::string getName(){
			return this->name;
		};

		unsigned int getIndex(){
			return this->index;
		};

		Bone * findBoneByName(std::string name);

		glm::quat getQuaternionForFrame(int frame);
		void setQuaternionForFrame(int frame, glm::quat quaternion);
		glm::vec3 getLocationForFrame(int frame);
		void setLocationForFrame(int frame, glm::vec3 location);

		void applyArmatureRotation(int frame, glm::quat rotation);

    protected:
		void applyChildrenTransformation(int frame, glm::vec3 location, glm::quat rotation);
		void rotateLocationAroundAPointForFrame(int frame, glm::vec3 location, glm::quat rotation);
		//void propagate
		//BoneFrameMatrix * getBoneFrameMatrixForFrame(int frame);
		std::string name;
        std::vector<std::unique_ptr<BoneFrameMatrix>> frameMatrices;
        unsigned int index;
        glm::mat4 * invertedBoneMat;
        std::list<std::unique_ptr<Bone>> children;
    private:
};
}

#endif // BONE_H
