using CSGL;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Numerics;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using TGAPE_CS.Engine.Math;

namespace TGAPE_CS.Engine.Renderer
{
    public class Material
    {
        protected uint _shaderProgramHandle;
        protected int _cameraToClipSpaceUniformHandle;
        protected int _worldToCameraSpaceUniformHandle;
        protected int _modelToWorldSpaceUniformHandle;
        protected int[] _lightDirectionUniformHandles;
        protected int[] _lightColorUniformHandles;
        protected int[] _pointLightUniformHandles;
        protected int _lightsCountUniformHandle;

        public Material(string vertexFilePath, string fragmentFilePath)
        {
            _lightDirectionUniformHandles = new int[4];
            _lightColorUniformHandles = new int[4];
            _pointLightUniformHandles = new int[4];

            LoadShaders(vertexFilePath, fragmentFilePath);
            OpenGL.glUseProgram(_shaderProgramHandle);

            _cameraToClipSpaceUniformHandle = OpenGL.glGetUniformLocation(_shaderProgramHandle, "cameraToClip");//TODO:do not hardcode shit like this
            _modelToWorldSpaceUniformHandle = OpenGL.glGetUniformLocation(_shaderProgramHandle, "modelToWorld");//TODO:do not hardcode shit like this
            _worldToCameraSpaceUniformHandle = OpenGL.glGetUniformLocation(_shaderProgramHandle, "worldToCamera");//TODO:do not hardcode shit like this
            for(int i = 0; i<4; i++)//TODO:add dynamic lights - not max 4 lol
            {
                _lightDirectionUniformHandles[i] = OpenGL.glGetUniformLocation(_shaderProgramHandle, string.Format("lightDir[{0}]", i));
                _lightColorUniformHandles[i] = OpenGL.glGetUniformLocation(_shaderProgramHandle, string.Format("lightCol[{0}]", i));
                _pointLightUniformHandles[i] = OpenGL.glGetUniformLocation(_shaderProgramHandle, string.Format("pointLight[{0}]", i));
            }
            _lightsCountUniformHandle = OpenGL.glGetUniformLocation(_shaderProgramHandle, "lightsNumber");
        }

