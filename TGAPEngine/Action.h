#pragma once
#include <vector>
#include <iostream>
#include <string>
#include "Keyframe.h"
namespace engine{
	struct ActionBoneKeyframes{
		unsigned int boneID;
		std::vector<Keyframe> keyframes;
	};
	class Action
	{
	public:
		Action(std::string name, unsigned int rangeStart, unsigned int rangeEnd);
		~Action();
		void addBoneKeyframes(ActionBoneKeyframes boneKeyframe);
		Keyframe getQuatLoc(unsigned int bone, unsigned int frame);//returns keyframe with frame number or interpolated data between keyframes when none found
		unsigned int rangeStart, rangeEnd;
		std::string name;
	protected:
		
		std::vector<ActionBoneKeyframes> boneKeyframes;
	};
	

}