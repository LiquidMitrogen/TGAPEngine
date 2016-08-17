#include "Bone.h"
namespace engine{
Bone::Bone(BoneStruct bones[], unsigned int thisIndex):index(thisIndex)
{
	this->boneLocation = glm::vec3(0.0f, 0.0f, 0.0f);
	this->boneQuaternion = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
	this->boneNeedsUniformUpdate = true;

    unsigned int i;
    Bone * boneTmp;
    this->invertedBoneMat = bones[index].invMat;
	this->name = std::string(bones[index].name);
	std::cout << this->name << std::endl;
	for (int i = 0; i < 4; i++){
		std::cout << (*invertedBoneMat)[i].x << " " << (*invertedBoneMat)[i].y << " " << (*invertedBoneMat)[i].z << " " << (*invertedBoneMat)[i].w << " " << std::endl;
	}
	
    for(i=0;i<bones[index].childrenCount;i++){
        boneTmp = new Bone(bones,bones[index].childrenIndices[i]);
        children.emplace_back(boneTmp);
    }
}
void Bone::applyAction(Action * action, unsigned int frame){
	this->boneNeedsUniformUpdate = true;

	Keyframe keyframe = action->getQuatLoc(this->index, frame);
	this->boneQuaternion = keyframe.quaternion;
	this->boneLocation = keyframe.location;

	for (std::list<std::unique_ptr<Bone>>::iterator it = this->children.begin(); it != this->children.end(); ++it){

		(*it)->applyAction(action, frame);

	}
}
void Bone::modifyBoneUnif(GLint* boneUnifArray)
{

	if (this->boneNeedsUniformUpdate == true){
		this->boneNeedsUniformUpdate = false;
		glm::mat4 originToLoc = glm::translate(glm::mat4(1.0f), this->boneLocation);
		glm::mat4 rotateMatrix = glm::mat4_cast(this->boneQuaternion);

		glm::mat4 * matrix = new glm::mat4(1.0f);
		*matrix = originToLoc* rotateMatrix * (*this->invertedBoneMat) *  (*matrix);
		const float *pSource = (const float*)glm::value_ptr(*matrix);
		glUniformMatrix4fv(boneUnifArray[this->index], 1, GL_FALSE, pSource);
		delete matrix;
	}

    for (std::list<std::unique_ptr<Bone>>::iterator it = this->children.begin() ; it != this->children.end(); ++it){

        (*it)->modifyBoneUnif(boneUnifArray);

    }
    

}

Bone * Bone::findBoneByName(std::string name){
	if (this->name == name){
		return this;
	}
	else{
		for (std::list<std::unique_ptr<Bone>>::iterator it = this->children.begin(); it != this->children.end(); ++it){

			Bone * bonePtr = NULL;
			bonePtr = (*it)->findBoneByName(name);
			if (bonePtr != NULL){
				return bonePtr;
			}

		}
	}
	return NULL;
}
//BoneFrameMatrix * Bone::getBoneFrameMatrixForFrame(int frame){
//	for (std::vector<std::unique_ptr<BoneFrameMatrix>>::iterator it = this->frameMatrices.begin(); it != this->frameMatrices.end(); ++it){
//		if ((*it)->index == frame){
//			return it->get();
//		}
//	}
//	std::cout << "BFM not found for frame " << frame << std::endl;
//	return NULL;
//}
//glm::quat Bone::getQuaternionForFrame(int frame){
//	for (std::vector<std::unique_ptr<BoneFrameMatrix>>::iterator it = this->frameMatrices.begin(); it != this->frameMatrices.end(); ++it){
//				if ((*it)->index == frame){
//					return (*it)->quaternion;
//				}
//		}
//		std::cout << "BFM not found for frame " << frame << std::endl;
//		return glm::quat(1.0f,0.0f,0.0f,0.0f);
//}
//
//void Bone::setQuaternionForFrame(int frame, glm::quat quaternion){
//	for (std::vector<std::unique_ptr<BoneFrameMatrix>>::iterator it = this->frameMatrices.begin(); it != this->frameMatrices.end(); ++it){
//		if ((*it)->index == frame){
//			(*it)->quaternion = quaternion;
//			return;
//		}
//	}
//	std::cout << "BFM not found for frame " << frame << std::endl;
//}


void Bone::rotateLocationAroundAPointForFrame(glm::vec3 location, glm::quat rotation){
	this->boneNeedsUniformUpdate = true;
	glm::vec3 oldloc = this->boneLocation;
	glm::vec4 translated = glm::vec4(oldloc.x - location.x, oldloc.y - location.y, oldloc.z - location.z, 1.0);
	glm::vec4 rotated = rotation * translated;
	glm::vec3 newLoc = glm::vec3(rotated.x + location.x, rotated.y + location.y, rotated.z + location.z);
	this->boneLocation = newLoc;
}

//glm::vec3 Bone::getLocationForFrame(int frame){
//	for (std::vector<std::unique_ptr<BoneFrameMatrix>>::iterator it = this->frameMatrices.begin(); it != this->frameMatrices.end(); ++it){
//		if ((*it)->index == frame){
//			return (*it)->location;
//		}
//	}
//	std::cout << "BFM not found for frame " << frame << std::endl;
//	return glm::vec3(0.0f, 0.0f, 0.0f);
//}
//void Bone::setLocationForFrame(int frame, glm::vec3 location){
//	for (std::vector<std::unique_ptr<BoneFrameMatrix>>::iterator it = this->frameMatrices.begin(); it != this->frameMatrices.end(); ++it){
//		if ((*it)->index == frame){
//			(*it)->location = location;
//			return;
//		}
//	}
//	std::cout << "BFM not found for frame " << frame << std::endl;
//}

void Bone::applyArmatureRotation(glm::quat rotation){
	this->boneNeedsUniformUpdate = true;
	glm::quat old = this->boneQuaternion;
	this->boneQuaternion = rotation * old;
	glm::vec3 pivot = this->boneLocation;

	applyChildrenTransformation(pivot, rotation);

	


}
void Bone::applyChildrenTransformation(glm::vec3 location, glm::quat rotation){

	for (std::list<std::unique_ptr<Bone>>::iterator it = this->children.begin(); it != this->children.end(); ++it){
		(*it)->boneNeedsUniformUpdate = true;
		glm::quat old = (*it)->getQuaternion();
		(*it)->setQuaternion(rotation * old);
		(*it)->rotateLocationAroundAPointForFrame(location, rotation);
		(*it)->applyChildrenTransformation(location, rotation);

	}
}
}
