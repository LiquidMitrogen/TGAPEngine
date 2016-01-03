#include "DrawingContext.h"

namespace engine{
	DrawingContext::DrawingContext(Camera * camera, Light * light) :activeCamera(camera), activeLight(light)
	{
	}


	DrawingContext::~DrawingContext()
	{
	}
}