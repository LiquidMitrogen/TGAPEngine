#pragma once
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

namespace engine{
	using namespace glm;
	class Light
	{
	public:
		Light(vec4 direction, vec3 color, vec4 location, bool isDirectional);
		Light(float xdir, float ydir, float zdir, float r, float g, float b, float x, float y, float z, bool isDirectional);
		virtual ~Light();

		void setDirection(vec4 dir){ this->direction = dir; };
		void setColor(vec3 color){ this->color = color; };
		void setLocation(vec4 loc){ this->location = loc; };

		vec4 getDirection(){ return this->direction; };
		vec3 getColor(){ return this->color; };
		vec4 getLocation(){ return this->location; };
		bool isDirectional(){ return this->m_isDirectional; };

	protected:
	private:
		bool m_isDirectional;
		vec4 direction;
		vec3 color;
		vec4 location;

	};
}