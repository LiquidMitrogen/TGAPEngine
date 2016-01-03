#ifndef MAINENGINE_H
#define MAINENGINE_H
#include<iostream>
#include<iomanip>
#define GLEW_STATIC
#include <GL/glew.h>
#define GLFW_DLL
#include <GLFW/glfw3.h>
#include "Config.h"


#include "Image.h"
#include "Material.h"
#include "VertexAttributes.h"
#include "TimerController.h"

#include "TimerEvent.h"
#include "BamFileReader.h"
#include "Entity.h"
#include "SingleMatrixMaterial.h"
#include "Utility.h"
#include "AnimatedActor.h"
#include "SingleTextureMatrixMaterial.h"
#include "Targa.h"
#include "Renderer.h"
#include "Window.h"
#include "FontRasterizer.h"





namespace engine{

class MainEngine
{
    public:
		MainEngine(unsigned int width, unsigned int height);
        virtual ~MainEngine();
        void mainLoop();
		void mainSingleLoop();
		int initialise(unsigned int width, unsigned int height);
        void testing();
		void setRenderer(Renderer * renderer){ this->m_renderer = renderer; };
		GLFWwindow* getWindow(){ return this->m_window; };

        static TimerController tController;
		

    protected:
        GLFWwindow* m_window;
		Renderer *m_renderer;

    private:
		
		
};
}

#endif // MAINENGINE_H
