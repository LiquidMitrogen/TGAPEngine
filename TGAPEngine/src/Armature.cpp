#include "Armature.h"

namespace engine{



	Armature::~Armature()
	{
	}

	void Armature::applyRotationToBone(Bone * bone, glm::quat quaternion){
		bone->applyArmatureRotation(quaternion);
	}

	void Armature::modifyArmatureUniforms(GLint * boneUniforms){
		this->rootBone->modifyBoneUnif(boneUniforms, glm::mat4(1.0f));
	}


	Bone * Armature::findBoneByName(std::string boneName){
		return this->rootBone->findBoneByName(boneName);
	}
	Action<float>* Armature::getActionByName(std::string actionName){
		for (std::vector<Action<float>*>::iterator it = this->actions.begin(); it != this->actions.end(); it++){
			if ((*it)->name == actionName) return (*it);
		}
		return nullptr;
	}
	void Armature::applyActionByName(std::string actionName, float time){
		//BUG!
		for (std::vector<Action<float>*>::iterator it = this->actions.begin(); it != this->actions.end(); it++){
			if ((*it)->name == actionName)
			{
				this->rootBone->applyAction((*it), time);
				return;
			}
		}
	}
	void Armature::applyAction(Action<float>* action, float time){
		this->rootBone->applyAction(action, time);
	}


}