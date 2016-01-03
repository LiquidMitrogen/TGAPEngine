#ifndef SINGLEMATRIXMATERIAL_H
#define SINGLEMATRIXMATERIAL_H
#include "Material.h"
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#define GLEW_STATIC
#include <GL/glew.h>
#include "Config.h"
class SingleMatrixMaterial : public engine::Material
{
    public:
        
        SingleMatrixMaterial(const char * vertex_file_path,const char * fragment_file_path) ;
        virtual ~SingleMatrixMaterial(){};
        
        void setUniformColor(glm::vec4 color);
        void setUniformAmbient(glm::vec4 color);
		void setUniformSpecular(glm::vec4 color);
        void setUniformShininess(float shininess);
		void setUniformSampler(GLuint sampler);
		void setNormalUniformSampler(GLuint sampler);
		void setSpecularColorUniformSampler(GLuint sampler);
		void setShininessUniformSampler(GLuint sampler);
        GLint * getBoneUniforms();
        void setBoneUniformsIdentity();
        glm::mat4 genMatrixWithAngle(float angle,float dist);
    protected:
        float angle = 0;
       
		GLint colorUniform, ambientUniform, specularUniform, shininessUniform, samplerUniform;
		GLint specTexSamplerUniform, shininessSamplerUniform, normalMapSamplerUniform;
        GLint * boneUniforms;
        glm::mat4 worldToCamera;
    private:
};

#endif // SINGLEMATRIXMATERIAL_H
