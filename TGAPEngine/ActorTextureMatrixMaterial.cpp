#include "ActorTextureMatrixMaterial.h"

ActorTextureMatrixMaterial::ActorTextureMatrixMaterial(const char * vertex_file_path,const char * fragment_file_path)
: engine::Material(vertex_file_path,fragment_file_path)
{
    glUseProgram(this->program);
    this->matrixUnif = glGetUniformLocation(program,"matrix");
    this->colorUniform = glGetUniformLocation(program,"color");
    this->lightDirUnif = glGetUniformLocation(program,"lightDir");
    this->lightDirUnif2 = glGetUniformLocation(program,"lightDir2");
    this->texUnif = glGetUniformLocation(program,"texture1");
    //std::cout<<this->program<<std::endl;
    worldToCamera = glm::lookAt(
    glm::vec3(-15.0f,0.0f,30.0f), // Camera is at (4,3,3), in World Space
    glm::vec3(0.0f,0.0f,0.0f), // and looks at the origin
    glm::vec3(0.0f,1.0f,0.0f)  // Head is up (set to 0,-1,0 to look upside-down)
);
    glm::vec4 light(0.0,-1.0,0.0,0.0);
    glm::vec4 light2(0.0,1.0,-1.0,0.0);
    glm::mat4 cameraToClip = glm::perspective(0.872664626f,1.0f,1.0f,100.0f);
    worldToCamera = cameraToClip * worldToCamera;
    setUniformMatrix(worldToCamera);
    setUniformColor(glm::vec4(1.0f,0.1f,0.1f,1.0f));
    setUniformLightDir(worldToCamera * light);
    setUniformLightDir2(worldToCamera * light2);

    GLint * tempBoneUnif;
    tempBoneUnif = new GLint[14];
	if (DEBUG_MODE == 1){
		std::cout << "new ActorTextureMatrixMaterial" << std::endl;
	}
    int i;
    for(i=0;i<14;i++){
        char buffer [50];
        sprintf(buffer, "bone[%d]",i);
        //std::cout<<buffer<<std::endl;
        tempBoneUnif[i] = glGetUniformLocation(program,buffer);
    }
    this->boneUniforms = tempBoneUnif;

}

GLint * ActorTextureMatrixMaterial::getBoneUniforms()
{
    return this->boneUniforms;
}


void ActorTextureMatrixMaterial::setUniformLightDir2(glm::vec4 lightDir)
{
    glUniform4fv(this->lightDirUnif2,1,&lightDir[0]);
}
void ActorTextureMatrixMaterial::setUniformLightDir(glm::vec4 lightDir)
{
    glUniform4fv(this->lightDirUnif,1,&lightDir[0]);
}


void ActorTextureMatrixMaterial::setUniformMatrix(glm::mat4 matrix)
{
glUniformMatrix4fv(this->matrixUnif,1,GL_FALSE,&matrix[0][0]);
}
void ActorTextureMatrixMaterial::setBoneUniformsIdentity()
{
    glm::mat4 ident(1.0f);
    int i;
    for(i=0;i<14;i++){
        glUniformMatrix4fv(this->boneUniforms[i],1,GL_FALSE,&ident[0][0]);
    }
}



void ActorTextureMatrixMaterial::setUniformColor(glm::vec4 color)
{
    //std::cout<<color.x<<std::endl;
    glUniform4fv(this->colorUniform,1,&color[0]);
}

void ActorTextureMatrixMaterial::setUniformSampler(int sampler)
{

    glUniform1i(this->texUnif,sampler);

}
void ActorTextureMatrixMaterial::setBoneUniform(glm::mat4 matrix, unsigned int boneNumber)
{
    glUniformMatrix4fv((this->boneUniforms[boneNumber]),1,GL_FALSE,&matrix[0][0]);
}

