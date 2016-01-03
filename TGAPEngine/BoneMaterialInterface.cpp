#include "BoneMaterialInterface.h"

BoneMaterialInterface::BoneMaterialInterface()
{
    //ctor
}

BoneMaterialInterface::~BoneMaterialInterface()
{
    //dtor
}
GLint * BoneMaterialInterface::getBoneUniforms()
{
    return this->boneUniforms;
}
