#ifndef MATERIAL_H
#define MATERIAL_H
#include<iostream>
#include<string>
#include<vector>
#include<stdio.h>
#include<sstream>
#include<fstream>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
//#define GLEW_STATIC
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
		void setUniformLightDir(glm::vec4 lightDir, int index);
		void setUniformLightCol(glm::vec3 lightCol, int index);
		void setUniformPointLight(bool isPointLight,int index);
		void setUniformLightsNumber(unsigned int number);
        void use();

    protected:
        GLuint program;
		GLint cameraToClipUnif;
		GLint worldToCameraUnif;
		GLint modelToWorldUnif;
		GLint lightDirUnif[4];
		GLint lightColUnif[4];
		GLint pointLightUnif[4];
		GLint lightsNumberUnif;

    private:


};
}
#endif // MATERIAL_H
