using CSGL;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Numerics;
using System.Text;
using System.Threading.Tasks;
using TGAPE_CS.Engine;
using TGAPE_CS.Engine.Loader;
using TGAPE_CS.Engine.Renderer;

namespace TGAPE_CS
{
    public static class Program
    {
        public static void Main()
        {
            var engine = new MainEngine(800, 600);
            engine.SceneRenderer = new SceneRenderer(
                Camera.ConstructAPerspectiveCamera(1.0f, 4.0f / 3.0f, 1.0f, 50.0f, new Vector3(0.0f, 0.0f, 0.0f), -Vector3.UnitZ, Vector3.UnitY), 
                new Light(new Vector4(0.0f, 0.0f, 1.0f, 1.0f), new Vector3(0.7f), Vector4.Zero, true));
            var scene = BamFileReader.LoadScene(@"C:\Users\szymo\Documents\TGAPEngine\TGAPEngine\res\cube.bams");
            engine.SceneRenderer.ActiveScene = scene;
            engine.MainLoop();
            Console.WriteLine("fffffffuuuuuuu");
        }
    }
}
