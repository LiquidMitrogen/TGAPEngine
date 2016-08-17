#include "Action.h"
namespace engine{

	Action::Action(std::string name, unsigned int rangeStart, unsigned int rangeEnd)
	{
		this->name = name;
		this->rangeStart = rangeStart;
		this->rangeEnd = rangeEnd;
	}


	Action::~Action()
	{
	}

	void Action::addBoneKeyframes(ActionBoneKeyframes boneKeyframe){
		this->boneKeyframes.push_back(boneKeyframe);
	}

	Keyframe Action::getQuatLoc(unsigned int bone, unsigned int frame){
		for (std::vector<ActionBoneKeyframes>::iterator it = this->boneKeyframes.begin(); it != this->boneKeyframes.end(); it++){
			if (it->boneID == bone){
				if (it->keyframes.size() == 0){
					std::cout << "no keyframes" << std::endl;
					__debugbreak();
					Keyframe empty(frame, glm::vec3(0.0f, 0.0f, 0.0f), glm::quat(1.0f, 0.0f, 0.0f, 0.0f));
					return empty;
				}
				if (it->keyframes.size() == 1){
					return it->keyframes[0];
				}
				Keyframe * last = nullptr;
				for (std::vector<Keyframe>::iterator kit = it->keyframes.begin(); kit != it->keyframes.end(); kit++){
					if (kit->frame >= frame){
						if (kit->frame == frame){
							return *kit;
						}
						else{
							if (kit == it->keyframes.begin()){
								std::cout << "chosen frame was before first keyframe" << std::endl;
								__debugbreak();
								return *kit;
							}
							float interpRatio = (float)(frame - last->frame) / (float)((*kit).frame - last->frame);

							glm::vec3 vec3Interpolated = glm::mix(last->location, (*kit).location, interpRatio);
							glm::quat quatInterpolated = glm::slerp(last->quaternion, (*kit).quaternion, interpRatio);
							Keyframe interpolated(frame, vec3Interpolated, quatInterpolated);
							return interpolated;
						}
					}
					last = &(*kit);
				}
				std::cout << "chosen frame was after last keyframe" << std::endl;
				__debugbreak();
				return *(it->keyframes.end());
			}
		}
		std::cout << "Action does not have animation for wanted bone" << std::endl;
		Keyframe empty(frame, glm::vec3(0.0f, 0.0f, 0.0f), glm::quat(1.0f, 0.0f, 0.0f, 0.0f));
		__debugbreak();
		return empty;
	}

}