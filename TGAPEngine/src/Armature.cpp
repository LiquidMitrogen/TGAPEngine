#include "Armature.h"

namespace engine{



	Armature::~Armature()
	{
	}

	void Armature::applyRotationToBone(Bone * bone, glm::quat quaternion){
		bone->applyArmatureRotation(quaternion);
	}

	void Armature::modifyArmatureUniforms(GLint * boneUniforms){
		this->rootBone->modifyBoneUnif(boneUniforms);
	}


	Bone * Armature::findBoneByName(std::string boneName){
		return this->rootBone->findBoneByName(boneName);
	}
	Action * Armature::getActionByName(std::string actionName){
		for (std::vector<Action>::iterator it = this->actions.begin(); it != this->actions.end(); it++){
			if ((*it).name == actionName) return &(*it);
		}
		return nullptr;
	}
	void Armature::applyActionByName(std::string actionName, unsigned int frame){
		//BUG!
		for (std::vector<Action>::iterator it = this->actions.begin(); it != this->actions.end(); it++){
			this->rootBone->applyAction(&(*it), frame);
		}
	}
	void Armature::applyAction(Action * action, unsigned int frame){
		this->rootBone->applyAction(action, frame);
	}


}