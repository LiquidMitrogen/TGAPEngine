#include "SingleMatrixMaterial.h"

SingleMatrixMaterial::SingleMatrixMaterial(const char * vertex_file_path,const char * fragment_file_path)
: engine::Material(vertex_file_path,fragment_file_path)
{
    glUseProgram(this->program);
    
    //this->colorUniform = glGetUniformLocation(program,"colorRed");
    this->colorUniform = glGetUniformLocation(program,"diffuse");
    this->ambientUniform = glGetUniformLocation(program,"ambient");
    this->specularUniform = glGetUniformLocation(program,"specular");
    this->shininessUniform = glGetUniformLocation(program,"shininess");
	this->samplerUniform = glGetUniformLocation(program, "texture1");
	this->normalMapSamplerUniform = glGetUniformLocation(program, "normalMap");
	this->specTexSamplerUniform = glGetUniformLocation(program, "specTexMap");
	this->shininessSamplerUniform = glGetUniformLocation(program, "shininessMap");
    
    //std::cout<<this->program<<std::endl;
//worldToCamera = genMatrixWithAngle(0.0,10.0);
    //setUniformMatrix(worldToCamera);
    //setUniformColor(glm::vec4(1.0f,0.0f,0.0f,1.0f));
    //setUniformLightDir(worldToCamera * light);
    //setup
    setUniformAmbient(glm::vec4(1.0f,0.0f,0.0f,0.0f));
	GLint * tempBoneUnif;
	tempBoneUnif = new GLint[14];
	if (DEBUG_MODE == 1){
		std::cout << "new SingleMat" << std::endl;
	}
	int i;
	for (i = 0; i<14; i++){
		char buffer[50];
		sprintf(buffer, "bone[%d]", i);
		//std::cout<<buffer<<std::endl;
		tempBoneUnif[i] = glGetUniformLocation(program, buffer);
	}
	this->boneUniforms = tempBoneUnif;
}

GLint * SingleMatrixMaterial::getBoneUniforms()
{
    return this->boneUniforms;
}



glm::mat4 SingleMatrixMaterial::genMatrixWithAngle(float angle, float dist){
    glm::mat4 lookAtMat = glm::lookAt(
    glm::vec3(sin(angle)*dist,3.0f,cos(angle)*dist), // Camera is at (4,3,3), in World Space
    glm::vec3(0.0f,0.0f,0.0f), // and looks at the origin
    glm::vec3(0.0f,1.0f,0.0f)  // Head is up (set to 0,-1,0 to look upside-down)
);
    //glm::vec4 light(0.0,-1.0,0.0,0.0);

    glm::mat4 camPerMat = lookAtMat;
    return camPerMat;
}




void SingleMatrixMaterial::setUniformColor(glm::vec4 color)
{
    glUniform4fv(this->colorUniform,1,&color[0]);
}

void SingleMatrixMaterial::setUniformAmbient(glm::vec4 color)
{
    glUniform4fv(this->ambientUniform,1,&color[0]);
}
void SingleMatrixMaterial::setUniformSpecular(glm::vec4 color)
{
    glUniform4fv(this->specularUniform,1,&color[0]);
}
void SingleMatrixMaterial::setUniformShininess(float shininess)
{
    glUniform1f(this->shininessUniform,shininess);
}

void SingleMatrixMaterial::setUniformSampler(GLuint sampler){
	glUniform1i(this->samplerUniform, sampler);
}

void SingleMatrixMaterial::setNormalUniformSampler(GLuint sampler){
	glUniform1i(this->normalMapSamplerUniform, sampler);
}
void SingleMatrixMaterial::setSpecularColorUniformSampler(GLuint sampler){
	glUniform1i(this->specTexSamplerUniform, sampler);
}
void SingleMatrixMaterial::setShininessUniformSampler(GLuint sampler){
	glUniform1i(this->shininessSamplerUniform, sampler);
}
//void SingleMatrixMaterial::setUniformColor(float color)
//{
    //std::cout<<color.x<<std::endl;
  //  glUniform1f(this->colorUniform,color);
//}

