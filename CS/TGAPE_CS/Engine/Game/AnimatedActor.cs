using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using TGAPE_CS.Engine.Renderer;
using TGAPE_CS.Engine.Renderer.MaterialPremade;

namespace TGAPE_CS.Engine.Game
{
    public class AnimatedActor : Entity
    {
        public Armature Armature { get; set; }
        public string ArmatureName { get; set; }
        public AnimatedActor(VertexAttributes vertexAttributes, string armatureName) : base(vertexAttributes)
        {
            this.ArmatureName = armatureName;
        }

        public AnimatedActor(Entity other) : base(other)
        {
            if(other is AnimatedActor)
            {
                //TODO:also clone the armature
                this.Armature = (other as AnimatedActor).Armature;

            }
        }
        public void PrepareArmature()
        {
            //todo:material overhaul
            if (this.entityMaterial is SingleMatrixMaterial)
                Armature.UpdateArmatureUniforms((entityMaterial as SingleMatrixMaterial).BoneUniformHandles);
            //->modifyArmatureUniforms(((SingleMatrixMaterial*)this->entityMaterial)->getBoneUniforms());

        }
        public VertexAttributes GetVertexAttributes()
        {
            return base._vertexAttributes;
        }
        override public void Prepare()
        {
            base.Prepare();
            PrepareArmature();
        }
    }
}
