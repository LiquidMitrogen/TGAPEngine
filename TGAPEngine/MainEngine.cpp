#include "MainEngine.h"



namespace engine{
Entity *szklanka, * woda,*strzalka;
SingleMatrixMaterial *szklankamat, * wodamat, *strzalkamat;
bool mouseDown = false,fire=false;
double lastX;
int widthW = 1000;
int heightW = 600;
void window_size_callback(GLFWwindow* window, int width, int height)
{
        glViewport(0, 0, width, height);
		Window::m_width = width;
		Window::m_height = height;
}
/*
void mouseButtCallback(GLFWwindow *window, int button, int action, int mods){
    if(button == GLFW_MOUSE_BUTTON_LEFT){
        if(action == GLFW_PRESS){
            mouseDown = true;
            glfwGetCursorPos(window,&lastX,NULL);
        }
        if(action == GLFW_RELEASE){
            mouseDown = false;
        }
    }
}

void mousePosCallback(GLFWwindow *window, double x, double y){
    if(mouseDown){
        angle +=  -(x - lastX) / 100.0;
        lastX = x;
    }
}
void keyCallback(GLFWwindow * window,int key,int scancode,int action,int mods){

    if(action == GLFW_PRESS && key == GLFW_KEY_F){
            std::cout<<"keys"<<std::endl;
        //drawFfftResults = !drawFfftResults;
    }
    if(action == GLFW_PRESS && key == GLFW_KEY_W){
            
        //drawFfftResults = !drawFfftResults;
    }
    if(action == GLFW_PRESS && key == GLFW_KEY_S){
            
        //drawFfftResults = !drawFfftResults;
    }
    if(action == GLFW_PRESS && key == GLFW_KEY_EQUAL){
            
        //drawFfftResults = !drawFfftResults;
    }
    if(action == GLFW_PRESS && key == GLFW_KEY_MINUS){
            
        //drawFfftResults = !drawFfftResults;
    }
    if(action == GLFW_PRESS && key == GLFW_KEY_P){
            std::cout<<"keys"<<std::endl;
       // drawPompki = !drawPompki;
    }
    if(action == GLFW_PRESS && key == GLFW_KEY_E){
            std::cout<<"keys"<<std::endl;
       // drawEq = !drawEq;
    }
    if(action == GLFW_PRESS && key == GLFW_KEY_SPACE){
            fire = true;

       // tex = !tex;
    }
    if(action == GLFW_PRESS && key == GLFW_KEY_R){
            
       // drawPompki = !drawPompki;
    }
    if(action == GLFW_PRESS && key == GLFW_KEY_ENTER){
           // std::cout<<"easy = "<<easy<<std::endl;
       // easy = !easy;
    }
    if(action == GLFW_PRESS && key == GLFW_KEY_UP){
           
    }
    if(action == GLFW_PRESS && key == GLFW_KEY_DOWN){
           
    }
    if(action == GLFW_PRESS && key == GLFW_KEY_RIGHT){
           
    }
    if(action == GLFW_PRESS && key == GLFW_KEY_LEFT){
           
    }
} */
void error_callback(int error, const char* description)
{
std::cerr<<description<<std::endl;
}

MainEngine::MainEngine(unsigned int width, unsigned int height)
{
	initialise(width, height);
	this->m_renderer = NULL;
}

MainEngine::~MainEngine()
{
	glfwDestroyWindow(m_window);
    glfwTerminate();
}

int MainEngine::initialise(unsigned int width, unsigned int height)
{
	Window::m_width = width;
	Window::m_height = height;
    if (!glfwInit())
        return(1);
    glfwSetErrorCallback(error_callback);
	glfwWindowHint(GLFW_SRGB_CAPABLE, GL_TRUE);
	glfwWindowHint(GLFW_SAMPLES, 4);
	
	this->m_window = glfwCreateWindow(Window::m_width, Window::m_height, "App", NULL, NULL);
	int mj, mi, rev;
	glfwGetVersion(&mj, &mi, &rev);
	std::cout << "GLFW " << mj << "." << mi << "." << rev << std::endl;
	
	
    //glfwSetKeyCallback(this->window,keyCallback);
    //glfwSetMouseButtonCallback(this->window,mouseButtCallback);
    //glfwSetCursorPosCallback(this->window,mousePosCallback);
	glfwSetWindowSizeCallback(m_window, window_size_callback);
	if (!m_window)
    {
        glfwTerminate();
        return 1;
    }
	glfwMakeContextCurrent(m_window);
	//glfwSwapInterval(0);
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
      std::cerr<<"Error: "<<glewGetErrorString(err)<<std::endl;
    }
    std::cout<<"Status: Using GLEW "<<glewGetString(GLEW_VERSION)<<std::endl;
    if(GLEW_VERSION_4_0){
        std::cout<<"jest 4.0";
    }
	if (glewGetExtension("GL_ARB_texture_non_power_of_two")){
		std::cout << "jest NPOT" << std::endl;
	}

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LEQUAL);
	glDepthRange(0.0f, 1.0f);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    return 0;
}

