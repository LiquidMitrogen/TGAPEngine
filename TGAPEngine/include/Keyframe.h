#pragma once
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
namespace engine{
	class Keyframe
	{
	public:
		Keyframe(unsigned int index, glm::vec3 location, glm::quat quaternion);
		~Keyframe();

		unsigned int frame;
		glm::quat quaternion;
		glm::vec3 location;
	};
}
