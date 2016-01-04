#pragma once
#include <vector>
#include <memory>
#include "Entity.h"
#include "Config.h"
#include "SingleTextureMatrixMaterial.h"
#include "ActorTextureMatrixMaterial.h"
//#include "ShadowPassMaterial.h"
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define GLFW_DLL
#include <GLFW/glfw3.h>
#include "Camera.h"
#include "Light.h"
#include <string>
#include "AnimatedActor.h"
#include "Window.h"
#include "DrawingContext.h"
#include "Scene.h"

//#include "ParticleSystem.h"
namespace engine{
	class Renderer
	{
	public:
		Renderer(Camera * drawingPassCamera, Light * light);
		virtual ~Renderer();
		//void addEntity(Entity *e);
		//void removeEntity(Entity *e);
		void setActiveScene(Scene * scene){ this->activeScene = scene; };
		//void shadowMapPass();
		void drawingPass();
		//void testRender();
		//void testParticles(ActorTextureMatrixMaterial * mat, ParticleSystem * particleSystem);
		static glm::mat4 worldToCameraMatrix;
		Camera * getActiveCamera(){
			return drawPassCamera;
		};
		Light * getLight(){
			return tmp1;
		};
	protected:
	private:
		void makeSMFBO();
		const bool disableSmadowMapping = true;


		Scene * activeScene;

		DrawingContext * context;

		Light *tmp1, *tmp2;

		glm::vec3 perspectivePoints[10];

		glm::mat4 calculateOrthoProjection();// calculates orthogonal projection matrix from projection matrix far and near values
		Camera * drawPassCamera;
		Camera * shadowPassCamera;
		GLuint shadowMapFBO = 0;
		GLuint shadowMapTex = 0;
		GLuint shadowMapSampler = 0;
		Entity * ent;
	};
}
