#include "AnimatedActor.h"
namespace engine {


AnimatedActor::~AnimatedActor()
{
    //dtor
}/*
void AnimatedActor::draw()
{

    this->vertexAttributeBuf->use();
    entityMaterial->use();
    for (std::vector<std::unique_ptr<Image>>::iterator it = this->textures.begin() ; it != this->textures.end(); ++it){
        (*it)->use();
    }
    glDrawElements(GL_TRIANGLES,vertexAttributeBuf->indiceCount,GL_UNSIGNED_INT,0);
}
*/
void AnimatedActor::calcBoneUniforms()
{
    glm::mat4 * unitMatrix;
    unitMatrix = new glm::mat4(1.0f);
	if (DEBUG_MODE == 1){
		std::cout << "new AnimatedActor" << std::endl;
	}
    this->entityMaterial->use();
    rootBone->modifyBoneUnif(((SingleMatrixMaterial*)this->entityMaterial)->getBoneUniforms(),unitMatrix,glm::quat(1.0f,0.0f,0.0f,0.0f),this->animationFrameIndex);
    delete unitMatrix;
}
void AnimatedActor::applyRotationToBoneAtFrame(Bone * bone, int frame, glm::quat quaternion){
	this->needBoneUnifUpdate = true;
	bone->applyArmatureRotation(frame, quaternion);
}
void AnimatedActor::prepare(){
	Entity::prepare();
	if (needBoneUnifUpdate == true){
		calcBoneUniforms();
		needBoneUnifUpdate = false;
	}
}
}
