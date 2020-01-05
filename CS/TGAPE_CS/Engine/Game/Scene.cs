using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using TGAPE_CS.Engine.Renderer;

namespace TGAPE_CS.Engine.Game
{
    public class Scene
    {
        protected List<Entity> Entities;

        public void Draw(DrawingContext context)
        {
            foreach(var entity in Entities)
            {

            }
        }
    }
}
