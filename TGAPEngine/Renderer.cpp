#include "Renderer.h"

namespace engine{
	glm::mat4 Renderer::worldToCameraMatrix;
	Renderer::Renderer(Camera *drawingPassCamera, Light * light)
	{
		if (!disableSmadowMapping)
			makeSMFBO();
		this->drawPassCamera = drawingPassCamera;
		//this->drawPassCamera = new Camera(glm::perspective(1.182664626f, 9.0f / 6.0f, 1.0f, 750.0f), glm::vec3(1.0f, 0.0f, 15.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		//this->calculateOrthoProjection();
		//this->shadowPassCamera = new Camera(glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.0f, 100.0f), glm::vec3(-5.0f, 0.0f, 15.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		this->shadowPassCamera = NULL;
		this->tmp1 = light;
		this->context = new DrawingContext(drawingPassCamera, light);
		if (DEBUG_MODE == 1){
			std::cout << "new renderer" << std::endl;
		}
		//this->tmp1 = new DirectionalLight(vec4(0.0, -1.0, 0.0, 0.0), vec3(1.0, 1.0, 1.0), vec4(0.0, 10.0, 0.0, 1.0));
		//this->tmp2 = new DirectionalLight(vec4(0.0, 1.0, -1.0, 0.0), vec3(1.0, 1.0, 1.0), vec4(0.0, 0.0, 10.0, 1.0));
		//test

		// zNear -----------------------------------
		perspectivePoints[0] = glm::vec3(-1.0, 0.0, -1.0);
		perspectivePoints[1] = glm::vec3(1.0, 0.0, -1.0);
		perspectivePoints[2] = glm::vec3(0.0, -1.0, -1.0);
		perspectivePoints[3] = glm::vec3(0.0, 1.0, -1.0);
		// zFar
		perspectivePoints[4] = glm::vec3(-1.0, 0.0, 1.0);
		perspectivePoints[5] = glm::vec3(1.0, 0.0, 1.0);
		perspectivePoints[6] = glm::vec3(0.0, -1.0, 1.0);
		perspectivePoints[7] = glm::vec3(0.0, 1.0, 1.0);

		perspectivePoints[9] = glm::vec3(0.0, 0.0, -73.0);
		perspectivePoints[9] = glm::vec3(0.0, 0.0, 1.0);

		//const float vertexPos[]{
		//	-0.5, -1.0, -2.8,
		//		1.5, -1.0, -2.8,
		//		1.5, 1.0, -2.8,
		//		-0.5, 1.0, -2.8,

		//		//texcoords

		//		0.0, 0.0,
		//		1.0, 0.0,
		//		1.0, 1.0,
		//		0.0, 1.0
		//};

		//const unsigned int indicesTex[]{
		//	0, 1, 2,
		//		0, 2, 3
		//};



		//std::vector<char> poses((char *)vertexPos, ((char *)vertexPos) + sizeof(vertexPos));
		//std::vector<unsigned int> indyki(indicesTex, indicesTex + sizeof(indicesTex) / sizeof(unsigned int));
		//engine::VaoInitData vid2;

		////renderer.makeSMFBO();


		//vid2.attrNumber = 2;
		//vid2.attrNormBool[0] = GL_FALSE;
		//vid2.attrOffset[0] = 0;
		//vid2.attrSize[0] = 3;
		//vid2.attrType[0] = GL_FLOAT;

		//vid2.attrNormBool[1] = GL_FALSE;
		//vid2.attrOffset[1] = 4 * 3 * sizeof(float);
		//vid2.attrSize[1] = 2;
		//vid2.attrType[1] = GL_FLOAT;
		//engine::VertexAttributes * vaa;
		//vaa = new VertexAttributes(poses, indyki, vid2);
		//this->ent = new engine::Entity(vaa);
		//engine::Material * mat = new SingleTextureMatrixMatrial("texture.vert", "texturesm.frag");



		//this->ent->entityMaterial = mat;
		//this->ent->entityMaterial->use();
		//((SingleTextureMatrixMatrial *)(this->ent->entityMaterial))->setUniformSampler(1);
	}

