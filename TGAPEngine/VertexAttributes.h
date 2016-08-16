#ifndef VERTEXATTRIBUTES_H
#define VERTEXATTRIBUTES_H
//#define GLEW_STATIC
#include <GL/glew.h>
#include<vector>
#include<iostream>
namespace engine{
    typedef struct VaoInitData
    {
        unsigned char attrNumber;
        unsigned char attrSize[16];
        GLenum attrType[16];
        GLboolean attrNormBool[16];
        unsigned int attrOffset[16];

    }VaoInitData;
class VertexAttributes
{
    public:
        VertexAttributes();
        VertexAttributes(std::vector<char> data,std::vector<unsigned short> iData,VaoInitData initData);
        VertexAttributes(std::vector<char> data,std::vector<unsigned int> iData,VaoInitData initData);
		VertexAttributes(std::vector<char> data, VaoInitData initData, unsigned int additionalBufferSize);//particle mode instanced no indices
        virtual ~VertexAttributes();
        void use();
        unsigned int indiceCount;
		void updateBuffer(char * data, unsigned int offset, unsigned int size);
    protected:
    private:
        GLuint vboBuffer;
        GLuint iVboBuffer;
        GLuint vaoBuffer;


        void createBuffer(std::vector<char> data, std::vector<unsigned short> iData);
        void createBuffer(std::vector<char> data, std::vector<unsigned int> iData);
		void createBuffer(std::vector<char> data, unsigned int additionalBufferSize);
        void createVertexArray(VaoInitData initData);



};
}

#endif // VERTEXATTRIBUTES_H
