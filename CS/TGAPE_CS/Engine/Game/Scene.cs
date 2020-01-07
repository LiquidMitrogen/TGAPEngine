using System;
using System.Collections.Generic;
using System.Linq;
using System.Numerics;
using System.Text;
using System.Threading.Tasks;
using TGAPE_CS.Engine.Renderer;

namespace TGAPE_CS.Engine.Game
{
    public class Scene
    {
        public List<Entity> Entities { get; set; }
        public Scene()
        {
            Entities = new List<Entity>();
        }
        public Scene(IEnumerable<Entity> entities)
        {
            Entities = new List<Entity>(entities);
        }

        public void Draw(DrawingContext context)
        {
            foreach(var entity in Entities)
            {
                entity.Draw(Matrix4x4.Identity, context);
            }
        }
        public Entity FindEntityByName(string name)
        {
            return Entities.FirstOrDefault(x => x.name.Equals(name));
        }
    }
}
