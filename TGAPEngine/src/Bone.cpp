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
Bone::Bone(unsigned int thisIndex, int nodeIndex, glm::mat4* invMat, std::string name):index(thisIndex), nodeIndex(nodeIndex), invertedBoneMat(invMat), name(name)
{

	this->boneLocation = glm::vec3(0.0f, 0.0f, 0.0f);
	this->boneQuaternion = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
	this->boneNeedsUniformUpdate = true;
}
void Bone::applyAction(Action<float> * action, float time){
	this->boneNeedsUniformUpdate = true;

	auto quaternion =  action->getQuat(this->nodeIndex, time);
	auto location =  action->getLoc(this->nodeIndex, time);
	if(quaternion.has_value())
		this->boneQuaternion = quaternion.value();
	if (location.has_value())
		this->boneLocation = location.value();

	for (std::list<std::unique_ptr<Bone>>::iterator it = this->children.begin(); it != this->children.end(); ++it){

		(*it)->applyAction(action, time);

	}
}
void Bone::modifyBoneUnif(GLint* boneUnifArray, glm::mat4 parentTransform)
{
	glm::mat4 originToLoc = glm::translate(glm::mat4(1.0f), this->boneLocation);
	glm::mat4 rotateMatrix = glm::mat4_cast(this->boneQuaternion);
	auto newParentTransform = parentTransform * originToLoc * rotateMatrix;

	if (this->boneNeedsUniformUpdate == true){
		this->boneNeedsUniformUpdate = false;

		glm::mat4 * matrix = new glm::mat4(1.0f);
		//glm::vec4 test(4.38f, 6.39f, 0.21f, 1.0f);
		//std::cout << test.x << "  " << test.y << "  " << test.z << "  " << this->name << std::endl;
		//auto tes2 = (*this->invertedBoneMat) * test;
		//std::cout << tes2.x<<"  "<< tes2.y <<"  "<< tes2.z << "  " << this->name << std::endl;
		//auto tes3 = newParentTransform * tes2;
		//std::cout << tes3.x << "  " << tes3.y << "  " << tes3.z << "  " << this->name << std::endl;
		*matrix = newParentTransform * (*this->invertedBoneMat) *  (*matrix);
		//auto tes4 = *matrix * test;
		const float *pSource = (const float*)glm::value_ptr(*matrix);
		glUniformMatrix4fv(boneUnifArray[this->index], 1, GL_FALSE, pSource);
		delete matrix;
	}

    for (std::list<std::unique_ptr<Bone>>::iterator it = this->children.begin() ; it != this->children.end(); ++it){

        (*it)->modifyBoneUnif(boneUnifArray, newParentTransform);

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
		//glm::quat old = (*it)->getQuaternion();
		//(*it)->setQuaternion(rotation * old);
		//(*it)->rotateLocationAroundAPointForFrame(location, rotation);
		(*it)->applyChildrenTransformation(location, rotation);

	}
}
}
