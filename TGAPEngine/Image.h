#ifndef IMAGE_H
#define IMAGE_H
#include <iostream>
#define GLEW_STATIC
#include <GL/glew.h>
#include <cstring>
#include "Config.h"

#define TG_FILTER_NEAREST 0
#define TG_FILTER_LINEAR 1
#define TG_FILTER_MIPMAP_LINEAR 2
#define TG_FILTER_NEARMIPMAP_LINEAR 3



namespace engine{
class Image
{
    public:
        Image(GLubyte * data, unsigned int width,unsigned int height,int unit,GLint format, unsigned int filterMode, bool repeat);
        virtual ~Image(){};
        void use();
		void reject();
        GLubyte * getImageData();
        void setImageData(GLubyte *imageData);
		void setSubImageData(GLubyte *imageData, unsigned int bx, unsigned int by, unsigned int ex, unsigned int ey);
        int texUnit;
        void setUseFlag(bool use){this->useFlag = use;};
    protected:
    private:
		unsigned int filterMode;
        GLint format;
        unsigned int formatSize;
        GLubyte *imageData;
        unsigned int imageWidth;
        unsigned int imageHeight;
        GLuint handle;
        GLuint sampler;
        GLuint pixelBuffer;
        void * mappedBuffer;
        bool useFlag = true;

};
}
#endif // IMAGE_H
