using CSGL;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Numerics;
using System.Text;
using System.Threading.Tasks;

namespace TGAPE_CS.Engine.Renderer
{
    public enum FilterMode : uint
    {
        Nearest = OpenGL.GL_NEAREST,
        Linear = OpenGL.GL_LINEAR,
        MipmapLinear = OpenGL.GL_LINEAR_MIPMAP_LINEAR,
        NearMipmapLinear = OpenGL.GL_NEAREST_MIPMAP_LINEAR
    }
    public class Image
    {
        private FilterMode _filterMode;
        private uint _format;
        private uint _formatSize;
        private byte[] _imageData;
        private uint _imageHeight;
        private uint _imageWidth;
        private uint _textureHandle;
        private uint _samplerHandle;
        private uint _pixelBufferHandle;
        private bool useFlag = true;

        public uint texUnit;

        public bool UseFlag
        {
            private get => useFlag;
            set => useFlag = value;
        }
        public byte[] ImageData
        {
            get => _imageData;
            set => SetImageData(value);
        }


        public Image(byte[] data, uint width, uint height, uint unit, uint format, FilterMode filterMode, bool repeat)
        {
            texUnit = unit;
            _format = format;
            _filterMode = filterMode;

            OpenGL.glEnable(OpenGL.GL_TEXTURE_2D);
            _imageData = data;
            _imageHeight = height;
            _imageWidth = width;
            OpenGL.glGenSamplers(1, ref _samplerHandle);
            var magFilter = filterMode == FilterMode.Nearest ? OpenGL.GL_NEAREST : OpenGL.GL_LINEAR;
            var minFilter = (uint)filterMode;
            OpenGL.glSamplerParameteri(_samplerHandle, OpenGL.GL_TEXTURE_MAG_FILTER, (int)magFilter);
            OpenGL.glSamplerParameteri(_samplerHandle, OpenGL.GL_TEXTURE_MIN_FILTER, (int)minFilter);
            OpenGL.glSamplerParameteri(_samplerHandle, OpenGL.GL_TEXTURE_WRAP_S, repeat ? (int)OpenGL.GL_REPEAT : (int)OpenGL.GL_CLAMP_TO_EDGE);
            OpenGL.glSamplerParameteri(_samplerHandle, OpenGL.GL_TEXTURE_WRAP_T, repeat ? (int)OpenGL.GL_REPEAT : (int)OpenGL.GL_CLAMP_TO_EDGE);

            OpenGL.glGenBuffers(1, ref _pixelBufferHandle);
            OpenGL.glBindBuffer(OpenGL.GL_PIXEL_UNPACK_BUFFER, _pixelBufferHandle);
            switch (format)
            {
                case OpenGL.GL_LUMINANCE:
                    {
                        _formatSize = 1;
                        break;
                    }
                case OpenGL.GL_RED:
                    {
                        _formatSize = 1;
                        break;
                    }
                case OpenGL.GL_RGB:
                    {
                        _formatSize = 3;
                        break;
                    }
                case OpenGL.GL_RGBA:
                    {
                        _formatSize = 4;
                        break;
                    }
                default:
                    {
                        _formatSize = 1;
                        break;
                    }
            }
            unsafe
            {
                fixed (void* ptrData = data)
                    OpenGL.glBufferData(OpenGL.GL_PIXEL_UNPACK_BUFFER, (int)width * (int)height * sizeof(byte) * (int)_formatSize, (IntPtr)ptrData, OpenGL.GL_DYNAMIC_DRAW);
            }
            OpenGL.glGenTextures(1, ref _textureHandle);
            OpenGL.glBindTexture(OpenGL.GL_TEXTURE_2D, _textureHandle);
            OpenGL.glTexParameteri(OpenGL.GL_TEXTURE_2D, OpenGL.GL_TEXTURE_BASE_LEVEL, 0);
            if (filterMode == FilterMode.MipmapLinear)
            {
                OpenGL.glTexParameteri(OpenGL.GL_TEXTURE_2D, OpenGL.GL_TEXTURE_MAX_LEVEL, 1000);
                OpenGL.glGenerateMipmap(OpenGL.GL_TEXTURE_2D);
            }
            else
            {
                OpenGL.glTexParameteri(OpenGL.GL_TEXTURE_2D, OpenGL.GL_TEXTURE_MAX_LEVEL, 0);
            }
            if (data != null)
            {
                OpenGL.glTexImage2D(OpenGL.GL_TEXTURE_2D, 0, (int)OpenGL.GL_RGBA8, (int)width, (int)height, 0, format, OpenGL.GL_UNSIGNED_BYTE, IntPtr.Zero);
            }
            if (filterMode == FilterMode.MipmapLinear && data != null)
            {
                OpenGL.glGenerateMipmap(OpenGL.GL_TEXTURE_2D);
            }
            OpenGL.glBindTexture(OpenGL.GL_TEXTURE_2D, 0);
            OpenGL.glBindBuffer(OpenGL.GL_PIXEL_UNPACK_BUFFER, 0);
        }
        public void Use()
        {
            if (UseFlag)
            {
                OpenGL.glActiveTexture(OpenGL.GL_TEXTURE0 + texUnit);
                OpenGL.glBindTexture(OpenGL.GL_TEXTURE_2D, _textureHandle);
                OpenGL.glBindSampler(texUnit, _samplerHandle);
            }
        }
        public void Reject()
        {
            OpenGL.glBindSampler(texUnit, 0);
            OpenGL.glBindTexture(OpenGL.GL_TEXTURE_2D, 0);
            OpenGL.glActiveTexture(OpenGL.GL_TEXTURE0);
        }
        public void SetImageData(byte[] imageData)
        {
            _imageData = imageData;
            OpenGL.glBindBuffer(OpenGL.GL_PIXEL_UNPACK_BUFFER, _pixelBufferHandle);
            OpenGL.glBindTexture(OpenGL.GL_TEXTURE_2D, _textureHandle);
            if (_imageData != null)
            {
                unsafe
                {
                    fixed (void* ptrData = imageData)
                        OpenGL.glBufferSubData(OpenGL.GL_PIXEL_UNPACK_BUFFER, 0,(int)_imageHeight * (int)_imageWidth * sizeof(byte) * (int)_formatSize, (IntPtr)ptrData);
                }
                OpenGL.glTexSubImage2D(OpenGL.GL_TEXTURE_2D, 0, 0, 0, (int)_imageWidth, (int)_imageHeight, _format, OpenGL.GL_UNSIGNED_BYTE, IntPtr.Zero);
                if(_filterMode == FilterMode.MipmapLinear)
                {
                    OpenGL.glGenerateMipmap(OpenGL.GL_TEXTURE_2D);
                }
            }
            OpenGL.glBindTexture(OpenGL.GL_TEXTURE_2D, 0);
            OpenGL.glBindBuffer(OpenGL.GL_PIXEL_UNPACK_BUFFER, 0);
        }
        //TODO:magic numbers I not sure now whar coors are they
        public void SetSubImageData(byte[] imageData, uint bx, uint by, uint ex, uint ey)
        {
            uint tempBuffer = 0;
            _imageData = imageData;
            OpenGL.glBindBuffer(OpenGL.GL_PIXEL_UNPACK_BUFFER, _pixelBufferHandle);
            OpenGL.glBindTexture(OpenGL.GL_TEXTURE_2D, _textureHandle);
            if (_imageData != null)
            {
                OpenGL.glGenBuffers(1, ref tempBuffer);
                OpenGL.glBindBuffer(OpenGL.GL_PIXEL_UNPACK_BUFFER, tempBuffer);

                unsafe
                {
                    fixed (void* ptrData = imageData)
                        OpenGL.glBufferData(OpenGL.GL_PIXEL_UNPACK_BUFFER, (int)((ex - bx) * (ey - by) * sizeof(byte) * _formatSize), (IntPtr)ptrData, OpenGL.GL_DYNAMIC_DRAW);
                }
                OpenGL.glTexSubImage2D(OpenGL.GL_TEXTURE_2D, 0, (int)bx, (int)by, (int)(ex - bx), (int)(ey - by), _format, OpenGL.GL_UNSIGNED_BYTE, IntPtr.Zero);
                if (_filterMode == FilterMode.MipmapLinear)
                {
                    OpenGL.glGenerateMipmap(OpenGL.GL_TEXTURE_2D);
                }
            }
            OpenGL.glBindTexture(OpenGL.GL_TEXTURE_2D, 0);
            OpenGL.glBindBuffer(OpenGL.GL_PIXEL_UNPACK_BUFFER, 0);
        }

    }
}
