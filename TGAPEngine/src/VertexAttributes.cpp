#include "VertexAttributes.h"
namespace engine{
VertexAttributes::VertexAttributes()
{
    //ctor
}
VertexAttributes::VertexAttributes(std::vector<char> data, std::vector<unsigned short> iData, VaoInitData initData)
{
    createBuffer(data,iData);
    createVertexArray(initData);

}
VertexAttributes::VertexAttributes(std::vector<char> data, std::vector<unsigned int> iData, VaoInitData initData)
{
    createBuffer(data,iData);
    createVertexArray(initData);

}

VertexAttributes::VertexAttributes(std::vector<char> data, VaoInitData initData, unsigned int additionalBufferSize){
	createBuffer(data, additionalBufferSize);
	createVertexArray(initData);

}

void VertexAttributes::createBuffer(std::vector<char> data, unsigned int additionalBufferSize)
{
	glGenBuffers(1, &this->vboBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, this->vboBuffer);
	glBufferData(GL_ARRAY_BUFFER, data.size() + additionalBufferSize *sizeof(char), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, data.size() * sizeof(char), &data.front());
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	this->indiceCount = 0;
	
}

VertexAttributes::~VertexAttributes()
{
    //dtor
}
void VertexAttributes::createBuffer(std::vector<char> data, std::vector<unsigned short> iData)
{
    indiceType = GL_UNSIGNED_SHORT;
    glGenBuffers(1,&this->vboBuffer);
    glBindBuffer(GL_ARRAY_BUFFER,this->vboBuffer);
    glBufferData(GL_ARRAY_BUFFER,data.size()*sizeof(char),&data.front(),GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER,0);

    this->indiceCount = iData.size();
    glGenBuffers(1,&this->iVboBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,this->iVboBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,iData.size()*sizeof(short),&iData.front(),GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);

}
void VertexAttributes::createBuffer(std::vector<char> data, std::vector<unsigned int> iData)
{
    indiceType = GL_UNSIGNED_INT;
    glGenBuffers(1,&this->vboBuffer);
    glBindBuffer(GL_ARRAY_BUFFER,this->vboBuffer);
    glBufferData(GL_ARRAY_BUFFER,data.size()*sizeof(char),&data.front(),GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER,0);

    this->indiceCount = iData.size();
    glGenBuffers(1,&this->iVboBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,this->iVboBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,iData.size()*sizeof(int),&iData.front(),GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);

}
void VertexAttributes::createVertexArray(VaoInitData initData)
{
    glGenVertexArrays(1,&this->vaoBuffer);
    glBindVertexArray(this->vaoBuffer);
    glBindBuffer(GL_ARRAY_BUFFER,this->vboBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,this->iVboBuffer);
    if(initData.attrNumber>16)
    {
        std::cerr<<"Warning : attribute number is greater than 16"<<std::endl;
        initData.attrNumber=16;
    }
    int i;
    for(i=0;i<initData.attrNumber;++i){
        glEnableVertexAttribArray(i);
        if(initData.attrType[i] == GL_FLOAT){
            glVertexAttribPointer(i,initData.attrSize[i],initData.attrType[i],initData.attrNormBool[i],0,(void*)initData.attrOffset[i]);
        }else{
            glVertexAttribIPointer(i,initData.attrSize[i],initData.attrType[i],0,(void*)initData.attrOffset[i]);
        }
    }
    glBindVertexArray(0);
}
void VertexAttributes::use()
{
    glBindVertexArray(this->vaoBuffer);
    glBindBuffer(GL_ARRAY_BUFFER,this->vboBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,this->iVboBuffer);


}

void VertexAttributes::updateBuffer(char * data, unsigned int offset, unsigned int size){
	glBindBuffer(GL_ARRAY_BUFFER, this->vboBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

}
