#include "Entity.h"
namespace engine {
Entity::Entity(VertexAttributes * va):vertexAttributeBuf(va)
{
    this->location = glm::vec3(0.0,0.0,0.0);
    this->rotation = glm::quat(1.0,0.0,0.0,0.0);
    this->scale = glm::vec3(1.0,1.0,1.0);
	needsUpdate = true;
    //ctor
}

Entity::Entity(Entity& other) //copying ctor
{
	this->location = glm::vec3(0.0, 0.0, 0.0);
	//this->rotation = glm::quat(1.0,0.0,0.0,0.0);
	this->rotation = glm::quat(1.0, 0.0, 0.0, 0.0);
	this->scale = glm::vec3(1.0, 1.0, 1.0);
	needsUpdate = true;
	this->vertexAttributeBuf = other.vertexAttributeBuf;
}

void Entity::applyRotation(glm::quat rotationQuaternion)//globalnie - world-space
{
    this->rotation = rotationQuaternion * this->rotation;
	needsUpdate = true;
}

void Entity::applyLocalRotationQ(glm::quat rotationQuaternion)//lokalnie - model space
{
	this->rotation = this->rotation * rotationQuaternion;
	needsUpdate = true;
	this->rotation = glm::normalize(this->rotation);
	/*
	if(qm::Quaternion::isInf(this->rotation)){
	std::cout<<"this->rotation INF"<<std::endl;
	exit(-2);
	}
	if(qm::Quaternion::isNan(this->rotation)){
	std::cout<<"this->rotation Nan"<<std::endl;
	exit(-2);
	}*/
}


void Entity::applyLocalRotation(glm::quat rotationQuaternion)//lokalnie za pomoca macierzy
{
	glm::mat4 rotateMatrix = glm::mat4_cast(rotationQuaternion);
	//glm::mat4 rotateMatrix = rotationQuaternion.toMatrix();
	modelToWorldMatrix = modelToWorldMatrix * rotateMatrix;
	// this->rotation = this->rotation * rotationQuaternion;
	// this->needsUpdate = true;
}

void Entity::applyTranslation(glm::vec3 locationVector)//globalnie - world-space
{
	this->location = this->location + locationVector;
	needsUpdate = true;
	/*
	glm::vec3 nan = glm::isnan(this->location);
	glm::vec3 inf = glm::isinf(this->location);
	if (inf.x || inf.y || inf.z){
		std::cout << "glm location inf" << std::endl;
		exit(-2);
	}
	if (nan.x || nan.y || nan.z){
		std::cout << "glm location nan" << std::endl;
		exit(-2);
	}
	*/
}
void Entity::applyLocalTranslation(glm::vec3 locationVector)//lokalnie model space za pomoca macierzy
{
	modelToWorldMatrix = glm::translate(modelToWorldMatrix, locationVector);
}

void Entity::applyScale(glm::vec3 scaleXYZ)
{
    this->scale.x = this->scale.x * scaleXYZ.x;
    this->scale.y = this->scale.y * scaleXYZ.y;
    this->scale.z = this->scale.z * scaleXYZ.z;
	needsUpdate = true;
}

void Entity::applyScaleUniform(float scale)
{
    this->scale = this->scale * scale;
	needsUpdate = true;
}

void Entity::resetRotation()
{
    this->rotation = glm::quat(1.0,0.0,0.0,0.0);
	needsUpdate = true;
}

void Entity::resetLocation()
{
    this->location = glm::vec3(0.0,0.0,0.0);
	needsUpdate = true;
}

void Entity::resetScale()
{
    this->scale = glm::vec3(1.0,1.0,1.0);
	needsUpdate = true;
}
glm::mat4 Entity::getTransformationMatrix()

{
	updateMatrix();
	return modelToWorldMatrix;
}
void Entity::updateMatrix()
{
	if (needsUpdate){
		modelToWorldMatrix = glm::mat4(1.0f);
		glm::mat4 translateMatrix = glm::translate(glm::mat4(1.0f), location);
		//glm::mat4 rotateMatrix = glm::mat4_cast(rotation);
		glm::mat4 rotateMatrix =  glm::mat4_cast(rotation);
		glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), scale);

		modelToWorldMatrix = translateMatrix*scaleMatrix*rotateMatrix * modelToWorldMatrix;

		for (int i = 0; i<4; i++){
			if (std::isinf(modelToWorldMatrix[i].x)){
				std::cout << "glm mtw inf" << std::endl;
				exit(-2);
			}
			if (std::isnan(modelToWorldMatrix[i].x)){
				std::cout << "glm mtw nan" << std::endl;
				exit(-2);
			}

			if (std::isinf(modelToWorldMatrix[i].y)){
				std::cout << "glm mtw inf" << std::endl;
				exit(-2);
			}
			if (std::isnan(modelToWorldMatrix[i].y)){
				std::cout << "glm mtw nan" << std::endl;
				exit(-2);
			}

			if (std::isinf(modelToWorldMatrix[i].z)){
				std::cout << "glm mtw inf" << std::endl;
				exit(-2);
			}
			if (std::isnan(modelToWorldMatrix[i].z)){
				std::cout << "glm mtw nan" << std::endl;
				exit(-2);
			}

			if (std::isinf(modelToWorldMatrix[i].w)){
				std::cout << "glm mtw inf" << std::endl;
				exit(-2);
			}
			if (std::isnan(modelToWorldMatrix[i].w)){
				std::cout << "glm mtw nan" << std::endl;
				exit(-2);
			}
		}
		needsUpdate = false;
	}

}

Entity * Entity::makeDuplicate()
{
	Entity * e = new Entity(this->vertexAttributeBuf);
	if (DEBUG_MODE == 1){
		std::cout << "new Entity" << std::endl;
	}
	return e;
}




Entity::~Entity()
{
    //dtor
}

void Entity::draw(glm::mat4 parentMatrix, DrawingContext * context)
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
	
    if(DEBUG_MODE == 1)
    std::cout<<"Drawing glDrawElements"<<std::endl;
	glm::mat4 combinedMatrix = parentMatrix * getTransformationMatrix();
	this->entityMaterial->setUniformModelToWorld(combinedMatrix);
	if (this->disableDepthTests){
		glDisable(GL_DEPTH_TEST);
	}
    glDrawElements(GL_TRIANGLES,vertexAttributeBuf->indiceCount,GL_UNSIGNED_INT,0);

	cleanUp();
	if (this->disableDepthTests){
		glEnable(GL_DEPTH_TEST);
	}
	for (std::list<std::unique_ptr<Entity>>::iterator it = children.begin(); it != children.end(); ++it){
		(*it)->draw(combinedMatrix, context);
	}
    //std::cout<<"done"<<std::endl;
}
void Entity::addImage(Image * image)
{

	this->textures.emplace_back(image);
}
void Entity::prepare()
{
	for (std::vector<std::unique_ptr<Image>>::iterator it = this->textures.begin(); it != this->textures.end(); ++it){
		(*it)->use();
	}
	this->vertexAttributeBuf->use();
	this->entityMaterial->use();
}


void Entity::cleanUp(){
	for (std::vector<std::unique_ptr<Image>>::iterator it = this->textures.begin(); it != this->textures.end(); ++it){
		(*it)->reject();
	}
}
void Entity::addChild(Entity * e){
	this->children.emplace_back(e);
}
}
