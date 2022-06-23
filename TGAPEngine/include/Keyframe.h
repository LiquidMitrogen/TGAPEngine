#pragma once
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
namespace engine{
	template <class T, class V>
	class Keyframe
	{
	public:
	Keyframe(T time, V value) : time(time), value(value) {}
	~Keyframe() {}

		T time;
		V value;
	};
}