	Renderer::~Renderer()
	{
		//dtor
	}
	void Renderer::addEntity(Entity *e)
	{
		entities.emplace_back(e);
	}
	/*void Renderer::shadowMapPass()
	{
		double time = glfwGetTime();
		this->shadowPassCamera->setLocation(glm::vec3(sin(time * 2.4) * 15, 0.0f, cos(time * 2.4) * 15));
		this->tmp1->setLocation(glm::vec4(this->shadowPassCamera->getLocation(), 1.0f));
		vec3 dir = this->shadowPassCamera->getTarget() - this->shadowPassCamera->getLocation();
		dir = glm::normalize(dir);
		this->tmp1->setDirection(glm::vec4(dir, 0.0f));
		glCullFace(GL_FRONT);
		glEnable(GL_CULL_FACE);
		glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		//--------------------------------------------------------------------------------------------------
		for (std::vector<std::unique_ptr<Entity>>::iterator it = entities.begin(); it != entities.end(); ++it)

		{
			if ((*it)->makesShadow == false) continue;
			if (DEBUG_MODE == 1)
				std::cout << "using VertexAttributes" << std::endl;
			(*it)->getVertexAttributes()->use();
			//if(DEBUG_MODE == 1)
			//std::cout<<"using Materials"<<std::endl;
			// (*it)->entityMaterial->use();
			Material * mat = (*it)->entityMaterial;
			mat->use();
			//((ActorTextureMatrixMaterial *)mat)->setUniformLightDir(this->tmp1->getDirection());
			//((ActorTextureMatrixMaterial *)mat)->setUniformLightDir2(this->tmp2->getDirection());
			((ActorTextureMatrixMaterial *)mat)->setUniformToLightMatrix(this->shadowPassCamera->getWorldToProjectionMatrix() * (*it)->getTransformationMatrix());
			//   (*it)->resetLocation();
			(*it)->resetRotation();
			//((ShadowPassMaterial *)mat)->setUniformModelMatrix((*it)->getTransformationMatrix());
			//((ActorTextureMatrixMaterial *)mat)->setUniformModelToLightMatrix((*it)->getTransformationMatrix());
			glUniform1i(((ActorTextureMatrixMaterial *)mat)->smUniform, 1);


			glDrawElements(GL_TRIANGLES, (*it)->getVertexAttributes()->indiceCount, GL_UNSIGNED_INT, 0);

			////--------------------------------------------------------------------------------------------------
			//   (*it)->resetLocation();
			(*it)->resetRotation();
			time = glfwGetTime();
			(*it)->applyTranslation(glm::vec3(sin(time) * 12, sin(time * 10) * 2, 0.0));
			//  (*it)->applyRotation(glm::quat(glm::vec3(time * 1.63,0.0,0.0)));
			(*it)->applyRotation(qm::Quaternion::fromAxisAngle(time * 1.63f, 1.0, 0.0, 0.0f));
			((ActorTextureMatrixMaterial *)mat)->setUniformToLightMatrix(this->shadowPassCamera->getWorldToProjectionMatrix() * (*it)->getTransformationMatrix());


			glDrawElements(GL_TRIANGLES, (*it)->getVertexAttributes()->indiceCount, GL_UNSIGNED_INT, 0);
			//std::cout<<"done"<<std::endl;
			//-----------------------------------------------------------------------------------------------
			(*it)->applyTranslation(glm::vec3(-sin(time) * 12, -sin(time * 10) * 2, 0.0));
			
		}
		glCullFace(GL_BACK);
		glDisable(GL_CULL_FACE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}*/
	void Renderer::drawingPass()
	{

		//vec4 windowLoc = vec4(0.0f, 0.0f, 1.5f, 1.0f);
		//vec4 lookAtPoint = vec4(0.0f, 0.0f, 2.5f, 1.0f);
		//vec4 upDir = vec4(0.0f, 1.0f, 0.0f, 0.0f);

		//std::string spaceshipName = "myspaceship";

		/*for (std::vector<std::unique_ptr<Entity>>::iterator it = entities.begin(); it != entities.end(); ++it){
			if ((*it)->name == spaceshipName){
				mat4 matt = (*it)->getTransformationMatrix();
				windowLoc = matt * windowLoc;
				lookAtPoint = matt * lookAtPoint;
				upDir = matt * upDir;
				// std::cout<<windowLoc.x<<" "<<windowLoc.y<<" "<<windowLoc.z<<std::endl;
				//std::cout<<lookAtPoint.x<<" "<<lookAtPoint.y<<" "<<lookAtPoint.z<<std::endl;
				//std::cout<<upDir.x<<" "<<upDir.y<<" "<<upDir.z<<std::endl;
				// std::cout<<std::endl;
			}
		}*/



		// double time = glfwGetTime();
		//this->drawPassCamera->setLocation(glm::vec3(windowLoc.x, windowLoc.y, windowLoc.z));
		//this->drawPassCamera->setTarget(glm::vec3(lookAtPoint.x, lookAtPoint.y, lookAtPoint.z));
		//this->drawPassCamera->setUpDirection(glm::vec3(upDir.x, upDir.y, upDir.z));
		glm::mat4 dpmat = drawPassCamera->getWorldToClipMatrix();
		Renderer::worldToCameraMatrix = dpmat;
		/*for (int i = 0; i<4; i++){
			if (std::isinf(dpmat[i].x)){
				std::cout << "glm mtw inf" << std::endl;
				exit(-2);
			}
			if (std::isnan(dpmat[i].x)){
				std::cout << "glm mtw nan" << std::endl;
				exit(-2);
			}

			if (std::isinf(dpmat[i].y)){
				std::cout << "glm mtw inf" << std::endl;
				exit(-2);
			}
			if (std::isnan(dpmat[i].y)){
				std::cout << "glm mtw nan" << std::endl;
				exit(-2);
			}

			if (std::isinf(dpmat[i].z)){
				std::cout << "glm mtw inf" << std::endl;
				exit(-2);
			}
			if (std::isnan(dpmat[i].z)){
				std::cout << "glm mtw nan" << std::endl;
				exit(-2);
			}

			if (std::isinf(dpmat[i].w)){
				std::cout << "glm mtw inf" << std::endl;
				exit(-2);
			}
			if (std::isnan(dpmat[i].w)){
				std::cout << "glm mtw nan" << std::endl;
				exit(-2);
			}
		}*/

		//  this->shadowPassCamera->setLocation(glm::vec3(sin(time * 2.4)*25,0.0f,cos(time * 2.4)*25));
		//  this->tmp1->setLocation(glm::vec4(this->shadowPassCamera->getLocation(),1.0f));
		//  vec3 dir = this->shadowPassCamera->getTarget() - this->shadowPassCamera->getLocation();
		//   dir = glm::normalize(dir);
		//  this->tmp1->setDirection(glm::vec4(dir,0.0f));



		glCullFace(GL_BACK);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// glActiveTexture(GL_TEXTURE0+1);
		//glBindTexture(GL_TEXTURE_2D,this->shadowMapTex);
		//glBindSampler(1,this->shadowMapSampler);

		for (std::vector<std::unique_ptr<Entity>>::iterator it = entities.begin(); it != entities.end(); ++it)

		{

			//(*it)->prepare();
			//Material * mat = (*it)->entityMaterial;

			////((ActorTextureMatrixMaterial *)mat)->setUniformSampler(1);
			////((ActorTextureMatrixMaterial *)mat)->setUniformLightDir(this->drawPassCamera->getWorldToProjectionMatrix() * this->tmp1->getDirection());
			////((ActorTextureMatrixMaterial *)mat)->setUniformLightDir2(this->drawPassCamera->getWorldToProjectionMatrix() *this->tmp2->getDirection());
			//if (this->tmp1->isDirectional()){
			//	mat->setUniformLightDir(this->tmp1->getDirection());
			//	mat->setUniformPointLight(false);
			//}
			//else{
			//	mat->setUniformPointLight(true);
			//	mat->setUniformLightDir(this->tmp1->getLocation());
			//}
			//
			////mat->setUniformLightDir2(this->drawPassCamera->getWorldToProjectionMatrix() *this->tmp2->getDirection());
			////glUniform1i(((ActorTextureMatrixMaterial *)mat)->smUniform, 0);

			//mat->setUniformWorldToCamera(this->drawPassCamera->getWorldToCameraMatrix());
			//mat->setUniformCameraToClip(this->drawPassCamera->getCameraToClipMatrix());
			(*it)->draw(glm::mat4(1.0f),context);
			//glDrawElements(GL_TRIANGLES,(*it)->getVertexAttributes()->indiceCount,GL_UNSIGNED_INT,0);
			//   (*it)->resetRotation();
			//   (*it)->resetLocation();
			//   (*it)->applyTranslation(glm::vec3(4.0f,12.0f,-7.0f));
			//   (*it)->applyRotation(glm::quat(glm::vec3(time * 1.63,0.0,0.0)));
		}
	}
	/*
	void Renderer::testRender(){
		glActiveTexture(GL_TEXTURE0 + 1);
		glBindTexture(GL_TEXTURE_2D, this->shadowMapTex);
		glBindSampler(1, this->shadowMapSampler);
		this->ent->getVertexAttributes()->use();
		this->ent->entityMaterial->use();
		glDrawElements(GL_TRIANGLES, this->ent->getVertexAttributes()->indiceCount, GL_UNSIGNED_INT, 0);
	}*/
	void Renderer::makeSMFBO()
	{
		glGenFramebuffers(1, &shadowMapFBO);
		glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);

