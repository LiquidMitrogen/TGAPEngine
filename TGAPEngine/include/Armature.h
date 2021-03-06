#pragma once

//#define GLEW_STATIC
#include <GL/glew.h>

#include"Entity.h"
#include "Bone.h"
#include "Action.h"

#include <vector>
namespace engine{
	class Armature : public Entity
	{
	public:
		Armature(Bone * rootBone) :Entity(nullptr),rootBone(rootBone){};
		~Armature();

		void addAction(Action action){

			this->actions.push_back(action);
		}

		Bone * findBoneByName(std::string boneName);
		Action * getActionByName(std::string actionName);
		void applyActionByName(std::string actionName, unsigned int frame);
		void applyAction(Action * action, unsigned int frame);

		void applyRotationToBone(Bone * bone, glm::quat quaternion);
		void modifyArmatureUniforms(GLint * boneUniforms);
	protected:
		std::vector<Action> actions;
		Bone * rootBone;
	};
}
