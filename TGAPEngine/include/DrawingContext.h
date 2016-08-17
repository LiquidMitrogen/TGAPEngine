#pragma once
#include "Camera.h"
#include "Light.h"
#include <iostream>
#include "Config.h"
namespace engine{
	class DrawingContext
	{
	public:
		DrawingContext(Camera * camera, Light * lights[]);
		virtual ~DrawingContext();
		void copyLights(Light * lights[]){
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
		Camera * activeCamera;
		Light * activeLights[4];
		unsigned int lightsNumber = 1;

	};
}
