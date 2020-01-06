using CSGL;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using TGAPE_CS.Engine.Renderer;

namespace TGAPE_CS.Engine
{
    public class MainEngine
    {
        private SceneRenderer sceneRenderer;
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
            if (_windowPointer == IntPtr.Zero)
            {
                Glfw3.glfwTerminate();
                //TODO:throw some exceptions
                Environment.Exit(1);
            }
            Glfw3.glfwSetWindowSizeCallback(_windowPointer, OnWindowResize);
            Glfw3.glfwMakeContextCurrent(_windowPointer);
            Glfw3.glfwSwapInterval(1);
            if (Glfw3.glfwExtensionSupported("GL_ARB_texture_non_power_of_two") == Glfw3.GLFW_TRUE)
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
        public void MainLoop()
        {
            while(Glfw3.glfwWindowShouldClose(_windowPointer) == Glfw3.GLFW_FALSE)
            {
                if (sceneRenderer != null)
                {
                    OpenGL.glClear(OpenGL.GL_COLOR_BUFFER_BIT | OpenGL.GL_DEPTH_BUFFER_BIT);
                    sceneRenderer.DrawingPass();
                }
                Glfw3.glfwSwapBuffers(_windowPointer);
                Glfw3.glfwPollEvents();
                //TODO:do a C# Stopwatch to measure time
                Thread.Sleep(30);

            }
            
        }
        public void Testing()
        {

        }
    }
}
