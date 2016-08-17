#include "Light.h"

namespace engine{
	Light::Light(vec4 direction, vec3 color, vec4 location, bool isDirectional) :direction(direction), color(color), location(location), m_isDirectional(isDirectional)
	{
		//ctor
	}
	Light::Light(float xdir, float ydir, float zdir, float r, float g, float b, float x, float y, float z, bool isDirectional)
	{
		this->direction = vec4(xdir, ydir, zdir, 0.0f);
		this->color = vec3(r, g, b);
		this->location = vec4(x, y, z, 1.0f);
		this->m_isDirectional = isDirectional;

	}
	Light::~Light()
	{
		//dtor
	}


}