        public void LoadShaders(string vertexFilePath, string fragmentFilePath)
        {
            if(!File.Exists(vertexFilePath) || !File.Exists(fragmentFilePath))
            {
                //TODO:throw?
                return;
            }
            var vertexShaderHandle = OpenGL.glCreateShader(OpenGL.GL_VERTEX_SHADER);
            var fragmentShaderHandle = OpenGL.glCreateShader(OpenGL.GL_FRAGMENT_SHADER);
            var vertexShaderCode = File.ReadAllBytes(vertexFilePath);
            var fragmentShaderCode = File.ReadAllBytes(fragmentFilePath);

            int result = OpenGL.GL_FALSE;
            int infoLogLength = 0;
            
            Console.WriteLine(string.Format("Compiling shader : {0}\n"), vertexFilePath);
            int lenghtOfVertexCode = vertexShaderCode.Length * sizeof(byte);
            IntPtr vetrexCodePointer = Marshal.AllocHGlobal(lenghtOfVertexCode);
            Marshal.Copy(vertexShaderCode, 0, vetrexCodePointer, vertexShaderCode.Length);
            OpenGL.glShaderSource(vertexShaderHandle, 1, ref vetrexCodePointer, ref lenghtOfVertexCode);
            Marshal.FreeHGlobal(vetrexCodePointer);
            OpenGL.glCompileShader(vertexShaderHandle);

            OpenGL.glGetShaderiv(vertexShaderHandle, OpenGL.GL_COMPILE_STATUS, ref result);
            OpenGL.glGetShaderiv(vertexShaderHandle, OpenGL.GL_INFO_LOG_LENGTH, ref infoLogLength);
            if(infoLogLength > 0)
            {
                int lengthOfTheBuffer = 0;
                IntPtr logBuffer = Marshal.AllocHGlobal(infoLogLength);
                OpenGL.glGetShaderInfoLog(vertexShaderHandle, infoLogLength, ref lengthOfTheBuffer, logBuffer);

                byte[] buffer = new byte[lengthOfTheBuffer];
                Marshal.Copy(logBuffer, buffer, 0, lengthOfTheBuffer);
                Marshal.FreeHGlobal(logBuffer);
                var logMessage = Encoding.ASCII.GetString(buffer);
                Console.WriteLine(logMessage);
                if (result != OpenGL.GL_TRUE)
                {
                    Debugger.Break();
                    //TODO:THROW?
                    return;
                }
            }

            result = OpenGL.GL_FALSE;
            infoLogLength = 0;
            Console.WriteLine(string.Format("Compiling shader : {0}\n"), fragmentFilePath);
            int lenghtOfFragmentCode = fragmentShaderCode.Length * sizeof(byte);
            IntPtr fragmentCodePointer = Marshal.AllocHGlobal(lenghtOfFragmentCode);
            Marshal.Copy(fragmentShaderCode, 0, fragmentCodePointer, lenghtOfFragmentCode);
            OpenGL.glShaderSource(fragmentShaderHandle, 1, ref fragmentCodePointer, ref lenghtOfFragmentCode);
            Marshal.FreeHGlobal(fragmentCodePointer);
            OpenGL.glCompileShader(fragmentShaderHandle);

            OpenGL.glGetShaderiv(fragmentShaderHandle, OpenGL.GL_COMPILE_STATUS, ref result);
            OpenGL.glGetShaderiv(fragmentShaderHandle, OpenGL.GL_INFO_LOG_LENGTH, ref infoLogLength);
            if (infoLogLength > 0)
            {
                int lengthOfTheBuffer = 0;
                IntPtr logBuffer = Marshal.AllocHGlobal(infoLogLength);
                OpenGL.glGetShaderInfoLog(fragmentShaderHandle, infoLogLength, ref lengthOfTheBuffer, logBuffer);

                byte[] buffer = new byte[lengthOfTheBuffer];
                Marshal.Copy(logBuffer, buffer, 0, lengthOfTheBuffer);
                Marshal.FreeHGlobal(logBuffer);
                var logMessage = Encoding.ASCII.GetString(buffer);
                Console.WriteLine(logMessage);
                if (result != OpenGL.GL_TRUE)
                {
                    Debugger.Break();
                    //TODO:THROW?
                    return;
                }
            }

            result = OpenGL.GL_FALSE;
            infoLogLength = 0;
            Console.WriteLine("Linking program\n");
            uint programHandle = OpenGL.glCreateProgram();
            OpenGL.glAttachShader(programHandle, vertexShaderHandle);
            OpenGL.glAttachShader(programHandle, fragmentShaderHandle);
            OpenGL.glBindFragDataLocation(programHandle, 0, "colorOut");//TODO:this could be a metadata thingie not hardcoded
            OpenGL.glLinkProgram(programHandle);

            OpenGL.glGetProgramiv(programHandle, OpenGL.GL_LINK_STATUS, ref result);
            OpenGL.glGetProgramiv(programHandle, OpenGL.GL_INFO_LOG_LENGTH, ref infoLogLength);
            if (infoLogLength > 0)
            {
                int lengthOfTheBuffer = 0;
                IntPtr logBuffer = Marshal.AllocHGlobal(infoLogLength);
                OpenGL.glGetProgramInfoLog(programHandle, infoLogLength, ref lengthOfTheBuffer, logBuffer);

                byte[] buffer = new byte[lengthOfTheBuffer];
                Marshal.Copy(logBuffer, buffer, 0, lengthOfTheBuffer);
                Marshal.FreeHGlobal(logBuffer);
                var logMessage = Encoding.ASCII.GetString(buffer);
                Console.WriteLine(logMessage);
                if (result != OpenGL.GL_TRUE)
                {
                    Debugger.Break();
                    //TODO:THROW?
                    return;
                }
            }

            OpenGL.glDeleteShader(vertexShaderHandle);
            OpenGL.glDeleteShader(fragmentShaderHandle);

            _shaderProgramHandle = programHandle;
        }
        public void Use()
        {
            OpenGL.glUseProgram(_shaderProgramHandle);
        }
        public void SetUniformModelToWorld(Matrix4x4 modelToWorld)
        {
            var matrixAsArray = MatrixHelper.ConvertToAnArray(modelToWorld);
            OpenGL.glUniformMatrix4fv(_modelToWorldSpaceUniformHandle, 1, OpenGL.GL_TRUE, ref matrixAsArray[0]);
        }
        public void SetUniformCameraToClip(Matrix4x4 cameraToClip)
        {
            var matrixAsArray = MatrixHelper.ConvertToAnArray(cameraToClip);
            OpenGL.glUniformMatrix4fv(_cameraToClipSpaceUniformHandle, 1, OpenGL.GL_TRUE, ref matrixAsArray[0]);
        }
        public void SetUniformWorldToCamera(Matrix4x4 worldToCamera)
        {
            var matrixAsArray = MatrixHelper.ConvertToAnArray(worldToCamera);
            OpenGL.glUniformMatrix4fv(_worldToCameraSpaceUniformHandle, 1, OpenGL.GL_TRUE, ref matrixAsArray[0]);
        }
        public void SetUniformLightDirection(Vector4 lightDirection, int lightIndex)
        {
            float[] vectorAsArray = new float[4];
            lightDirection.CopyTo(vectorAsArray);
            OpenGL.glUniform4fv(_lightDirectionUniformHandles[lightIndex], 1, ref vectorAsArray[0]);
        }
        public void SetUniformLightColor(Vector3 lightColor, int lightIndex)
        {
            float[] vectorAsArray = new float[3];
            lightColor.CopyTo(vectorAsArray);
            OpenGL.glUniform3fv(_lightColorUniformHandles[lightIndex], 1, ref vectorAsArray[0]);
        }
        public void SetUniformPointLight(bool isAPointLight, int lightIndex)
        {
            OpenGL.glUniform1ui(_pointLightUniformHandles[lightIndex], isAPointLight ? 1u : 0u);
        }
        public void SetUniformLightsCount(uint count)
        {
            OpenGL.glUniform1ui(_lightsCountUniformHandle, count);
        }

    }
}
