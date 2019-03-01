#ifndef UTILITY_H_INCLUDED
#define UTILITY_H_INCLUDED

#include<iostream>
#define M_PI       3.14159265358979323846
#include<cmath>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>


namespace engine{
    float interpf(float a, float b, float val);
    //void convolute(float * a,unsigned int al, float * b,unsigned int bl, float*out);
    //void lowPassGen(float * out, unsigned int len, float cutoffFreq);
    //void blackmanWindowGen(float * out,unsigned int totalLen);
    //void envelopeRectifier(float * in, unsigned int inLength, float * lowPass, unsigned int lowPassLength,float * out);
    //void envelopeMarker(float * envelope,unsigned int envelopeLength, float threshold);
	//void blitChar(unsigned char * src, unsigned char * dst, unsigned int dstx, unsigned int dsty, unsigned int ssizex, unsigned int ssizey, unsigned int dsizex, unsigned int dsizey);
	//
	void blitCharRGBA(unsigned char * src, unsigned char * dst, unsigned int dstx, unsigned int dsty, unsigned int ssizex, unsigned int ssizey, unsigned int dsizex, unsigned int dsizey);
	glm::quat twoVectorsToQuat(glm::vec3 A, glm::vec3 B);
	
}


#endif // UTILITY_H_INCLUDED
