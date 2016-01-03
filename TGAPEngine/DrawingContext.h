#pragma once
#include "Camera.h"
#include "Light.h"
#include <iostream>
#include "Config.h"
namespace engine{
	class DrawingContext
	{
	public:
		DrawingContext(Camera * camera, Light * light);
		virtual ~DrawingContext();
		Camera * activeCamera;
		Light * activeLight;

	};
}
