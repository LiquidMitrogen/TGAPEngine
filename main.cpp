
#include "MainEngine.h"
#include "Bone.h"
#include "Camera.h"
#include "Light.h"
#include "BamFileReader.h"
#include "Entity.h"
#include "Material.h"
#include "SingleMatrixMaterial.h"
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include "FontRasterizer.h"
#include "Image.h"
#include "Utility.h"
#include "TextboxEntity.h"
#include "ParticleSystem.h"

#include <ft2build.h>
#include FT_FREETYPE_H

bool mouseDown = false;

void mouseButtCallback(GLFWwindow *window, int button, int action, int mods){
	if (button == GLFW_MOUSE_BUTTON_LEFT){
		if (action == GLFW_PRESS){
			mouseDown = true;
		}
		if (action == GLFW_RELEASE){
			mouseDown = false;
		}
	}
}
int main(){


	engine::BamFileReader bam;
	engine::MainEngine me(800,600);
	//engine::Camera * mc = new engine::Camera(glm::perspective(1.182664626f, 9.0f / 6.0f, 1.0f, 750.0f), glm::vec3(1.0f, 0.0f, 15.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	engine::Camera * mc = engine::Camera::perspectiveCamera(1.182664626f, 9.0f / 6.0f, 1.0f, 750.0f, 0.5f, 2.0f, 2.0f, 0.5f, 2.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	engine::Light * mdlight = new engine::Light(1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 10.0f,false);
	engine::Renderer *mr = new engine::Renderer(mc, mdlight);
	me.setRenderer(mr);


	TARGAINFO info;
	GLubyte * cbit = LoadTGAImage((char *)"nor5.tga", &info, true);
	engine::Image * cimg = new engine::Image(cbit, info.width, info.height, 1, GL_RGB, TG_FILTER_LINEAR, true);

	engine::AnimatedActor * e = bam.loadFile("p2reduweightt.bam");
	engine::Bone * bonePtr = e->getArmatureRoot()->findBoneByName("Neck");
	
	

	if (bonePtr != NULL){
		//e->applyRotationToBoneAtFrame(bonePtr, 1, glm::angleAxis((float)M_PI / 4.0f, glm::vec3(0.0f, 1.0f, 0.0f)));
	}
	else{
		std::cout << "bone not found" << std::endl;
	}
	//std::cout << boneIndex << " "  << std::endl;
	
	
	//e->addImage(cimg);
	SingleMatrixMaterial * mat = new SingleMatrixMaterial("D:\\pro\\shadery\\bone\\vertex.vert", "D:\\pro\\shadery\\Blinn-Phong\\fragment.frag");
	e->entityMaterial = mat;
	e->setAmimationFrame(1);
	e->calcBoneUniforms();
	mat->setUniformSampler(1);
	mat->setUniformAmbient(glm::vec4(0.3f, 0.3f, 0.3f, 1.0f));
	mat->setUniformColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
	mat->setUniformShininess(128.0f);
	mat->setUniformSpecular(glm::vec4(0.5f, 0.5f, 0.5f,1.0f));
	mr->addEntity(e);
	//e->applyTranslation(glm::vec3(0.0f, 0.0,0.0));
	//e->applyRotation(glm::angleAxis((float)M_PI, glm::vec3(0.0f, 1.0f, 0.0)));


	engine::FontRasterizer * ft = new engine::FontRasterizer();
	if (DEBUG_MODE == 1){
		std::cout << "new main" << std::endl;
	}
	ft->loadFont("Hack-Regular.ttf");
	//FT_GlyphSlot slot = ft.getGlyphBitmap(0, 30, 0x00000061);
	unsigned int bitmapWidth;
	//GLubyte * bitmap = ft->getTextBitmap(128, 128, "Dobra, Koniec zabawy",&bitmapWidth);

	/*TARGAINFO info;
	GLubyte * imagePtrspc = LoadTGAImage((char *)"1.tga", &info,true);
	std::cout << info.height << " " << info.width << " " << info.bpp << std::endl;
	engine::Image * spaceshipTexture = new engine::Image(imagePtrspc, info.width, info.height, 1, GL_RED);*/

	//GLubyte * bitmap = ft.getGlyphBitmapBytes(10, 10, 0x00000066);
	

	/*GLubyte * watafak = new GLubyte[512 * 512];
	for (int w = 0; w < 512 * 512; w++){
		watafak[w] = 0;
	}*/

	//engine::blitChar(slot->bitmap.buffer, watafak, 0, 0, slot->bitmap.width, slot->bitmap.rows, 512, 512);

	//engine::Image * image = new engine::Image(bitmap, bitmapWidth, 128, 1, GL_RED, TG_FILTER_MIPMAP_LINEAR);

	//engine::TextboxEntity * ei = new engine::TextboxEntity(bam.loadNoBoneFile("wAtf2.bam"),"zle ale stabilnie jest w rgba",ft);
	//if (DEBUG_MODE == 1){
	//	std::cout << "new main" << std::endl;
	//}
	//ei->setTextHeight(64);
	//SingleMatrixMaterial * mat2 = new SingleMatrixMaterial("Text.vert", "Text.frag");
	//if (DEBUG_MODE == 1){
	//	std::cout << "new main" << std::endl;
	//}
	//ei->entityMaterial = mat2;
	////ei->addImage(image);
	//mat2->setUniformColor(glm::vec4(1.0f, 0.1f, 0.1f, 1.0f));
	//mat2->setUniformAmbient(glm::vec4(0.2f, 0.05f, 0.05f, 1.0f));
	//mat2->setUniformSampler(1);
	////ei->applyRotation(glm::angleAxis((float)M_PI /2, glm::vec3(0.0f, -1.0f, 0.0f)));

	////mr->addEntity(ei);
	////e->addChild(ei);
	//ei->applyTranslation(glm::vec3(0.0f, 0.0f, 9.0f));
	//ei->applyScale(glm::vec3(4.0f, 1.0f, 1.0f));


	GLubyte * imagePtrstar = LoadTGAImage((char *)"star.tga", &info, true);
	/*for (int i = 0; i < info.height * info.width * info.bpp; i++){
		std::cout <<(unsigned int) imagePtrstar[i] << std::endl;
	}*/
	engine::Image * star = new engine::Image(imagePtrstar, info.width, info.height, 1, GL_RGBA, TG_FILTER_NEAREST, true);
	engine::MagicPSCallbacks * fpsc = new engine::MagicPSCallbacks;
	engine::ParticleSystem * fps = engine::ParticleSystem::createSquareParticleSystem(2000, fpsc);
	fps->start(15);
	fps->addImage(star);
	fps->setTimeBetweenParticles(0.0001);
	//fps->applyRotation(glm::angleAxis(-(float)M_PI / 2, glm::vec3(1.0f, 0.0f, 0.0f)));
	fps->applyTranslation(glm::vec3(0.0f, 2.0f, 0.0f));
	//fps->pause(true);
	SingleMatrixMaterial * matf = new SingleMatrixMaterial("D:\\pro\\shadery\\particleSystem\\vertex.vert", "D:\\pro\\shadery\\particleSystem\\fragment.frag");
	fps->entityMaterial = matf;
	matf->setUniformSampler(1);
	matf->setUniformAmbient(glm::vec4(0.0f, 0.5f, 0.0f, 0.5f));
	mr->addEntity(fps);

	//engine::Entity * guiPlane = bam.loadNoBoneFile("wAtf2.bam");
	//GLubyte * imagePtrspc = LoadTGAImage((char *)"st.tga", &info, true);
	//engine::Image * guiPlaneTex = new engine::Image(imagePtrspc, info.width, info.height, 1, GL_RGBA, TG_FILTER_LINEAR, true);
	//SingleMatrixMaterial * matGUI = new SingleMatrixMaterial("GUI.vert", "GUI.frag");
	//if (DEBUG_MODE == 1){
	//	std::cout << "new main" << std::endl;
	//}
	//guiPlane->addImage(guiPlaneTex);
	//matGUI->use();
	//matGUI->setUniformSampler(1);
	//guiPlane->applyTranslation(glm::vec3(0.0f, 0.0f, 0.0f));
	//matGUI->setUniformAmbient(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
	//matGUI->setUniformColor(glm::vec4(0.3f, 0.5f, 0.2f, 1.0f));
	//matGUI->setUniformSpecular(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	//guiPlane->entityMaterial = matGUI;
	////mr->addEntity(guiPlane);
	//glm::vec4 farP = glm::vec4(-1.0f, -1.0f, -750.0f, 1.0f);
	//glm::vec4 farPC = mc->getCameraToClipMatrix()  * farP;
	//std::cout << farPC.x << " " << farPC.y << " " << farPC.z << " " << farPC.w << std::endl;

//m.testing();
	glfwSetMouseButtonCallback(me.getWindow(), mouseButtCallback);
	glm::quat rotationQuat = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
	double xpos, ypos;
	int x, y;
	float time = 0;
	float tmpsc = 0;
	float angle = -(float)M_PI / 200.0f;
	double timeS = 0, timeE = 0;
	float timePassed = 0;
	while (!glfwWindowShouldClose(me.getWindow())){
		timePassed = timeE - timeS;
		//std::cout << 1 / timePassed << std::endl;
		timeS = glfwGetTime();
		fps->simulate(timePassed);
		//std::cout << fps->getParticleCount() << std::endl;
		/*if (time >= 8000){
			time = 0;
			angle = -angle;
		}
		tmpsc = (time / 8000) * M_PI * 2;*/
		if (mouseDown == true){
			mouseDown = false;
			glfwGetCursorPos(me.getWindow(), &xpos, &ypos);
			glfwGetWindowSize(me.getWindow(), &x, &y);
			float xmouse = (xpos / x) * 2 - 1;
			float ymouse = -((ypos / y) * 2 - 1);
			glm::vec3 mfpl = mc->getMouseOnFarPlaneWorldSpace(xmouse, ymouse);
			std::cout << mfpl.x << " " << mfpl.y << " " << mfpl.z << std::endl;
			glm::vec3 bonepos = bonePtr->getLocationForFrame(1);
			e->applyRotationToBoneAtFrame(bonePtr, 1, glm::inverse(rotationQuat));
			rotationQuat = engine::twoVectorsToQuat(glm::normalize(mfpl - bonepos), glm::vec3(0.0f, 0.0f, -1.0f) );
			e->applyRotationToBoneAtFrame(bonePtr, 1, rotationQuat);
		}
		
		//mdlight->setLocation(glm::vec4(sin(tmpsc) * 7.0f, 0.0f, cos(tmpsc) * 7.0f, 1.0f));
		//e->applyRotation(glm::angleAxis((float)M_PI / 800.0f, glm::vec3(0.0f, 1.0f, 0.0)));
		//e->applyRotationToBoneAtFrame(bonePtr, 1, glm::angleAxis(angle, glm::vec3(0.0f, 1.0f, 0.0f)));

		me.mainSingleLoop();
		//Sleep(33);
		timeE = glfwGetTime();
		time += timePassed;
		
	}
	//me.mainLoop();


return 0;
};
