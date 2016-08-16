#pragma once
#include "Entity.h"
#include "VertexAttributes.h"
#include <vector>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define GLEW_STATIC
#include <GL/glew.h>
#include <cstdlib>
#include <ctime>

#define TG_RANDOM ((float)rand() / (float)RAND_MAX)

const double PI = 3.141592;
namespace engine{
	class ParticleSystemCallbacks{
	public:
		ParticleSystemCallbacks(){};
		virtual ~ParticleSystemCallbacks(){};
		virtual void startParticle(struct Particle * particle) = 0;
		virtual void simulateParticle(std::vector<struct Particle> * particles,float timePassed) = 0;
	};
	struct Particle{
		glm::vec4 posSize;
		glm::vec3 velocity;
		float life;
		float seed;


	};

	class FountainPSCallbacks : public ParticleSystemCallbacks{
	public:
		FountainPSCallbacks(){ srand(time(NULL)); };
		virtual void startParticle(struct Particle * particle);
		virtual void simulateParticle(std::vector<struct Particle> * particles, float timePassed);
	protected:

	};

	class MagicPSCallbacks : public ParticleSystemCallbacks{
	public:
		MagicPSCallbacks(){ srand(time(NULL)); };
		virtual void startParticle(struct Particle * particle);
		virtual void simulateParticle(std::vector<struct Particle> * particles, float timePassed);
	protected:
		float hornlen = 0.5f;
		float hornwidth = 0.08f;

	};
	class ConfettiPSCallbacks : public ParticleSystemCallbacks{
	public:
		ConfettiPSCallbacks(){ srand(time(NULL)); };
		virtual void startParticle(struct Particle * particle);
		virtual void simulateParticle(std::vector<struct Particle> * particles, float timePassed);
		float confettiSize = 0.2;
		glm::vec3 gravityVec = glm::vec3(0.0f, -1.0f, 0.0f);
		float gravityAcc = 1.0f;
		float velocity = 1.0f;
	protected:
		

	};

	class ParticleSystem : public Entity
	{
	public:
		static ParticleSystem * createSquareParticleSystem(unsigned int maxParticles, ParticleSystemCallbacks * callbacks);
		ParticleSystem(VertexAttributes * va, unsigned int maxParticles, ParticleSystemCallbacks * callbacks);
		ParticleSystem(VertexAttributes * va, unsigned int maxParticles, ParticleSystemCallbacks * callbacks,unsigned int vaOffset);
		virtual ~ParticleSystem();
		virtual Entity * makeDuplicate();
		
		virtual void prepare();
		virtual void draw(glm::mat4 parentMatrix, DrawingContext * context);

		virtual void simulate(float milisecondsPassed);
		virtual void start(unsigned int startParticleNum);
		virtual void pause(bool paused);
		virtual void stop();
		virtual void setTimeBetweenParticles(float sec){ this->secBetweenParticles = sec; };
		
		unsigned int getParticleCount(){ return this->particleCount; };
		

	protected:
		float partialTime = 0;
		//unsigned int findFree();
		unsigned int lastIndex = 0;
		float secBetweenParticles = 0.1;
		//virtual void
		unsigned int vaOffset = 0;
		bool needsBufferUpdate = true;
		bool paused = false;
		unsigned int particleCount = 0;
		void updateBuffers();
		ParticleSystemCallbacks * callbacks;
		//GLuint vertex_buffer;
		//GLuint particles_position_buffer;
		unsigned int maxParticles;
		struct Particle templateParticle;
		std::vector<struct Particle> particles;
		bool running = true;

	};

}