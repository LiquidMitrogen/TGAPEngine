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
void AnimatedActor::prepareArmature()
{

    this->entityMaterial->use();
    this->armature->modifyArmatureUniforms(((SingleMatrixMaterial*)this->entityMaterial)->getBoneUniforms());
    
}

void AnimatedActor::prepare(){
	Entity::prepare();
	prepareArmature();
}
}
