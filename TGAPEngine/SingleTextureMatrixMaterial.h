#ifndef SINGLETEXTUREMATRIXMATRIAL_H
#define SINGLETEXTUREMATRIXMATRIAL_H
#include "Material.h"
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#define GLEW_STATIC
#include <GL/glew.h>
class SingleTextureMatrixMatrial : public engine::Material
{
    public:
        //SingleMatrixMaterial() : engine::Material(){std::cout<<"der cons"<<std::endl;};
        SingleTextureMatrixMatrial(const char * vertex_file_path,const char * fragment_file_path) ;
        virtual ~SingleTextureMatrixMatrial(){};
        void setUniformMatrix(glm::mat4 matrix);
        void setUniformSampler(int sampler);
    protected:
        GLint matrixUnif;
        GLint texUnif;
    private:
};

#endif // SINGLETEXTUREMATRIXMATRIAL_H
