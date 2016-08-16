#ifndef ANIMATEDACTOR_H
#define ANIMATEDACTOR_H

#include "Armature.h"
#include "Material.h"
#include "VertexAttributes.h"
#include "SingleMatrixMaterial.h"
#include "ActorTextureMatrixMaterial.h"
//#define GLEW_STATIC
#include <GL/glew.h>
#include <vector>
#include "Image.h"
#include "Entity.h"
#include "Config.h"
#include <iostream>
namespace engine{

class AnimatedActor: public Entity
{
    public:
        AnimatedActor(VertexAttributes * va,std::string armatureName)
			:Entity(va), armatureName(armatureName){};
        virtual ~AnimatedActor();
        VertexAttributes * getVertexAttributes(){return vertexAttributeBuf;};
        void prepareArmature();
		Armature * getArmature(){
			return this->armature;
		};
		void assignArmature(Armature * armature){
			this->armature = armature;
		};
		//void setAnimationFrame(unsigned int frame){ this->animationFrameIndex = frame; this->needBoneUnifUpdate = true; };
		
		virtual void prepare();

		std::string armatureName;
	protected:
		
		Armature * armature;



    private:
};
}
#endif // ANIMATEDACTOR_H