void MainEngine::testing()
{
	/*
const float vertexPos[]{
1.0,1.0,1.0,
1.0,-1.0,1.0,
-1.0,-1.0,1.0,
-1.0,1.0,1.0,
-1.0,1.0,-1.0,
-1.0,-1.0,-1.0,
1.0,-1.0,-1.0,
1.0,1.0,-1.0


};

const unsigned int indicesTex[]{
0,1,2,
0,2,3,
//
0,7,1,
1,7,6,
//
4,5,2,
3,4,2,
//
4,5,6,
6,7,4,
//
0,7,4,
3,0,4,
//
2,1,5,
1,6,5
};




std::vector<char> poses((char *)vertexPos,((char *) vertexPos )+ sizeof(vertexPos));
std::vector<unsigned int> indyki(indicesTex, indicesTex + sizeof(indicesTex) / sizeof(unsigned int) );
//std::cout<<" asdasda "<<sizeof(indicesTex) / sizeof(unsigned int)<<std::endl;
printf("\n%llu\n",poses.size()); 
engine::VaoInitData vid,vid2;





vid2.attrNumber=1;
vid2.attrNormBool[0]=GL_FALSE;
vid2.attrOffset[0]=0;
vid2.attrSize[0]=3;
vid2.attrType[0]=GL_FLOAT;

*/

engine::BamFileReader bam;

//engine::VertexAttributes * vaa;
//vaa = new VertexAttributes(poses,indyki,vid2);
//engine::VertexAttributes * vaa2;
//vaa2 = new VertexAttributes(poses,indyki,vid2);

//ent = new engine::Entity(vaa);

//ent->entityMaterial = mat;
//ent->entityMaterial->use();

//kula = new engine::Entity(vaa);
//kulamat = new SingleMatrixMaterial("vertex.vert","fragment.frag");
//kula->entityMaterial = kulamat;
//kula->entityMaterial->use();


//std::cout<<"ojej"<<std::endl;
/*
szklanka = bam.loadFile("szklankav2.bam");
woda = bam.loadFile("wodav2.bam");
strzalka = bam.loadFile("strzalkav2.bam");
//zamek = new engine::Entity(vaa);
//std::cout<<"ojej"<<std::endl;
szklankamat = new SingleMatrixMaterial("vertex.vert","fragment.frag");
wodamat = new SingleMatrixMaterial("vertex.vert","fragment.frag");
strzalkamat = new SingleMatrixMaterial("vertex.vert","fragment.frag");
szklanka->entityMaterial = szklankamat;
woda->entityMaterial = wodamat;
strzalka->entityMaterial = strzalkamat;

woda->entityMaterial->use();
wodamat->setUniformColor(glm::vec4(0.3f,0.3f,1.0f,1.0f));

szklanka->entityMaterial->use();
szklankamat->setUniformColor(glm::vec4(0.5f,0.5f,0.5f,1.0f));
szklankamat->setUniformAmbient(glm::vec4(0.21f,0.21f,0.21f,1.0f));
szklankamat->setUniformSpecular(glm::vec4(1.0f,1.0f,1.0f,1.0f));
szklankamat->setUniformShininess(128);

strzalka->entityMaterial->use();
strzalkamat->setUniformColor(glm::vec4(1.0f,0.0f,0.0f,1.0f));
strzalkamat->setUniformAmbient(glm::vec4(0.41f,0.11f,0.11f,1.0f));
strzalkamat->setUniformSpecular(glm::vec4(1.0f,1.0f,1.0f,1.0f));
strzalkamat->setUniformShininess(128);
//zamek->applyTranslation(glm::vec3(0.0f,0.0f,10.0f));
*/




glClearColor(1.0f,1.0f,1.0f,1.0f);
glEnable(GL_DEPTH_TEST);
glDepthMask(GL_TRUE);
glDepthFunc(GL_LEQUAL);
glDepthRange(0.0f, 1.0f);
glEnable(GL_BLEND);
glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


}


void MainEngine::mainLoop()
{


if (m_renderer == NULL){
	std::cout << "renderer is not initialized" << std::endl;
	exit(1);
}
    while (!glfwWindowShouldClose(m_window))//looooooop
    {
        if(DEBUG_MODE == 1)
        std::cout<<"clearing"<<std::endl;
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (DEBUG_MODE == 1)
			std::cout << "drawing - calling Renderer" << std::endl;
		m_renderer->drawingPass();//drawing


        if(DEBUG_MODE == 1)
        std::cout<<"swapping framebuffer"<<std::endl;
		glfwSwapBuffers(m_window);
        glfwPollEvents();



        Sleep(33);
    // Keep running
    }
}
void MainEngine::mainSingleLoop(){
	if (m_renderer == NULL){
		std::cout << "renderer is not initialized" << std::endl;
		exit(1);
	}
	
		/*if (DEBUG_MODE == 1)
			std::cout << "clearing" << std::endl;
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
*/
		if (DEBUG_MODE == 1)
			std::cout << "drawing - calling Renderer" << std::endl;
		m_renderer->drawingPass();//drawing


		if (DEBUG_MODE == 1)
			std::cout << "swapping framebuffer" << std::endl;
		glfwSwapBuffers(m_window);
		glfwPollEvents();



		
		// Keep running
	
}
}
