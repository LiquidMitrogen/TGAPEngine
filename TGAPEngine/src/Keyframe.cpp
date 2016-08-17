#include "Keyframe.h"

namespace engine{
	Keyframe::Keyframe(unsigned int frame, glm::vec3 location, glm::quat quaternion)
	{
		this->frame = frame;
		this->location = location;
		this->quaternion = quaternion;
	}


	Keyframe::~Keyframe()
	{
	}
}