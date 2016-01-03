#ifndef ACTORTEXTUREMATRIXMATERIAL_H
#define ACTORTEXTUREMATRIXMATERIAL_H
#include "Material.h"
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#define GLEW_STATIC
#include <GL/glew.h>
#include "Config.h"
class ActorTextureMatrixMaterial : public engine::Material
{
    public:
        //SingleMatrixMaterial() : engine::Material(){std::cout<<"der cons"<<std::endl;};
        ActorTextureMatrixMaterial(const char * vertex_file_path,const char * fragment_file_path) ;
        virtual ~ActorTextureMatrixMaterial(){};
        void setUniformMatrix(glm::mat4 matrix);
        void setUniformColor(glm::vec4 color);
        void setUniformLightDir(glm::vec4 lightDir);
        void setUniformLightDir2(glm::vec4 lightDir);
        GLint * getBoneUniforms();
        void setBoneUniformsIdentity();
        void setUniformSampler(int sampler);
        void setBoneUniform(glm::mat4 matrix,unsigned int boneNumber);
    protected:
        GLint matrixUnif;
        GLint texUnif;
        GLint colorUniform;
        GLint lightDirUnif;
        GLint lightDirUnif2;
        GLint * boneUniforms;
        glm::mat4 worldToCamera;
    private:
};

#endif // ACTORTEXTUREMATRIXMATERIAL_H
