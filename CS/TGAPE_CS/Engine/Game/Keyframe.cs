using System;
using System.Collections.Generic;
using System.Linq;
using System.Numerics;
using System.Text;
using System.Threading.Tasks;

namespace TGAPE_CS.Engine.Game
{
    public struct Keyframe
    {
        public uint frame;
        public Quaternion rotation;
        public Vector3 location;
        public Keyframe(uint frame, Vector3 location, Quaternion rotation)
        {
            this.frame = frame;
            this.location = location;
            this.rotation = rotation;
        }
    }
}
