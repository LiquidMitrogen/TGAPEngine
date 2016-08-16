#include "Camera.h"


namespace engine{

	Camera::Camera(mat4 toProjectionMatrix, vec3 location, vec3 lookAtTarget, vec3 upDirection) :
		cameraToClip(toProjectionMatrix), location(location), target(lookAtTarget), upDirection(upDirection), needsUpdate(true)
	{

	}
	Camera * Camera::perspectiveCamera(float fovangle, float screenRatio, float near, float far, float x, float y, float z, float targetx, float targety, float targetz, float upx, float upy, float upz)
	{
		Camera * cam = new Camera(glm::perspective(fovangle, screenRatio, near, far), glm::vec3(x, y, z), glm::vec3(targetx, targety, targetz), glm::vec3(upx, upy, upz));
		cam->setNearFar(near, far);
		return cam;
		if (DEBUG_MODE == 1){
			std::cout << "new Camera" << std::endl;
		}
	}
	void Camera::setLocation(vec3 location)
	{
		this->location = location;
		this->needsUpdate = true;
	}

	vec3 Camera::getLocation()
	{
		return this->location;
	}

	void Camera::applyTranslation(vec3 translationVector)
	{
		this->location = this->location + translationVector;
		this->needsUpdate = true;
	}

	void Camera::setTarget(vec3 newTarget)
	{
		this->target = newTarget;
		this->needsUpdate = true;
	}

	vec3 Camera::getTarget()
	{
		return this->target;
	}

	void Camera::setUpDirection(vec3 newUpDirection)
	{
		this->upDirection = newUpDirection;
		this->needsUpdate = true;
	}

	vec3 Camera::getUpDirection()
	{
		return this->upDirection;
	}

	void Camera::updateWorldToClipMatrix()
	{
		worldToCamera = glm::lookAt(
			this->location, // Camera is at location
			this->target, // and looks at the target
			this->upDirection  // Head is up upDirection
			);
		this->worldToClip = this->cameraToClip * worldToCamera;
		this->needsUpdate = false;
	}

	mat4 Camera::getWorldToClipMatrix()
	{
		if (needsUpdate) updateWorldToClipMatrix();
		return this->worldToClip;
	}
	mat4 Camera::getWorldToCameraMatrix()
	{
		if (needsUpdate) updateWorldToClipMatrix();
		return this->worldToCamera;
	}
	mat4 Camera::getCameraToClipMatrix()
	{
		if (needsUpdate) updateWorldToClipMatrix();
		return this->cameraToClip;
	}

	Camera::~Camera()
	{
		//dtor
	}
	vec3 Camera::getMouseOnNearPlaneWorldSpace(float mouseXClipSpace, float mouseYClipSpace){
		glm::vec4 mouseOnNearPlaneClipSpace = glm::vec4(mouseXClipSpace * nearPlane, mouseYClipSpace * nearPlane, -1.0f * nearPlane, nearPlane);
		glm::vec4 mouseOnNearPlaneWorld = inverse(this->getCameraToClipMatrix() * this->getWorldToCameraMatrix()) * mouseOnNearPlaneClipSpace;
		return glm::vec3(mouseOnNearPlaneWorld);
	}
	vec3 Camera::getMouseOnFarPlaneWorldSpace(float mouseXClipSpace, float mouseYClipSpace){
		glm::vec4 mouseOnFarPlaneClipSpace = glm::vec4(mouseXClipSpace * farPlane, mouseYClipSpace * farPlane, 1 * farPlane, farPlane);
		glm::vec4 mouseOnFarPlaneWorld = inverse(this->getCameraToClipMatrix() * this->getWorldToCameraMatrix()) * mouseOnFarPlaneClipSpace;
		return glm::vec3(mouseOnFarPlaneWorld);
	}
}
