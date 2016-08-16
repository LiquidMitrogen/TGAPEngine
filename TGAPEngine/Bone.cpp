#include "Bone.h"
namespace engine{
Bone::Bone(BoneStruct bones[], unsigned int thisIndex):index(thisIndex)
{
    unsigned int i;
    BoneFrameMatrix * frameTmp;
    Bone * boneTmp;
    this->invertedBoneMat = bones[index].invMat;
	this->name = std::string(bones[index].name);
	std::cout << this->name << std::endl;
	for (int i = 0; i < 4; i++){
		std::cout << (*invertedBoneMat)[i].x << " " << (*invertedBoneMat)[i].y << " " << (*invertedBoneMat)[i].z << " " << (*invertedBoneMat)[i].w << " " << std::endl;
	}
	
    for(i=0;i<bones[index].framesCount;i++){
        frameTmp = new BoneFrameMatrix(bones[index].frames[i].quaternion,bones[index].frames[i].location,bones[index].frames[i].rangeMin,bones[index].frames[i].rangeMax,bones[index].frames[i].index);
		if (DEBUG_MODE == 1){
			std::cout << "new Bone" << std::endl;
		}
        frameMatrices.emplace_back(frameTmp);
    }
    for(i=0;i<bones[index].childrenCount;i++){
        boneTmp = new Bone(bones,bones[index].childrenIndices[i]);
		if (DEBUG_MODE == 1){
			std::cout << "new Bone" << std::endl;
		}
        children.emplace_back(boneTmp);
    }
    //std::cout<<frameMatrices.size()<<std::endl;
    //std::cout<<children.size()<<std::endl<<std::endl;
}
QuatLoc * Bone::getMatrixForFrame(unsigned int frame)
{
    unsigned int i,mati;
    mati=0;
    glm::quat * quaternionsInRange;
    glm::vec3 * locationsInRange;
    quaternionsInRange = new glm::quat[3];
    locationsInRange = new glm::vec3[3];
	if (DEBUG_MODE == 1){
		std::cout << "new Bone" << std::endl;
	}
    unsigned int indexesInRange[3];
    for (std::vector<std::unique_ptr<BoneFrameMatrix>>::iterator it = this->frameMatrices.begin() ; it != this->frameMatrices.end(); ++it){
        if((*it)->inRange(frame)){
            if(mati<3){
                quaternionsInRange[mati]=(*it)->quaternion;
                locationsInRange[mati]=(*it)->location;
                indexesInRange[mati] = (*it)->index;
                mati++;
            }else{
                break;
            }
        }
    }
    if(mati==0){
            delete quaternionsInRange;
            delete locationsInRange;
      return NULL;
    }
    if(mati == 1){
            QuatLoc * result;
            result = new QuatLoc;
			if (DEBUG_MODE == 1){
				std::cout << "new Bone" << std::endl;
			}
            result->quaternion = quaternionsInRange[0];
            result->location = locationsInRange[0];
            delete quaternionsInRange;
            delete locationsInRange;
            return result;
    }
    if(mati == 2) {
        float interpScalar = ((frame - indexesInRange[0]) * 1.0f) / (indexesInRange[1] - indexesInRange[0]);
        //std::cout<<interpScalar<<std::endl;
        QuatLoc * result;
        result = new QuatLoc;
		if (DEBUG_MODE == 1){
			std::cout << "new Bone" << std::endl;
		}
        result->quaternion = glm::slerp(quaternionsInRange[0],quaternionsInRange[1],interpScalar);
        result->location = glm::mix(locationsInRange[0],locationsInRange[1],interpScalar);
        delete quaternionsInRange;
        delete locationsInRange;
        return result;
    }
    if(mati == 3){
        std::cout<<"too many matrices to interpolate"<<std::endl;
        delete [] quaternionsInRange;
        delete [] locationsInRange;
        return NULL;
    }
    delete [] quaternionsInRange;
    delete [] locationsInRange;
    return NULL;

}
void Bone::modifyBoneUnif(GLint* boneUnifArray, glm::mat4 * parentMatrix, glm::quat parentQuaternion,unsigned int frame)
{
    int i;
    QuatLoc * boneQuatLoc = (this->getMatrixForFrame(frame));
    if(boneQuatLoc == NULL) std::cout<<"NULL quatloc"<< frame <<std::endl;
    glm::mat4 * matrix = new glm::mat4(1.0f);
	if (DEBUG_MODE == 1){
		std::cout << "new Bone" << std::endl;
	}
    glm::vec4 location;
    glm::quat quaternion;
    location = glm::vec4(boneQuatLoc->location,1.0f);
    //quaternion = parentQuaternion * boneQuatLoc->quaternion;
    glm::mat4 locToOrigin, originToLoc, rotateMatrix;
    //locToOrigin = glm::translate(glm::mat4(1.0f),glm::vec3(-location.x,-location.y,-location.z));
    originToLoc = glm::translate(glm::mat4(1.0f),glm::vec3(location.x,location.y,location.z));
    rotateMatrix = glm::mat4_cast(boneQuatLoc->quaternion);
    *matrix =  originToLoc* rotateMatrix * (*this->invertedBoneMat) *  (*matrix);
    //*matrix =  rotateMatrix * originToLoc * (*matrix);
    const float *pSource = (const float*)glm::value_ptr(*matrix);
   // std::cout<<"index = "<<this->index<<std::endl;
   // for(i=0;i<16;i++){
    //    std::cout<<pSource[i]<<" ";
    //}
   // std::cout<<std::endl;
    //*matrix = *parentMatrix * (*matrix);
    //std::cout<<boneUnifArray[this->index]<<std::endl;
    //std::cout<<this->index<<std::endl;
    glUniformMatrix4fv(boneUnifArray[this->index],1,GL_FALSE,pSource);
    for (std::list<std::unique_ptr<Bone>>::iterator it = this->children.begin() ; it != this->children.end(); ++it){

        (*it)->modifyBoneUnif(boneUnifArray,matrix,quaternion,frame);

    }
    delete matrix;

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
glm::quat Bone::getQuaternionForFrame(int frame){
	for (std::vector<std::unique_ptr<BoneFrameMatrix>>::iterator it = this->frameMatrices.begin(); it != this->frameMatrices.end(); ++it){
				if ((*it)->index == frame){
					return (*it)->quaternion;
				}
		}
		std::cout << "BFM not found for frame " << frame << std::endl;
		return glm::quat(1.0f,0.0f,0.0f,0.0f);
}

void Bone::setQuaternionForFrame(int frame, glm::quat quaternion){
	for (std::vector<std::unique_ptr<BoneFrameMatrix>>::iterator it = this->frameMatrices.begin(); it != this->frameMatrices.end(); ++it){
		if ((*it)->index == frame){
			(*it)->quaternion = quaternion;
			return;
		}
	}
	std::cout << "BFM not found for frame " << frame << std::endl;
}


void Bone::rotateLocationAroundAPointForFrame(int frame, glm::vec3 location, glm::quat rotation){
	glm::vec3 oldloc = this->getLocationForFrame(frame);
	glm::vec4 translated = glm::vec4(oldloc.x - location.x, oldloc.y - location.y, oldloc.z - location.z, 1.0);
	glm::vec4 rotated = rotation * translated;
	glm::vec3 newLoc = glm::vec3(rotated.x + location.x, rotated.y + location.y, rotated.z + location.z);
	this->setLocationForFrame(frame, newLoc);
}

glm::vec3 Bone::getLocationForFrame(int frame){
	for (std::vector<std::unique_ptr<BoneFrameMatrix>>::iterator it = this->frameMatrices.begin(); it != this->frameMatrices.end(); ++it){
		if ((*it)->index == frame){
			return (*it)->location;
		}
	}
	std::cout << "BFM not found for frame " << frame << std::endl;
	return glm::vec3(0.0f, 0.0f, 0.0f);
}
void Bone::setLocationForFrame(int frame, glm::vec3 location){
	for (std::vector<std::unique_ptr<BoneFrameMatrix>>::iterator it = this->frameMatrices.begin(); it != this->frameMatrices.end(); ++it){
		if ((*it)->index == frame){
			(*it)->location = location;
			return;
		}
	}
	std::cout << "BFM not found for frame " << frame << std::endl;
}

void Bone::applyArmatureRotation(int frame, glm::quat rotation){

	glm::quat old = this->getQuaternionForFrame(frame);
	this->setQuaternionForFrame(frame,rotation * old);
	glm::vec3 pivot = this->getLocationForFrame(frame);

	applyChildrenTransformation(frame, pivot, rotation);

	


}
void Bone::applyChildrenTransformation(int frame, glm::vec3 location, glm::quat rotation){

	for (std::list<std::unique_ptr<Bone>>::iterator it = this->children.begin(); it != this->children.end(); ++it){

		glm::quat old = (*it)->getQuaternionForFrame(frame);
		(*it)->setQuaternionForFrame(frame, rotation * old);
		(*it)->rotateLocationAroundAPointForFrame(frame, location, rotation);
		(*it)->applyChildrenTransformation(frame, location, rotation);

	}
}
}
