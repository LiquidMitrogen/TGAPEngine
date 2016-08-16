#include "ParticleSystem.h"

namespace engine{

	ParticleSystem * ParticleSystem::createSquareParticleSystem(unsigned int maxParticles, ParticleSystemCallbacks * callbacks)
	{
		float array[] = {
			-0.5f, -0.5f, 0.0f,

			0.5f, -0.5f, 0.0f,

			-0.5f, 0.5f, 0.0f,

			0.5f, 0.5f, 0.0f
		};
		std::vector<char>buffer((char*)array, (char*)array + sizeof(array));
		struct VaoInitData vaoinit;
		vaoinit.attrNumber = 2;
		vaoinit.attrNormBool[0] = false; //vertices
		vaoinit.attrOffset[0] = 0;
		vaoinit.attrSize[0] = 3;
		vaoinit.attrType[0] = GL_FLOAT;

		vaoinit.attrNormBool[1] = false; //positions
		vaoinit.attrOffset[1] = sizeof(array);
		vaoinit.attrSize[1] = 4;
		vaoinit.attrType[1] = GL_FLOAT;
		VertexAttributes * va = new VertexAttributes(buffer,vaoinit, maxParticles * 4 * sizeof(float));
		
		return new ParticleSystem(va, maxParticles, callbacks, sizeof(array));
	}

	ParticleSystem::ParticleSystem(VertexAttributes * va, unsigned int maxParticles, ParticleSystemCallbacks * callbacks) :Entity(va)
	{
		this->callbacks = callbacks;
		this->maxParticles = maxParticles;
		this->particles.reserve(maxParticles);
		//ctor
	}
	ParticleSystem::ParticleSystem(VertexAttributes * va, unsigned int maxParticles, ParticleSystemCallbacks * callbacks,unsigned int vaOffset) :Entity(va)
	{
		this->vaOffset = vaOffset;
		this->callbacks = callbacks;
		this->maxParticles = maxParticles;
		this->particles.reserve(maxParticles);
		//ctor
	}
	Entity * ParticleSystem::makeDuplicate()
	{
		ParticleSystem * e = new ParticleSystem(this->vertexAttributeBuf,maxParticles,this->callbacks);
		if (DEBUG_MODE == 1){
			std::cout << "new Entity" << std::endl;
		}
		return e;
	}

	ParticleSystem::~ParticleSystem()
	{
	}

	void ParticleSystem::draw(glm::mat4 parentMatrix, DrawingContext * context)
	{
		this->prepare();
		Material * mat = this->entityMaterial;
		mat->setUniformLightsNumber(context->lightsNumber);
		for (int i = 0; i < context->lightsNumber; i++){
			mat->setUniformLightCol(context->activeLights[i]->getColor(), i);
			if (context->activeLights[i]->isDirectional()){
				mat->setUniformLightDir(context->activeLights[i]->getDirection(),i);
				mat->setUniformPointLight(false,i);
			}
			else{
				mat->setUniformPointLight(true,i);
				mat->setUniformLightDir(context->activeLights[i]->getLocation(),i);
			}
		}
		//mat->setUniformLightDir2(this->drawPassCamera->getWorldToProjectionMatrix() *this->tmp2->getDirection());
		//glUniform1i(((ActorTextureMatrixMaterial *)mat)->smUniform, 0);

		mat->setUniformWorldToCamera(context->activeCamera->getWorldToCameraMatrix());
		mat->setUniformCameraToClip(context->activeCamera->getCameraToClipMatrix());

		if (DEBUG_MODE == 1)
			std::cout << "Drawing glDrawElements" << std::endl;
		glm::mat4 combinedMatrix = parentMatrix * getTransformationMatrix();
		this->entityMaterial->setUniformModelToWorld(combinedMatrix);
		if (this->disableDepthTests){
			glDisable(GL_DEPTH_TEST);
		}

		glVertexAttribDivisor(0, 0); // particles vertices : always reuse the same 4 vertices -> 0
		glVertexAttribDivisor(1, 1); // positions : one per quad (its center) -> 1

		glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, particleCount);
		//glDrawElements(GL_TRIANGLES, vertexAttributeBuf->indiceCount, GL_UNSIGNED_INT, 0);