		glGenSamplers(1, &shadowMapSampler);
		glSamplerParameteri(this->shadowMapSampler, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glSamplerParameteri(this->shadowMapSampler, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glSamplerParameteri(this->shadowMapSampler, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glSamplerParameteri(this->shadowMapSampler, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		GLuint renbuf;
		glGenRenderbuffers(1, &renbuf);
		glBindRenderbuffer(GL_RENDERBUFFER, renbuf);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, Window::m_width, Window::m_height);
		glGenTextures(1, &shadowMapTex);
		glBindTexture(GL_TEXTURE_2D, shadowMapTex);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, Window::m_width, Window::m_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, shadowMapTex, 0);
		//GLenum drawBuffer[1] = {GL_DEPTH_ATTACHMENT};
		//glDrawBuffers(1,drawBuffer);
		glDrawBuffer(GL_NONE);
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << " FBO sie popsulo" << std::endl;
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	/*void Renderer::testParticles(ActorTextureMatrixMaterial* mat, ParticleSystem* particleSystem)
	{


		glActiveTexture(GL_TEXTURE0 + 1);
		glBindTexture(GL_TEXTURE_2D, this->shadowMapTex);
		glBindSampler(1, this->shadowMapSampler);



		// (*it)->prepare();
		mat->use();
		mat->setUniformSampler(1);
		mat->setUniformLightDir(this->drawPassCamera->getWorldToProjectionMatrix() * this->tmp1->getDirection());
		mat->setUniformLightDir2(this->drawPassCamera->getWorldToProjectionMatrix() *this->tmp2->getDirection());
		glUniform1i(mat->smUniform, 0);


		particleSystem->draw(this->drawPassCamera->getWorldToProjectionMatrix());
	}*/

	glm::mat4 Renderer::calculateOrthoProjection()
	{
		int i = 0;
		glm::vec4 tmp;
		for (i = 0; i<10; i++){
			tmp = glm::inverse(this->drawPassCamera->getWorldToClipMatrix()) * glm::vec4(this->perspectivePoints[i], 1.0);
			//tmp = (this->drawPassCamera->getWorldToProjectionMatrix()) * glm::vec4(this->perspectivePoints[i],1.0);
			std::cout << tmp.x << " " << tmp.y << " " << tmp.z << " " << tmp.w << std::endl;
		}
		return glm::mat4(1.0);
	}

	void Renderer::removeEntity(Entity *e){
		std::vector<std::unique_ptr<Entity>>::iterator it = entities.begin();
		while (it != entities.end()){
			if ((*it).get() == e){
				it = entities.erase(it);
				break;
			}
			else{
				++it;
			}

		}
	}

}
