using CSGL;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TGAPE_CS.Engine.Renderer
{
    public struct VaoInitData
    {
        public byte AttrbuteCount;
        public byte[] AttributeSize;
        public uint[] AttributeType;
        public byte[] AttributeNormal;
        public uint[] AttributeOffset;
    }
    public class VertexAttributes
    {
        private uint _vboBufferHandle;
        private uint _iVboBufferHandle;
        private uint _vaoBufferHandle;

        public uint IndiceCount;

        private void _createBuffer(byte[] data, uint additionalBufferSize)
        {
            OpenGL.glGenBuffers(1, ref _vboBufferHandle);
            OpenGL.glBindBuffer(OpenGL.GL_ARRAY_BUFFER, _vboBufferHandle);
            OpenGL.glBufferData(OpenGL.GL_ARRAY_BUFFER, (int)(data.Length + additionalBufferSize) * sizeof(byte), IntPtr.Zero, OpenGL.GL_STATIC_DRAW);
            unsafe
            {
                fixed (void* ptrData = data)
                    OpenGL.glBufferSubData(OpenGL.GL_ARRAY_BUFFER, 0, data.Length * sizeof(byte), (IntPtr)ptrData);
            }
            OpenGL.glBindBuffer(OpenGL.GL_ARRAY_BUFFER, 0);

            IndiceCount = 0;
        }
        private void _createBuffer(byte[] data, ushort[] indiceData)
        {
            OpenGL.glGenBuffers(1, ref _vboBufferHandle);
            OpenGL.glBindBuffer(OpenGL.GL_ARRAY_BUFFER, _vboBufferHandle);
            unsafe
            {
                fixed (void* ptrData = data)
                    OpenGL.glBufferData(OpenGL.GL_ARRAY_BUFFER, data.Length * sizeof(byte), (IntPtr)ptrData, OpenGL.GL_STATIC_DRAW);
            }
            OpenGL.glBindBuffer(OpenGL.GL_ARRAY_BUFFER, 0);

            IndiceCount = (uint)indiceData.Length;
            OpenGL.glGenBuffers(1, ref _iVboBufferHandle);
            OpenGL.glBindBuffer(OpenGL.GL_ELEMENT_ARRAY_BUFFER, _iVboBufferHandle);
            unsafe
            {
                fixed (void* ptrIData = indiceData)
                    OpenGL.glBufferData(OpenGL.GL_ELEMENT_ARRAY_BUFFER, indiceData.Length * sizeof(ushort), (IntPtr)ptrIData, OpenGL.GL_STATIC_DRAW);
            }
            OpenGL.glBindBuffer(OpenGL.GL_ELEMENT_ARRAY_BUFFER, 0);
        }
        private void _createBuffer(byte[] data, uint[] indiceData)
        {
            OpenGL.glGenBuffers(1, ref _vboBufferHandle);
            OpenGL.glBindBuffer(OpenGL.GL_ARRAY_BUFFER, _vboBufferHandle);
            unsafe
            {
                fixed (void* ptrData = data)
                    OpenGL.glBufferData(OpenGL.GL_ARRAY_BUFFER, data.Length * sizeof(byte), (IntPtr)ptrData, OpenGL.GL_STATIC_DRAW);
            }
            OpenGL.glBindBuffer(OpenGL.GL_ARRAY_BUFFER, 0);

            IndiceCount = (uint)indiceData.Length;
            OpenGL.glGenBuffers(1, ref _iVboBufferHandle);
            OpenGL.glBindBuffer(OpenGL.GL_ELEMENT_ARRAY_BUFFER, _iVboBufferHandle);
            unsafe
            {
                fixed (void* ptrIData = indiceData)
                    OpenGL.glBufferData(OpenGL.GL_ELEMENT_ARRAY_BUFFER, indiceData.Length * sizeof(uint), (IntPtr)ptrIData, OpenGL.GL_STATIC_DRAW);
            }
            OpenGL.glBindBuffer(OpenGL.GL_ELEMENT_ARRAY_BUFFER, 0);
        }
        private void _createVertexArray(VaoInitData vaoInitData)
        {
            OpenGL.glGenVertexArrays(1, ref _vaoBufferHandle);
            OpenGL.glBindVertexArray(_vaoBufferHandle);
            OpenGL.glBindBuffer(OpenGL.GL_ARRAY_BUFFER, _vboBufferHandle);
            OpenGL.glBindBuffer(OpenGL.GL_ELEMENT_ARRAY_BUFFER, _iVboBufferHandle);
            if(vaoInitData.AttrbuteCount > 16)
            {
                Console.WriteLine("Warning : attribute count is greater than 16");
                vaoInitData.AttrbuteCount = 16;
            }
            for(uint i = 0; i < vaoInitData.AttrbuteCount; i++)
            {
                OpenGL.glEnableVertexAttribArray(i);
                if(vaoInitData.AttributeType[i] == OpenGL.GL_FLOAT)
                {
                    OpenGL.glVertexAttribPointer(i, vaoInitData.AttributeSize[i], vaoInitData.AttributeType[i], vaoInitData.AttributeNormal[i], 0, (IntPtr)0 + (int)vaoInitData.AttributeOffset[i]);
                }
                else
                {
                    OpenGL.glVertexAttribIPointer(i, vaoInitData.AttributeSize[i], vaoInitData.AttributeType[i], 0, (IntPtr)0 + (int)vaoInitData.AttributeOffset[i]);
                }
            }
            OpenGL.glBindVertexArray(0);
        }

        public void Use()
        {
            OpenGL.glBindVertexArray(_vaoBufferHandle);
            OpenGL.glBindBuffer(OpenGL.GL_ARRAY_BUFFER, _vboBufferHandle);
            OpenGL.glBindBuffer(OpenGL.GL_ELEMENT_ARRAY_BUFFER, _iVboBufferHandle);
        }
        public void UpdateBuffer(byte[] data, uint offset, uint size)
        {
            OpenGL.glBindBuffer(OpenGL.GL_ARRAY_BUFFER, _vboBufferHandle);
            unsafe
            {
                fixed (void* ptrData = data)
                    OpenGL.glBufferSubData(OpenGL.GL_ARRAY_BUFFER, (int) offset, (int)size, (IntPtr)ptrData);
            }
            OpenGL.glBindBuffer(OpenGL.GL_ARRAY_BUFFER, 0);

        }
    }
}
