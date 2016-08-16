#ifndef BONE_H
#define BONE_H
#define GLM_FORCE_RADIANS
#include <string>
#include <glm/glm.hpp>
#include <glm/gtx/matrix_interpolation.hpp>
#include <vector>
#include <list>
#include <memory>
#include <iostream>
//#define GLEW_STATIC
#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include "Config.h"

#include "Action.h"
namespace engine{

typedef struct BoneStruct{
    unsigned int childrenCount;
    unsigned int * childrenIndices;
    glm::mat4 * invMat;
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
		void applyAction(Action * action, unsigned int frame);
        void modifyBoneUnif(GLint * boneUnifArray);
		std::string getName(){
			return this->name;
		};

		unsigned int getIndex(){
			return this->index;
		};

		Bone * findBoneByName(std::string name);

		/*glm::quat getQuaternionForFrame(int frame);
		void setQuaternionForFrame(int frame, glm::quat quaternion);
		glm::vec3 getLocationForFrame(int frame);
		void setLocationForFrame(int frame, glm::vec3 location);*/
		glm::quat getQuaternion(){ return this->boneQuaternion; };
		void setQuaternion(glm::quat quaternion){ this->boneQuaternion = quaternion; };
		glm::vec3 getLocation(){ return this->boneLocation; };
		void setLocation(glm::vec3 location){ this->boneLocation = location; };


		void applyArmatureRotation(glm::quat rotation);

    protected:
		void applyChildrenTransformation(glm::vec3 location, glm::quat rotation);
		void rotateLocationAroundAPointForFrame(glm::vec3 location, glm::quat rotation);
		std::string name;
        unsigned int index;
        glm::mat4 * invertedBoneMat;
		glm::quat boneQuaternion;
		glm::vec3 boneLocation;
        std::list<std::unique_ptr<Bone>> children;

		bool boneNeedsUniformUpdate = true;
    private:
};
}

#endif // BONE_H
