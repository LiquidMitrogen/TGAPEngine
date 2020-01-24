using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using TGAPE_CS.Engine.Renderer;

namespace TGAPE_CS.Engine.Game
{
    public class Armature : Entity
    {
        private Bone _rootBone;
        private List<ActorAction> _actions;
        public Armature(Bone rootBone) : base((VertexAttributes) null)
        {
            _actions = new List<ActorAction>();
            _rootBone = rootBone;
        }
        private Armature(VertexAttributes vertexAttributes) : base(vertexAttributes)
        {
        }

        private  Armature(Entity other) : base(other)
        {
        }


        public void AddAction(ActorAction action)
        {
            _actions.Add(action);
        }
        public void UpdateArmatureUniforms(int[] uniforms)
        {
            _rootBone.ModifyBoneUniform(uniforms);
        }
        public Bone FindBoneByName(string name)
        {
            return _rootBone.FindBoneByName(name);
        }
        public ActorAction FindActionByName(string name)
        {
            return _actions.FirstOrDefault(x => x.name.Equals(name));
        }
        public void ApplyActionByName(string name, uint frame)
        {
            var action = _actions.FirstOrDefault(x => x.name.Equals(name));
            if (action != null)
                _rootBone.ApplyAction(action, frame);
        }
        public void ApplyAction(ActorAction action, uint frame)
        {
            _rootBone.ApplyAction(action, frame);
        }
    }
}
