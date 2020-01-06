using CSGL;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Numerics;
using System.Text;
using System.Threading.Tasks;
using TGAPE_CS.Engine.Game;

namespace TGAPE_CS.Engine.Renderer
{
    public class SceneRenderer
    {
        private const bool _isShadowMapping = false;
        private bool _isClearDisabled = false;

        private uint _shadowMapFBOHandle = 0; 
        private uint _shadowMapTextureHandle = 0; 
        private uint _shadowMapSamplerHandle = 0;
        private Scene _activeScene;
        private Scene _auxScene;
        private DrawingContext _drawingContext;
        private List<Light> _lights;
        private Camera _drawPassCamera;
        private Camera _shadowPassCamera;

        public static Matrix4x4 WorldToCameraTransformationMatrix;


        public SceneRenderer(Camera drawingPassCamera, Light light)
        {
            if (_isShadowMapping)
            {
                //TODO: shadow mapping
            }
            _drawPassCamera = drawingPassCamera;
            _lights = new List<Light>
            {
                light
            };
            _drawingContext = new DrawingContext(drawingPassCamera, _lights);
            _auxScene = null;
            _activeScene = null;
        }
        public void DrawingPass()
        {
            WorldToCameraTransformationMatrix = _drawPassCamera.WorldToCameraSpaceTransformationMatrix;
            OpenGL.glCullFace(OpenGL.GL_BACK);
            if (!_isClearDisabled)
                OpenGL.glClear(OpenGL.GL_COLOR_BUFFER_BIT | OpenGL.GL_DEPTH_BUFFER_BIT);
            _drawingContext.ActiveLights = _lights;//TODO:this is unnecessary since this is just a reference
            if (_activeScene != null)
                _activeScene.Draw(_drawingContext);
            if (_auxScene != null)
                _auxScene.Draw(_drawingContext);
        }
    }
}
