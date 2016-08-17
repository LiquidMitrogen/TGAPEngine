#pragma once
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include<iostream>
#include"Config.h"
namespace engine{
	using namespace glm;
	class Camera
	{
	public:
		Camera(mat4 toProjectionMatrix, vec3 location, vec3 lookAtTarget, vec3 upDirection);
		static Camera * perspectiveCamera(float fov, float screenRatio, float nearZ, float farZ, float x, float y, float z, float targetx, float targety, float targetz, float upx, float upy, float upz);
		virtual ~Camera();

		void setLocation(vec3 location);
		vec3 getLocation();
		void applyTranslation(vec3 translationVector);
		void setTarget(vec3 newTarget);
		vec3 getTarget();
		void setUpDirection(vec3 newUpDirection);
		vec3 getUpDirection();
		void updateWorldToClipMatrix();

		mat4 getWorldToClipMatrix();
		mat4 getWorldToCameraMatrix();
		mat4 getCameraToClipMatrix();
		vec3 getMouseOnNearPlaneWorldSpace(float mouseXClipSpace, float mouseYClipSpace);
		vec3 getMouseOnFarPlaneWorldSpace(float mouseXClipSpace, float mouseYClipSpace);
		void setNearFar(float nearP, float farP){
			this->farPlane = farP;
			this->nearPlane = nearP;
		};
		void setWorldToClipMatrix(glm::mat4 cameraToClip, glm::mat4 worldToCamera);
	protected:
	private:
		float farPlane, nearPlane;
		mat4 cameraToClip;//
		mat4 worldToClip;//
		mat4 worldToCamera;
		vec3 location, target, upDirection;
		bool needsUpdate;
	};
}