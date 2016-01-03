#include "FftEvent.h"

void FftEvent::callMe(){
    //image2->setUseFlag(false);
//mic->fft();
fft->calculateFFT();
//kiss_fft_cpx * fftCpx = mic->getFftResult();
//if(DEBUG_MODE == 1)
//std::cout<<"i got fft results"<<std::endl;
//float* smaples = mic->getDataBlockIndex();
//float * sinc = mic->sincBlackmann;
//float * convoluted;
//convoluted = new float[201 + 1024];
//engine::convolute(smaples,1024,sinc,201,convoluted);

//float maxMag = 0;
//float minMag = 10000000;
//float maxEq = 0;
if(DEBUG_MODE == 1)
std::cout<<"making buffer for image"<<std::endl;
GLubyte * images;
images = new GLubyte[1024*2];
int o,k;
for(o=0;o<2;o++)
    for(k=0;k<1024;k++)
        images[o*1024+k] = 0;
//if(DEBUG_MODE == 1)
//std::cout<<"buffer initialised, calculating magnitude"<<std::endl;

//for(o=1;o<513;o++){
//  magnitude[o-1] =  sqrt(fftCpx[o].r * fftCpx[o].r + fftCpx[o].i * fftCpx[o].i);
 //  if(magnitude[o-1] < minMag)
  //      minMag = magnitude[o-1];
 //  magnitude[o-1] = 20 * log10(magnitude[o-1]/0.2);
  // magnitude[o-1] += 25.0f;
 //  if(magnitude[o-1] > maxMag)
 //       maxMag = magnitude[o-1];
//}
//magnitude[0] = 0;
//magnitude[1] = 0;
//magnitude[2] = 0;
//magnitude[3] = 0;
//if(DEBUG_MODE == 1)
//std::cout<<"magnitude calculated, generating an image"<<std::endl;
  //  maxMag = 14;
for(o=0;o<512;o++){

   float value;
    if(fft->maxMag > 1){
        value = fft->magnitude[o] / fft->maxMag;
    }else{
        value = fft->magnitude[o];
    }
   if(value<0)   value = 0;
   images[o*2] = value * 255;
   images[o*2 + 1] = value * 255;
   images[1024+o*2] = value * 255;
   images[1024+o*2 + 1] = value * 255;

}

//float avgEq = 0;
int numberOfMag = 510 / 9;
//int numberOfMag = 200 / 9;
//for(o=0;o<9;o++){
//    equalizerMagnitude[o] = 0;
 //   for(k=0;k<numberOfMag;k++){
 //       equalizerMagnitude[o] += magnitude[(o * numberOfMag) +k];
 //       if(equalizerMagnitude[o]<0.02) equalizerMagnitude[o] = 0.02;
 //   }
 //   avgEq += equalizerMagnitude[o];
 //   if(equalizerMagnitude[o]>maxEq) maxEq = equalizerMagnitude[o];
//}



if(DEBUG_MODE == 1)
std::cout<<"image generated, setting pbo and texture"<<std::endl;
//if(maxEq<1.0f) maxEq = 1.0f;
//avgEq = avgEq / 9;
//equalizerMagnitude[0] = equalizerMagnitude[0] / 2;
/*
for(o=0;o<1024;o++){
    images[(int)(floor(smaples[o] * 55)) * 1024 + o] = 0;
}*//*
for(o=0;o<101;o++){
    images[(int)(floor(sinc[o] * 30) + 30) * 1024 + o] = 0;
}*/
/*
for(o=55;o<1024;o++){
        //std::cout<< " ada sd a" <<convoluted[o]<<std::endl;
        if(convoluted[o]<0)convoluted[o] = -convoluted[o];
   // images[(int)(floor(convoluted[o] * 55)) * 1024 + (o-50)] = 0;
}
*/
//std::cout<<o*2 + 1<<std::endl;
//Pa_Sleep(100);*/


/*
int i;
for(i=0;i<1024*64;i++) images[i]=255;
images[32 * 1024 + 511] = 0;
images[32 * 1024 + 512] = 0;
images[32 * 1024 + 513] = 0;
images[32 * 1024 + 514] = 0;
images[32 * 1024 + 515] = 0;
images[32 * 1024 + 522] = 0;*/

//std::cout<<" changing texture now..."<<std::endl;
//image->setImageData(&imageData[0][0]);equalizerMagnitude[0] / maxEq
//std::cout<<equalizerMagnitude[1]<<std::endl;

//std::cout<<minMag<<std::endl;
//maxEq += 75;
float maxEq = numberOfMag * 14;
material->use();
material->setBoneUniform(glm::scale(glm::mat4(1.0f),glm::vec3(1.0f,(fft->equalizerMagnitude[0]/ maxEq)+0.02,1.0f)),8);
material->setBoneUniform(glm::scale(glm::mat4(1.0f),glm::vec3(1.0f,(fft->equalizerMagnitude[1]/ maxEq)+0.02,1.0f)),7);
material->setBoneUniform(glm::scale(glm::mat4(1.0f),glm::vec3(1.0f,(fft->equalizerMagnitude[2]/ maxEq)+0.02,1.0f)),6);
material->setBoneUniform(glm::scale(glm::mat4(1.0f),glm::vec3(1.0f,(fft->equalizerMagnitude[3]/ maxEq)+0.02,1.0f)),5);
material->setBoneUniform(glm::scale(glm::mat4(1.0f),glm::vec3(1.0f,(fft->equalizerMagnitude[4]/ maxEq)+0.02,1.0f)),9);
material->setBoneUniform(glm::scale(glm::mat4(1.0f),glm::vec3(1.0f,(fft->equalizerMagnitude[5]/ maxEq)+0.02,1.0f)),1);
material->setBoneUniform(glm::scale(glm::mat4(1.0f),glm::vec3(1.0f,(fft->equalizerMagnitude[6]/ maxEq)+0.02,1.0f)),2);
material->setBoneUniform(glm::scale(glm::mat4(1.0f),glm::vec3(1.0f,(fft->equalizerMagnitude[7]/ maxEq)+0.02,1.0f)),3);
material->setBoneUniform(glm::scale(glm::mat4(1.0f),glm::vec3(1.0f,(fft->equalizerMagnitude[8]/ maxEq)+0.02,1.0f)),4);
if(!tex)
image->setImageData(images);
//for(o=0;o<9;o++){
 //   this->oldEqualizer[o] = equalizerMagnitude[o];
//}
//oldMax = maxEq;
if(DEBUG_MODE == 1)
std::cout<<"pbo done"<<std::endl;
delete images;
if(DEBUG_MODE == 1)
std::cout<<"image deleted"<<std::endl;
//Pa_Sleep(33);
//delete convoluted;
}

