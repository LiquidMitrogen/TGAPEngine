#include "Image.h"

namespace engine{
	Image::Image(GLubyte* data, unsigned int width, unsigned int height, int unit, GLint format, unsigned int filterMode, bool repeat) :texUnit(unit), format(format), filterMode(filterMode)
{
    glEnable(GL_TEXTURE_2D);
this->imageData = data;
this->imageHeight = height;
this->imageWidth = width;
glGenSamplers(1, &this->sampler);
if (filterMode == TG_FILTER_NEAREST){
	glSamplerParameteri(this->sampler, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glSamplerParameteri(this->sampler, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}

if (filterMode == TG_FILTER_LINEAR){
	glSamplerParameteri(this->sampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glSamplerParameteri(this->sampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}

if (filterMode == TG_FILTER_MIPMAP_LINEAR){
	glSamplerParameteri(this->sampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glSamplerParameteri(this->sampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	
}
if (filterMode == TG_FILTER_NEARMIPMAP_LINEAR){
	glSamplerParameteri(this->sampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glSamplerParameteri(this->sampler, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);

}
if (repeat){
	glSamplerParameteri(this->sampler, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glSamplerParameteri(this->sampler, GL_TEXTURE_WRAP_T, GL_REPEAT);
}
else{
	glSamplerParameteri(this->sampler,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
	glSamplerParameteri(this->sampler,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
}




glGenBuffers(1,&this->pixelBuffer);
glBindBuffer(GL_PIXEL_UNPACK_BUFFER,this->pixelBuffer);
//glPixelStorei(GL_UNPACK_ALIGNMENT,1);
if(format == GL_LUMINANCE) formatSize = 1;

if (format == GL_RED) formatSize = 1;
if(format == GL_RGB) formatSize = 3;
if(format == GL_RGBA) formatSize = 4;
glBufferData(GL_PIXEL_UNPACK_BUFFER,width * height * sizeof(GLubyte) * formatSize,data,GL_DYNAMIC_DRAW);

//glBufferData(GL_PIXEL_UNPACK_BUFFER,width * height * sizeof(GLubyte),NULL,GL_DYNAMIC_DRAW);
//this->mappedBuffer = glMapBuffer(GL_PIXEL_UNPACK_BUFFER,GL_WRITE_ONLY);
//std::memcpy(this->mappedBuffer,data,1024*64);
//glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);
//this->mappedBuffer = NULL;

glGenTextures(1,&this->handle);
glBindTexture(GL_TEXTURE_2D,this->handle);
//glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );

glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
if (filterMode == TG_FILTER_MIPMAP_LINEAR){
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 1000);
	glGenerateMipmap(GL_TEXTURE_2D);
}
else{
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
}

if (data != NULL)
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width , height, 0,format, GL_UNSIGNED_BYTE, 0);


if ((filterMode == TG_FILTER_MIPMAP_LINEAR) && (data != NULL)){
	glGenerateMipmap(GL_TEXTURE_2D);
}

glBindTexture(GL_TEXTURE_2D, 0);
glBindBuffer(GL_PIXEL_UNPACK_BUFFER,0);

}

void Image::use()
{
    if(useFlag == true){
   // std::cout<<"using texture"<<std::endl;
        glActiveTexture(GL_TEXTURE0+texUnit);
        glBindTexture(GL_TEXTURE_2D,this->handle);
        glBindSampler(texUnit,this->sampler);
    }
}

GLubyte * Image::getImageData()
{
 return this->imageData;
}

void Image::setImageData(GLubyte* imageDatas)
{
    this->imageData = imageDatas;
    //glActiveTexture(GL_TEXTURE0+texUnit);
    if(DEBUG_MODE == 1)
    std::cout<<"binding"<<std::endl;
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER,this->pixelBuffer);
    glBindTexture(GL_TEXTURE_2D,this->handle);
    //glPixelStorei(GL_UNPACK_ALIGNMENT,1);
    if (this->imageData != NULL){
        //std::cout<<"change is made"<<std::endl;
        //glBufferData(GL_PIXEL_UNPACK_BUFFER,this->imageHeight * this->imageWidth * sizeof(GLubyte),imageDatas,GL_DYNAMIC_DRAW);
        if(DEBUG_MODE == 1)
        std::cout<<"sending data to pbo"<<std::endl;
        if(1 == 2){
            this->mappedBuffer = glMapBuffer(GL_PIXEL_UNPACK_BUFFER,GL_WRITE_ONLY);
            std::memcpy(this->mappedBuffer,imageDatas,this->imageHeight * this->imageWidth * sizeof(GLubyte) * formatSize);
            glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);
            this->mappedBuffer = NULL;
        }else{
            glBufferSubData(GL_PIXEL_UNPACK_BUFFER,0,this->imageHeight * this->imageWidth * sizeof(GLubyte) * formatSize,imageDatas);
        }
        if(DEBUG_MODE == 1)
        std::cout<<"updating texture from pbo"<<std::endl;
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0,0, this->imageWidth, this->imageHeight,format, GL_UNSIGNED_BYTE, 0);
		if (filterMode == TG_FILTER_MIPMAP_LINEAR){
			glGenerateMipmap(GL_TEXTURE_2D);
		}
        //glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, this->imageWidth , this->imageHeight, 0,GL_RED, GL_UNSIGNED_BYTE, 0);
        if(DEBUG_MODE == 1)
        std::cout<<"updating done, unbinding"<<std::endl;

    }
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER,0);
}

void Image::setSubImageData(GLubyte *imageData, unsigned int bx, unsigned int by, unsigned int ex, unsigned int ey)
{
	GLuint tmp;
	this->imageData = imageData;
	//glActiveTexture(GL_TEXTURE0+texUnit);
	if (DEBUG_MODE == 1)
		std::cout << "binding" << std::endl;
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, this->pixelBuffer);
	glBindTexture(GL_TEXTURE_2D, this->handle);
	//glPixelStorei(GL_UNPACK_ALIGNMENT,1);
	if (this->imageData != NULL){
		glGenBuffers(1, &tmp);
		glBindBuffer(GL_PIXEL_UNPACK_BUFFER, tmp);
		glBufferData(GL_PIXEL_UNPACK_BUFFER, (ex-bx) * (ey-by) * sizeof(GLubyte) * formatSize, imageData, GL_DYNAMIC_DRAW);
		if (DEBUG_MODE == 1)
			std::cout << "updating texture from pbo" << std::endl;
		glTexSubImage2D(GL_TEXTURE_2D, 0, bx, by, ex - bx, ey - by, format, GL_UNSIGNED_BYTE, 0);
		if (filterMode == TG_FILTER_MIPMAP_LINEAR){
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		//glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, this->imageWidth , this->imageHeight, 0,GL_RED, GL_UNSIGNED_BYTE, 0);
		if (DEBUG_MODE == 1)
			std::cout << "updating done, unbinding" << std::endl;

	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
	//glDeleteBuffers(1, &tmp);
}
}

