#include "DrawingContext.h"

namespace engine{
	DrawingContext::DrawingContext(Camera * camera, Light * lights[]) :activeCamera(camera)
	{
		lightsNumber = 0;
		for (int i = 0; i < 4; i++){
			if (lights[i] != NULL){
				activeLights[i] = lights[i];
				lightsNumber++;
			}
			else{
				break;
			}

		}
	}


	DrawingContext::~DrawingContext()
	{
	}
}