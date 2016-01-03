#ifndef BONEMATERIALINTERFACE_H
#define BONEMATERIALINTERFACE_H

namespace engine{
class BoneMaterialInterface
{
    public:
        virtual BoneMaterialInterface();
        virtual ~BoneMaterialInterface();
    protected:
        GLint * boneUniforms;
        GLint * getBoneUniforms();
    private:

};
}

#endif // BONEMATERIALINTERFACE_H
