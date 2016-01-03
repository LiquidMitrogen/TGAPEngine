#include "SingleTextureMatrixMaterial.h" //rename

SingleTextureMatrixMatrial::SingleTextureMatrixMatrial(const char * vertex_file_path,const char * fragment_file_path)
: engine::Material(vertex_file_path,fragment_file_path)
{
    glUseProgram(this->program);
    this->matrixUnif = glGetUniformLocation(program,"matrix");
    this->texUnif = glGetUniformLocation(program,"texture1");
    glm::mat4 worldToCamera = glm::lookAt(
    glm::vec3(-15.0f,0.0f,30.0f), // Camera is at (4,3,3), in World Space
    glm::vec3(0.0f,0.0f,0.0f), // and looks at the origin
    glm::vec3(0.0f,1.0f,0.0f)  // Head is up (set to 0,-1,0 to look upside-down)
);
    glm::mat4 cameraToClip = glm::perspective(0.872664626f,1.0f,1.0f,100.0f);
   // worldToCamera = cameraToClip * worldToCamera;
    setUniformMatrix(cameraToClip);

}



void SingleTextureMatrixMatrial::setUniformMatrix(glm::mat4 matrix)
{
glUniformMatrix4fv(this->matrixUnif,1,GL_FALSE,&matrix[0][0]);
}

void SingleTextureMatrixMatrial::setUniformSampler(int sampler)
{
    glUniform1i(this->texUnif,sampler);
}

