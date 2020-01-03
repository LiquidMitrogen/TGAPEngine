using System;
using System.Collections.Generic;
using System.Linq;
using System.Numerics;
using System.Text;
using System.Threading.Tasks;

namespace TGAPE_CS.Engine.Renderer
{//TODO:make light an entity so it can be used in object tree if possible if not change to a struct
    public class Light
    {
        public bool IsDirectional { get; set; }
        public Vector4 Direction { get; set; }
        public Vector3 Color { get; set; }
        public Vector4 Location { get; set; }
        public Light(Vector4 direction, Vector3 color, Vector4 location, bool isDirectional)
        {
            Direction = direction;
            Color = color;
            Location = location;
            IsDirectional = isDirectional;
        }
    }
}
