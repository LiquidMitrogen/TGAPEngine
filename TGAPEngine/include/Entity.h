#ifndef ENTITY_H
#define ENTITY_H
#include <iostream>
#include <string>
#include "Bone.h"
#include "Material.h"
#include "VertexAttributes.h"
//#define GLEW_STATIC
#include <GL/glew.h>
#include <vector>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Image.h"
#include "Config.h"
#include <windows.h>
#include "DrawingContext.h"
namespace engine{

class Entity
{
    public:
		unsigned int buildId = 0;
		std::string name;
		bool makesShadow = true;
		bool disableDepthTests = false;
		bool disableDrawing = false;

		Entity(VertexAttributes * va);
		Entity(Entity& other);
        virtual ~Entity();

        VertexAttributes * getVertexAttributes(){return vertexAttributeBuf;};
        void addImage(Image * image);
		virtual void draw(glm::mat4 parentMatrix, DrawingContext * context);
		//void draw(glm::mat4  worldToProjectionMatrix);
        Material * entityMaterial;

        void applyRotation(glm::quat rotationQuaternion);
		void applyRotation(glm::mat3 rotationMatrix);
		void applyLocalRotation(glm::quat rotationQuaternion);
		void applyLocalRotationQ(glm::quat rotationQuaternion);

        void applyTranslation(glm::vec3 locationVector);
		void applyLocalTranslation(glm::vec3 locationVector);

        void applyScale(glm::vec3 scaleXYZ);
        void applyScaleUniform(float scale);

        void resetRotation();
        void resetLocation();
        void resetScale();
		 
		void setTransformationMatrix(glm::mat4 matrix);

		void addChild(Entity * e);
		void setParent(Entity * e){ this->parent = e; };
		Entity * getParent(){ return this->parent; };
        glm::mat4 getTransformationMatrix();
		std::shared_ptr<Entity> findChildByName(std::string name);
        //void updateTransformationMatrix();
		virtual void prepare();
		virtual void cleanUp();
		virtual Entity * makeDuplicate();
    protected:
		Entity * parent = NULL;
		std::list<std::shared_ptr<Entity>> children;

        std::vector<std::unique_ptr<Image>> textures;

		glm::mat4 modelToWorldMatrix;

        glm::vec3 location;
        glm::quat rotation;
        glm::vec3 scale;
        VertexAttributes * vertexAttributeBuf;

		bool needsUpdate = false;
		void updateMatrix();
    private:
};
}
#endif // ENTITY_H
