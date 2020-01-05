using CSGL;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Numerics;
using System.Text;
using System.Threading.Tasks;

namespace TGAPE_CS.Engine.Renderer
{
    public class Renderer
    {
        private const bool _isShadowMapping = false;
        private bool _isClearDisabled = false;

        private uint _shadowMapFBOHandle = 0; 
        private uint _shadowMapTextureHandle = 0; 
        private uint _shadowMapSamplerHandle = 0; 



        public Renderer()
        {


        }
    }
}
