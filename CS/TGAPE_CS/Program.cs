using CSGL;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TGAPE_CS
{
    public static class Program
    {
        public static void Main()
        {
            Glfw3.csglLoadGlfw();
            Glfw3.glfwInit();
            Glfw3.glfwWindowHint(Glfw3.GLFW_CONTEXT_VERSION_MAJOR, 4); // Change this to your targeted major version
            Glfw3.glfwWindowHint(Glfw3.GLFW_CONTEXT_VERSION_MINOR, 5); // Change this to your targeted minor version

            IntPtr window = Glfw3.glfwCreateWindow(640, 480, "CSGL Window", IntPtr.Zero, IntPtr.Zero);

            Glfw3.glfwMakeContextCurrent(window);


            CSGL.CSGL.csglLoadGL();
        }
    }
}
