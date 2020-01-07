using CSGL;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Numerics;
using System.Text;
using System.Threading.Tasks;

namespace TGAPE_CS.Engine.Renderer.MaterialPremade
{
    public class SingleMatrixMaterial : Material
    {
        private int _colorUniformHandle;
        private int _ambientUniformHandle;
        private int _specularUniformHandle;
        private int _shininessUniformHandle;
        private int _samplerUniformHandle;
        private int _specularTextureSamplerHandle;
        private int _shininessTextureSamplerHandle;
        private int _normalMapSamplerHandle;
        private int[] _boneUniformHandles;

        protected Matrix4x4 _worldToCameraTransformationMatrix;

        public int[] BoneUniformHandles
        {
            get => _boneUniformHandles;
            private set => _boneUniformHandles = value;
        }

        private void SetUniformColor(Vector4 color)
        {
            float[] vectorArray = new float[4];
            color.CopyTo(vectorArray);
            OpenGL.glUniform4fv(_colorUniformHandle, 1, ref vectorArray[0]);
        }
        private void SetUniformAmbient(Vector4 color)
        {
            float[] vectorArray = new float[4];
            color.CopyTo(vectorArray);
            OpenGL.glUniform4fv(_ambientUniformHandle
, 1, ref vectorArray[0]);
        }
        private void SetUniformSpecular(Vector4 color)
        {
            float[] vectorArray = new float[4];
            color.CopyTo(vectorArray);
            OpenGL.glUniform4fv(_specularUniformHandle, 1, ref vectorArray[0]);
        }
        private void SetUniformShininess(float shininess)
        {
            OpenGL.glUniform1f(_shininessUniformHandle, shininess);
        }
        private void SetUniformSampler(int sampler)
        {
            OpenGL.glUniform1i(_samplerUniformHandle, sampler);
        }
        private void SetUniformNormalSampler(int sampler)
        {
            OpenGL.glUniform1i(_normalMapSamplerHandle, sampler);
        }
        private void SetUniformSpecularSampler(int sampler)
        {
            OpenGL.glUniform1i(_specularTextureSamplerHandle, sampler);
        }
        private void SetUniformShininessSampler(int sampler)
        {
            OpenGL.glUniform1i(_shininessTextureSamplerHandle, sampler);
        }

        public SingleMatrixMaterial(string vertexFilePath, string fragmentFilePath, Vector4? ambientColor = null, Vector4? diffuseColor = null, Vector4? specularColor = null, float? shininess = null) : base(vertexFilePath, fragmentFilePath)
        {
            OpenGL.glUseProgram(_shaderProgramHandle);

            _colorUniformHandle = OpenGL.glGetUniformLocation(_shaderProgramHandle, "diffuse");
            _ambientUniformHandle = OpenGL.glGetUniformLocation(_shaderProgramHandle, "ambient");
            _specularUniformHandle = OpenGL.glGetUniformLocation(_shaderProgramHandle, "specular");
            _shininessUniformHandle= OpenGL.glGetUniformLocation(_shaderProgramHandle, "shininess");
            _samplerUniformHandle = OpenGL.glGetUniformLocation(_shaderProgramHandle, "texture1");
            _normalMapSamplerHandle = OpenGL.glGetUniformLocation(_shaderProgramHandle, "normalMap");
            _specularTextureSamplerHandle = OpenGL.glGetUniformLocation(_shaderProgramHandle, "specTexMap");
            _shininessTextureSamplerHandle= OpenGL.glGetUniformLocation(_shaderProgramHandle, "shininessMap");

            SetUniformAmbient(ambientColor != null ? ambientColor.Value : new Vector4(0.1f, 0.1f, 0.1f, 1.0f));
            SetUniformColor(diffuseColor != null ? diffuseColor.Value : new Vector4(0.5f, 0.5f, 0.5f, 1.0f));
            SetUniformSpecular(specularColor != null ? specularColor.Value : new Vector4(0.5f, 0.5f, 0.5f, 1.0f));
            SetUniformShininess(shininess != null ? shininess.Value : 128.0f);

            //TODO:dynamic bone number
            _boneUniformHandles = new int[14];
            for (int i = 0; i < 14; i++)
            {
                _boneUniformHandles[i] = OpenGL.glGetUniformLocation(_shaderProgramHandle, string.Format("bone[{0}]", i.ToString("d")));
            }

        }
    }
}
