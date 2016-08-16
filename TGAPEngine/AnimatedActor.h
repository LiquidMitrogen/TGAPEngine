#ifndef ANIMATEDACTOR_H
#define ANIMATEDACTOR_H
#include "Bone.h"
#include "Material.h"
#include "VertexAttributes.h"
#include "SingleMatrixMaterial.h"
#include "ActorTextureMatrixMaterial.h"
#define GLEW_STATIC
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
        AnimatedActor(VertexAttributes * va,Bone * root,int maxAnimationFrame)
			:Entity(va), rootBone(root), maxAnimationFrame(maxAnimationFrame){};
        virtual ~AnimatedActor();
        VertexAttributes * getVertexAttributes(){return vertexAttributeBuf;};
        void calcBoneUniforms();
		Bone * getArmatureRoot(){
			return this->rootBone;
		}
        //SingleMatrixMaterial * entityMaterial;
		void setAmimationFrame(unsigned int frame){ this->animationFrameIndex = frame; this->needBoneUnifUpdate = true; };
        int getmaxAnimationFrame(){return this->maxAnimationFrame;};
		void applyRotationToBoneAtFrame(Bone * bone, int frame, glm::quat quaternion);
		virtual void prepare();
	protected:
        int animationFrameIndex = 1;
        int maxAnimationFrame;
        Bone * rootBone;
		bool needBoneUnifUpdate = true;



    private:
};
}
#endif // ANIMATEDACTOR_H
