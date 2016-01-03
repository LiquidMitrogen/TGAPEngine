#ifndef MATERIAL_H
#define MATERIAL_H
#include<iostream>
#include<string>
#include<vector>
#include<stdio.h>
#include<fstream>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define GLEW_STATIC
#include <GL/glew.h>
#include<algorithm>

namespace engine{
class Material
{
    public:
        void loadShaders(const char * vertex_file_path,const char * fragment_file_path);
        Material(const char * vertex_file_path,const char * fragment_file_path);
        virtual ~Material();
		virtual void setUniformCameraToClip(glm::mat4 matrix);
		virtual void setUniformWorldToCamera(glm::mat4 matrix);
		virtual void setUniformModelToWorld(glm::mat4 matrix);
		void setUniformLightDir(glm::vec4 lightDir);
		void setUniformPointLight(bool isPointLight);
        void use();

    protected:
        GLuint program;
		GLint cameraToClipUnif;
		GLint worldToCameraUnif;
		GLint modelToWorldUnif;
		GLint lightDirUnif;
		GLint pointLightUnif;

    private:


};
}
#endif // MATERIAL_H
