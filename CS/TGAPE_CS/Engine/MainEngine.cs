using CSGL;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TGAPE_CS.Engine
{
    public class MainEngine
    {
        public event Glfw3.GLFWerrorfun OnErrorCallback;
        public event Glfw3.GLFWwindowsizefun OnWindowResize;

        private IntPtr _windowPointer = IntPtr.Zero;
        private void ErrorCallback(int code, string description)
        {
            //TODO:handle glfw errors raised
            return;
        }
        private void WindowResizeCallback(IntPtr windowPointer, int width, int height)
        {
            OpenGL.glViewport(0, 0, width, height);
            Window.windowHeight = height;
            Window.windowWidth = width;
        }
        private void InitializeOpenGLParams()
        {
            OpenGL.glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
            OpenGL.glEnable(OpenGL.GL_DEPTH_TEST);
            OpenGL.glDepthMask(OpenGL.GL_TRUE);
            OpenGL.glDepthFunc(OpenGL.GL_LEQUAL);
            OpenGL.glDepthRange(0.0f, 1.0f);
            OpenGL.glEnable(OpenGL.GL_BLEND);
            OpenGL.glBlendFunc(OpenGL.GL_SRC_ALPHA, OpenGL.GL_ONE_MINUS_SRC_ALPHA);
        }
        private void InitializeGLFW()
        {
            //TODO:Remember to unsubscribe from events? Or do I even care?
            OnErrorCallback += ErrorCallback;
            OnWindowResize += WindowResizeCallback;
            //TODO:throw an exception instead?
            if (Glfw3.glfwInit() != 1)
                Environment.Exit(1);
            Glfw3.glfwSetErrorCallback(OnErrorCallback);
            int majorVersion = 0, minorVersion = 0, revision = 0;
            Glfw3.glfwGetVersion(ref majorVersion, ref minorVersion, ref revision);
            Console.WriteLine(string.Format("GLFW VERSION {0}.{1}.{2}", majorVersion, minorVersion, revision));
            Glfw3.glfwWindowHint(Glfw3.GLFW_SRGB_CAPABLE, OpenGL.GL_TRUE);
            Glfw3.glfwWindowHint(Glfw3.GLFW_SAMPLES, 4);
            _windowPointer = Glfw3.glfwCreateWindow(Window.windowWidth, Window.windowHeight, "App", IntPtr.Zero, IntPtr.Zero);
            if(_windowPointer == IntPtr.Zero)
            {
                Glfw3.glfwTerminate();
                //TODO:throw some exceptions
                Environment.Exit(1);
            }
            Glfw3.glfwSetWindowSizeCallback(_windowPointer, OnWindowResize);
            Glfw3.glfwMakeContextCurrent(_windowPointer);
            Glfw3.glfwSwapInterval(1);
            if(Glfw3.glfwExtensionSupported("GL_ARB_texture_non_power_of_two") == Glfw3.GLFW_TRUE)
            {
                Console.WriteLine("NPOT textures supported");
            }
            

        }
        public MainEngine(int windowWidth, int windowHeight)
        {
            InitializeEngine(windowWidth, windowHeight);
        }
        public void InitializeEngine(int windowWidth, int windowHeight)
        {
            Window.windowHeight = windowHeight;
            Window.windowWidth = windowWidth;
            Glfw3.csglLoadGlfw();
            InitializeGLFW();
            CSGL.CSGL.csglLoadGL();
            InitializeOpenGLParams();


        }
    }
}