		if (this->disableDepthTests){
			glEnable(GL_DEPTH_TEST);
		}
		for (std::list<std::unique_ptr<Entity>>::iterator it = children.begin(); it != children.end(); ++it){
			(*it)->draw(combinedMatrix, context);
		}
		//std::cout<<"done"<<std::endl;
	}

	void ParticleSystem::prepare()
	{
		for (std::vector<std::unique_ptr<Image>>::iterator it = this->textures.begin(); it != this->textures.end(); ++it){
			(*it)->use();
		}
		if (this->needsBufferUpdate == true){
			this->needsBufferUpdate = false;
			this->updateBuffers();
		}
		this->vertexAttributeBuf->use();
		this->entityMaterial->use();
	}

	
	void ParticleSystem::updateBuffers(){
		struct floatvec {
			float x, y, z, s;
			bool operator<(struct floatvec& that){

				// Sort in reverse order : far particles drawn first.

				return this->z < that.z;

			}
		};
		std::vector<struct floatvec> buf;
		buf.reserve(maxParticles);
		particleCount = 0;
		for (std::vector<Particle>::iterator it = this->particles.begin(); it != this->particles.end(); ++it){
			if ((*it).life > 0){
				if (particleCount > maxParticles- 1) break;
				particleCount++;
				struct floatvec newPar;
				newPar.x = (*it).posSize.x;
				newPar.y = (*it).posSize.y;
				newPar.z = (*it).posSize.z;
				newPar.s = (*it).posSize.w;
				buf.push_back(newPar);
			}
		}
		if (buf.size() > 0){
			std::sort(buf.begin(), buf.end());
			this->vertexAttributeBuf->updateBuffer((char *)&buf.front(), this->vaOffset, buf.size() * sizeof(struct floatvec));
		}
			
	}



	void ParticleSystem::simulate(float timePassed)
	{
		if (!this->paused){

			unsigned int newParticlesNum = floor(((float)timePassed + this->partialTime) / secBetweenParticles);
			//std::cout << newParticlesNum << std::endl;
			this->partialTime = (timePassed + this->partialTime) - (newParticlesNum * secBetweenParticles);
			if (this->particles.size() >= maxParticles){
				for (int i = 0; i < newParticlesNum; i++){
					lastIndex = 0;
					for (int i = lastIndex; i < particles.size(); i++){
						if (particles[i].life <= 0){
							this->callbacks->startParticle(&particles[i]);
							break;
						}
						lastIndex = i;
					}
				}
			}
			else{
				for (int i = 0; i < newParticlesNum; i++){
					struct Particle newParticle;
					this->callbacks->startParticle(&newParticle);
					this->particles.push_back(newParticle);
				}
			}
			
			this->needsBufferUpdate = true;
			this->callbacks->simulateParticle(&this->particles, timePassed);
		}
		
	}
	void ParticleSystem::start(unsigned int startParticleNum)
	{
		this->particles.clear();
		this->paused = false;
		for (int i = 0; i < startParticleNum; i++){
			struct Particle newParticle;
			this->callbacks->startParticle(&newParticle);
			this->particles.push_back(newParticle);
		}
	}
	void ParticleSystem::pause(bool paused)
	{
		this->paused = paused;
	}
	void ParticleSystem::stop()
	{
		this->particles.clear();
		this->paused = true;
	}








	void FountainPSCallbacks::startParticle(struct Particle * particle){
		particle->life = 15.0f;//15 sekund
		particle->posSize = glm::vec4(0.0f, 0.0f, 0.0f, 0.3f);
		particle->velocity = glm::normalize(glm::vec3(0.2f * sin(((float)rand() / (float)RAND_MAX) * PI) - 0.1f, 1.0f + 1.0f * ((float)rand() / (float)RAND_MAX), 0.2f * cos(((float)rand() / (float)RAND_MAX) * PI) - 0.1f)) * 5.0f;

	}
	void FountainPSCallbacks::simulateParticle(std::vector<struct Particle> * particles, float timePassed){ 
		for (std::vector<Particle>::iterator it = particles->begin(); it != particles->end(); ++it){
			if ((*it).life > 0){
				(*it).life -= timePassed;
				(*it).posSize.x += (*it).velocity.x * timePassed;
				(*it).velocity.y -= 1.0f * timePassed;
				(*it).posSize.y += (*it).velocity.y * timePassed;
				(*it).posSize.z += (*it).velocity.z * timePassed;
			}
		}
	}

	void MagicPSCallbacks::startParticle(struct Particle * particle){
		particle->life = 2.5f;//15 sekund
		float random = ((float)rand() / (float)RAND_MAX);
		particle->seed = random;
		particle->posSize = glm::vec4(sin((random - 0.5) * PI) * hornwidth, 0.0f, cos((random - 0.5) * PI) * hornwidth, 0.2f);
		//particle->velocity = glm::normalize(glm::vec3(0.2f * sin(((float)rand() / (float)RAND_MAX) * PI) - 0.1f, 1.0f + 1.0f * ((float)rand() / (float)RAND_MAX), 0.2f * cos(((float)rand() / (float)RAND_MAX) * PI) - 0.1f)) * 5.0f;
		particle->velocity = glm::vec3(0.0f, 1.0f, 0.0f) * 0.2f;
	}
	void MagicPSCallbacks::simulateParticle(std::vector<struct Particle> * particles, float timePassed){
		for (std::vector<Particle>::iterator it = particles->begin(); it != particles->end(); ++it){
			if ((*it).life > 0){
				(*it).life -= timePassed;
				(*it).posSize.y += (*it).velocity.y * timePassed;
				glm::vec2 pos = glm::vec2(sin(((*it).seed - 0.5) * PI) * hornwidth, cos(((*it).seed - 0.5) * PI) * hornwidth);
				(*it).posSize.x = sin(((*it).posSize.y + 0.1 * hornlen) / (hornlen + 0.1f * hornlen) * PI) * pos.x;// +sin(((*it).seed - 0.5) * 2 * PI) * hornwidth;
				(*it).posSize.z = cos((*it).posSize.y / (hornlen + 0.1f * hornlen) * PI) * pos.y;// +cos(((*it).seed - 0.5) * 2 * PI) * hornwidth;
				//(*it).posSize.x += (*it).velocity.x * timePassed;
				//(*it).velocity.y -= 1.0f * timePassed;
				//(*it).posSize.y += (*it).velocity.y * timePassed;
				//(*it).posSize.z += (*it).velocity.z * timePassed;
			}
		}
	}

	void ConfettiPSCallbacks::startParticle(struct Particle * particle){
		particle->life = 25.5f;//15 sekund
		float random = ((float)rand() / (float)RAND_MAX);
		particle->seed = random;
		particle->posSize = glm::vec4(0.0f, TG_RANDOM * 0.2f, 0.0f, confettiSize);
		particle->velocity = glm::vec3((TG_RANDOM - 0.5) * 0.2f, 1.0f, (TG_RANDOM - 0.5) * 0.2f) * velocity;
		//particle->posSize = glm::vec4(sin((random - 0.5) * PI) * hornwidth, 0.0f, cos((random - 0.5) * PI) * hornwidth, 0.2f);
		//particle->velocity = glm::normalize(glm::vec3(0.2f * sin(((float)rand() / (float)RAND_MAX) * PI) - 0.1f, 1.0f + 1.0f * ((float)rand() / (float)RAND_MAX), 0.2f * cos(((float)rand() / (float)RAND_MAX) * PI) - 0.1f)) * 5.0f;
		//particle->velocity = glm::vec3(0.0f, 1.0f, 0.0f) * 0.2f;
	}
	void ConfettiPSCallbacks::simulateParticle(std::vector<struct Particle> * particles, float timePassed){
		glm::vec3 gravityVecMult = this->gravityVec * gravityAcc;
		for (std::vector<Particle>::iterator it = particles->begin(); it != particles->end(); ++it){
			if ((*it).life > 0){
				(*it).life -= timePassed;
				//(*it).posSize.y += (*it).velocity.y * timePassed;
				//glm::vec2 pos = glm::vec2(sin(((*it).seed - 0.5) * PI) * hornwidth, cos(((*it).seed - 0.5) * PI) * hornwidth);
				//(*it).posSize.x = sin(((*it).posSize.y + 0.1 * hornlen) / (hornlen + 0.1f * hornlen) * PI) * pos.x;// +sin(((*it).seed - 0.5) * 2 * PI) * hornwidth;
				//(*it).posSize.z = cos((*it).posSize.y / (hornlen + 0.1f * hornlen) * PI) * pos.y;// +cos(((*it).seed - 0.5) * 2 * PI) * hornwidth;
				(*it).velocity.x += gravityVecMult.x * timePassed;
				(*it).velocity.y += gravityVecMult.y * timePassed;
				(*it).velocity.z += gravityVecMult.z * timePassed;
				(*it).posSize.x += (*it).velocity.x * timePassed;
				//(*it).velocity.y -= 1.0f * timePassed;
				(*it).posSize.y += (*it).velocity.y * timePassed;
				(*it).posSize.z += (*it).velocity.z * timePassed;
			}
		}
	}
}