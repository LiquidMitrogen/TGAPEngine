using System;
using System.Collections.Generic;
using System.Linq;
using System.Numerics;
using System.Text;
using System.Threading.Tasks;
using TGAPE_CS.Engine.Math;
using TGAPE_CS.Engine.Renderer;

namespace TGAPE_CS.Engine.Game
{
    public class Entity
    {
        //TODO: raplacing the bools with attributes if not more resource intensive
        //Entity should be a base class, and definitely should not have a VA Buffer
        private VertexAttributes _vertexAttributes;
        private List<Entity> _children;
        private Matrix4x4 _modelToWorldTransformationMatrix;
        private Vector3 _location;
        private Quaternion _rotation;
        private Vector3 _scale;

        private bool _isNeedsUpdate = false;



        public uint buildId = 0;
        public string name;
        public bool isMakingShadow = true;
        public bool isDisableDepthTests = true;
        public bool isDisableDrawing = true;

        public Matrix4x4 ModelToWorldTransformationMatrix
        {
            get
            {
                UpdateMatrix();
                return _modelToWorldTransformationMatrix;
            }
            set
            {
                _modelToWorldTransformationMatrix = value;
                _isNeedsUpdate = false;
            }
        }

        public Entity Parent { get; set; }

        public Entity(VertexAttributes vertexAttributes)
        {
            _location = Vector3.Zero;
            _rotation = Quaternion.Identity;
            _scale = Vector3.One;
            _isNeedsUpdate = true;
        }
        public Entity(Entity other)
        {
            _location = other._location;
            _rotation = other._rotation;
            _scale = other._scale;
            _isNeedsUpdate = true;
            _vertexAttributes = other._vertexAttributes;//TODO:this copies handles to buffer watch out
        }

        private void UpdateMatrix()
        {
            if (_isNeedsUpdate)
            {
                var transaltionMatrix = Matrix4x4.CreateTranslation(_location);
                var rotationMatrix = Matrix4x4.CreateFromQuaternion(_rotation);
                var scaleMatrix = Matrix4x4.CreateScale(_scale);
                _modelToWorldTransformationMatrix = rotationMatrix * scaleMatrix * transaltionMatrix;//Reversed order of multiplication :( also may be wort to check for nans and infs etc
                _isNeedsUpdate = false;
            }
        }

        //TODO: make public properties out of these :/
        public void ApplyRotation(Quaternion rotation)
        {
            _rotation = rotation * _rotation;
            _isNeedsUpdate = true;
        }
        public void ApplyRotation(Matrix4x4 rotation)
        {
            var rotationAsQuaternion = Quaternion.CreateFromRotationMatrix(rotation);
            _rotation = rotationAsQuaternion * _rotation;
            _isNeedsUpdate = true;
        }
        public void ApplyLocalRotationQ(Quaternion rotation)
        {
            _rotation = _rotation * rotation;
            _isNeedsUpdate = true;
            _rotation = Quaternion.Normalize(_rotation);
        }
        public void ApplyLocalRotation(Quaternion rotation)
        {
            var rotationMatrix = Matrix4x4.CreateFromQuaternion(rotation);
            _modelToWorldTransformationMatrix = _modelToWorldTransformationMatrix * rotationMatrix;
        }
        public void ApplyTranslation(Vector3 translation)
        {
            _location = _location + translation;
            _isNeedsUpdate = true;
        }
        public void ApplyLocalTranslation(Vector3 translation)
        {
            _modelToWorldTransformationMatrix = MatrixHelper.ApplyLocalTranslation(_modelToWorldTransformationMatrix, translation);
        }
        public void ApplyScale(Vector3 scale)
        {
            _scale.X = _scale.X * scale.X;
            _scale.Y = _scale.Y * scale.Y;
            _scale.Z = _scale.Z * scale.Z;
            _isNeedsUpdate = true;
        }
        public void ApplyUniformScale(float scale)
        {
            _scale = _scale * scale;
            _isNeedsUpdate = true;
        }
        public void ResetLocation()
        {
            _location = Vector3.Zero;
            _isNeedsUpdate = true;
        }
        public void ResetRotation()
        {
            _rotation = Quaternion.Identity;
            _isNeedsUpdate = true;
        }
        public void ResetScale()
        {
            _scale = Vector3.One;
            _isNeedsUpdate = true;
        }
        public Entity MakeDuplicate()
        {
            return new Entity(this._vertexAttributes);
        }
        //TODO:can I get rid of the opengl code from this?
        public void Draw(Matrix4x4 parentMatrix, DrawingContext drawingContext)
        {
            var combinedMatrix = ModelToWorldTransformationMatrix * parentMatrix;
            if (!isDisableDrawing)
            {
                /////////---------------------
            }
            foreach(var child in _children)
            {
                child.Draw(combinedMatrix, drawingContext);
            }
        }

    }
}
