using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TGAPE_CS.Engine.Renderer
{
    public class DrawingContext
    {
        public Camera ActiveCamera;
        public List<Light> ActiveLights;

        public DrawingContext(Camera camera, List<Light> lights)
        {
            ActiveCamera = camera;
            ActiveLights = lights;
        }
    }
}
