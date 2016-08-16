
#include "Material.h"

namespace engine{

Material::Material(const char * vertex_file_path,const char * fragment_file_path)
{
    std::cout<<"madWorldBase"<<std::endl;
    loadShaders(vertex_file_path,fragment_file_path);

	glUseProgram(this->program);

	this->cameraToClipUnif = glGetUniformLocation(program, "cameraToClip");
	this->modelToWorldUnif = glGetUniformLocation(program, "modelToWorld");
	this->worldToCameraUnif = glGetUniformLocation(program, "worldToCamera");
	for (int i = 0; i < 4; i++){
		std::ostringstream dir;
		dir << "lightDir[" << i << "]";
		std::ostringstream col;
		col << "lightCol[" << i << "]";
		std::ostringstream poi;
		poi << "pointLight[" << i << "]";
		this->lightDirUnif[i] = glGetUniformLocation(program, dir.str().c_str());
		this->lightColUnif[i] = glGetUniformLocation(program, col.str().c_str());
		this->pointLightUnif[i] = glGetUniformLocation(program, poi.str().c_str());
	}
	
	
	this->lightsNumberUnif = glGetUniformLocation(program, "lightsNumber");
}
Material::~Material()
{
    //dtor
}

void Material::loadShaders(const char * vertex_file_path,const char * fragment_file_path){

    // Create the shaders
    GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    // Read the Vertex Shader code from the file
    std::string VertexShaderCode;
    std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
    if(VertexShaderStream.is_open())
    {
        std::string Line = "";
        while(getline(VertexShaderStream, Line))
            VertexShaderCode += "\n" + Line;
        VertexShaderStream.close();
    }

    // Read the Fragment Shader code from the file
    std::string FragmentShaderCode;
    std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
    if(FragmentShaderStream.is_open()){
        std::string Line = "";
        while(getline(FragmentShaderStream, Line))
            FragmentShaderCode += "\n" + Line;
        FragmentShaderStream.close();
    }

    GLint Result = GL_FALSE;
    int InfoLogLength;

    // Compile Vertex Shader
    printf("Compiling shader : %s\n", vertex_file_path);
    char const * VertexSourcePointer = VertexShaderCode.c_str();
    glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
    glCompileShader(VertexShaderID);

    // Check Vertex Shader
    glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    std::vector<char> VertexShaderErrorMessage(InfoLogLength);
	if (InfoLogLength > 0){
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		fprintf(stdout, "%s\n", &VertexShaderErrorMessage[0]);
	}
    

    // Compile Fragment Shader
    printf("Compiling shader : %s\n", fragment_file_path);
    char const * FragmentSourcePointer = FragmentShaderCode.c_str();
    glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
    glCompileShader(FragmentShaderID);

    // Check Fragment Shader
    glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    std::vector<char> FragmentShaderErrorMessage(InfoLogLength);
	if (InfoLogLength > 0){
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		fprintf(stdout, "%s\n", &FragmentShaderErrorMessage[0]);
	}
    

    // Link the program
    fprintf(stdout, "Linking program\n");
    GLuint ProgramID = glCreateProgram();
    glAttachShader(ProgramID, VertexShaderID);
    glAttachShader(ProgramID, FragmentShaderID);
    glBindFragDataLocation(ProgramID,0,"colorOut");
    glLinkProgram(ProgramID);

    // Check the program
    glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
    glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    std::vector<char> ProgramErrorMessage( std::max(InfoLogLength, int(1)) );
    glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
    fprintf(stdout, "%s\n", &ProgramErrorMessage[0]);

    glDeleteShader(VertexShaderID);
    glDeleteShader(FragmentShaderID);

    this->program = ProgramID;
}
void Material::use()
{
    glUseProgram(this->program);
}

void Material::setUniformCameraToClip(glm::mat4 matrix)
{
	glUniformMatrix4fv(this->cameraToClipUnif, 1, GL_FALSE, &matrix[0][0]);
}

void Material::setUniformModelToWorld(glm::mat4 matrix)
{
	glUniformMatrix4fv(this->modelToWorldUnif, 1, GL_FALSE, &matrix[0][0]);
}

void Material::setUniformWorldToCamera(glm::mat4 matrix)
{
	glUniformMatrix4fv(this->worldToCameraUnif, 1, GL_FALSE, &matrix[0][0]);
}
void Material::setUniformLightDir(glm::vec4 lightDir,int index)
{
	glUniform4fv(this->lightDirUnif[index], 1, &lightDir[0]);
}

void Material::setUniformLightCol(glm::vec3 lightCol, int index)
{
	glUniform3fv(this->lightColUnif[index], 1, &lightCol[0]);
}

void Material::setUniformPointLight(bool isPointLight,int index){
	if (isPointLight){
		glUniform1ui(this->pointLightUnif[index], 1);
	}
	else
	{
		glUniform1ui(this->pointLightUnif[index], 0);
	}
	
}
void Material::setUniformLightsNumber(unsigned int number){
	
	glUniform1ui(this->lightsNumberUnif, number);
	

}
}
