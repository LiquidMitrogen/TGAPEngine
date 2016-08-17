#include "Utility.h"

namespace engine{

float interpf(float a, float b, float val)
{
    return ((b-a) * val + a);
}
void convolute(float* a, unsigned int al, float* b, unsigned int bl, float* out)
{
    unsigned int ai,bi;//iteratory
    //a jest dluzsze od b
    //a jest sygnalem wejsciowym
    //b jest odpowiedzia impulsowa
    //out jest wynikiem splotu obu sygnalow
    //dlugosc out jest rowna sumie dlugosci a i b
    //wszelkie bufory musza byc zadeklarowane
    if(a == NULL || b==NULL || out==NULL){
        return;
    }else{
        for(ai=0;ai<al+bl;ai++){
            out[ai]=0;
            for(bi=0;bi<bl;bi++){
                    if(ai-bi>=0 && ai-bi< al){
                    out[ai]+=a[ai-bi] * b[bi];
                }
            }
            //std::cout<<ai<<" : "<<out[ai]<<std::endl;
        }
    }
}
void lowPassGen(float* out, unsigned int len, float cutoffFreq)
{
    unsigned int i;
    int ri;
    unsigned int m;
    if((len % 2) == 0){
        m = len-1;
        out[len-1]=0;
    }else{
        m = len;
    }
    ri = -((m-1)/2);
    //m powinno byc nieparzyste
    //(m-1)/2 = indeks srodka
    //Fc = czestotliwosc zadana / (czestotliwosc samplingu) ale nie > od 0.5
    //w przykladzie sampling rate = 22050
    //max cutofffreq = 11025hz przy 0.5
    //rolloff zalezy od len
    //filtr typu Windowed-Sinc
    //nie za szybka generacja ze wzgledu na dzielenie liczb zmiennoprzecinkowych
    blackmanWindowGen(out,m);
    for(i=0;i<m;i++){
        //out[i]=out[i]*(sin(M_PI * 2 * cutoffFreq *ri)/ri*M_PI);
        if(ri!=0){
            out[i]*=2*cutoffFreq * (sin(M_PI * 2 * cutoffFreq *ri)/(M_PI * 2 * cutoffFreq *ri));
        }else out[i] *= 2 * cutoffFreq;
       // std::cout<<ri<<" ";
        ri++;
    }
}
void blackmanWindowGen(float* out, unsigned int totalLen)
{
    //do generowania funkcji okna
    //Test OK
    unsigned int i;
    for(i=0;i<totalLen;i++){
        out[i]= 0.42 - 0.5 * cos(M_PI * 2 * i / (totalLen-1)) + 0.08 * cos(M_PI * 4 *i/(totalLen-1));
    }
}


void envelopeRectifier(float* in, unsigned int inLength, float* lowPass, unsigned int lowPassLength, float* out)
{
//dlugoœc out = dlugosc in + lowPass
unsigned int i;
for(i=0;i<inLength;i++){
    if(in[i] < 0){
        in[i] = -in[i];
    }
}
convolute(in,inLength,lowPass,lowPassLength,out);
}

void envelopeMarker(float* envelope,unsigned int envelopeLength, float threshold)
{//zaznacza miejsca przekracaj¹ce threshold, zbocza wzrastajace jako wartosc wieksza od zera, opadajace jako mniejsze od zera
    unsigned int i;
    bool aboveThreshold = false;
    if(envelope[0]>threshold){
        aboveThreshold = true;
    }
    for(i=1;i<envelopeLength;i++){
        if(aboveThreshold == false && envelope[i] > threshold){
            aboveThreshold = true;
            envelope[i] = 1.0;
        }else
            if(aboveThreshold == true && envelope[i] <=threshold){
                aboveThreshold = false;
                envelope[i] = -1.0;
            }else{
                envelope[i] = 0;
            }
    }
}
void blitChar(unsigned char * src, unsigned char * dst, unsigned int dstx, unsigned int dsty, unsigned int ssizex, unsigned int ssizey, unsigned int dsizex, unsigned int dsizey){
	unsigned int i, j;
	for (i = 0; i < ssizey; i++){
		for (j = 0; j < ssizex; j++){
			dst[((dsty + i) * dsizex) + dstx + j] = src[i * ssizex + j];
		}
	}
}

void blitCharRGBA(unsigned char * src, unsigned char * dst, unsigned int dstx, unsigned int dsty, unsigned int ssizex, unsigned int ssizey, unsigned int dsizex, unsigned int dsizey){
	unsigned int i, j;
	for (i = 0; i < ssizey; i++){
		for (j = 0; j < ssizex; j++){
			unsigned int x = dstx + j * 4;
			unsigned int y = ((dsty + i) * dsizex * 4);
			dst[((dsty + i) * dsizex * 4) + dstx * 4 + j * 4] = src[i * ssizex + j];
		}
	}
}

glm::quat twoVectorsToQuat(glm::vec3 A, glm::vec3 B)
{
	A = glm::normalize(A);
	B = glm::normalize(B);
	float dotProduct = glm::dot(A, B);
	glm::vec3 crossProduct = glm::cross(A, B);
	crossProduct = glm::normalize(crossProduct);
	float angle = acos(dotProduct);
	//std::cout<<"A vector "<<A.x<<" "<<A.y<<" "<<A.z<<std::endl;
	//std::cout<<"B vector "<<B.x<<" "<<B.y<<" "<<B.z<<std::endl;
	//std::cout<<"Cross "<<crossProduct.x<<" "<<crossProduct.y<<" "<<crossProduct.z<<" Angle "<<angle<<std::endl;
	if (angle < 0.0001) return glm::angleAxis(0.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	return glm::angleAxis(-angle, crossProduct);
}

}
