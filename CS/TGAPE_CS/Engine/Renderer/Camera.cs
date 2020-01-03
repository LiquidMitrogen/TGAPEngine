using System;
using System.Collections.Generic;
using System.Linq;
using System.Numerics;
using System.Text;
using System.Threading.Tasks;

namespace TGAPE_CS.Engine.Renderer
{
    //TODO:make an entity so it can be used in object hierarchy
    public class Camera
    {
        private float _nearPlaneDistance;
        private float _farPlaneDistance;
        private bool _isNeedsUpdate;

        private Matrix4x4 _cameraToClipSpaceTransformationMatrix;
        private Matrix4x4 _worldToClipSpaceTransformationMatrix;
        private Matrix4x4 _worldToCameraSpaceTransformationMatrix;
        private Vector3 _location;
        private Vector3 _targetToLookAt;
        private Vector3 _upDirection;

        public Vector3 Location
        {
            get => _location;
            set
            {
                _location = value;
                _isNeedsUpdate = true;
            }
        }
        public Vector3 TargetToLookAt
        {
            get => _targetToLookAt;
            set
            {
                _targetToLookAt = value;
                _isNeedsUpdate = true;
            }
        }
        public Vector3 UpDirection
        {
            get => _upDirection;
            set
            {
                _upDirection = value;
                _isNeedsUpdate = true;
            }
        }

        public Matrix4x4 CameraToClipSpaceTransformationMatrix
        {
            get
            {
                if (_isNeedsUpdate)
                    UpdateWorldToClipSpaceTransformationMatrix();
                return _cameraToClipSpaceTransformationMatrix;
            }
            private set => _cameraToClipSpaceTransformationMatrix = value;
        }
        public Matrix4x4 WorldToClipSpaceTransformationMatrix
        {
            get
            {
                if (_isNeedsUpdate)
                    UpdateWorldToClipSpaceTransformationMatrix();
                return _worldToClipSpaceTransformationMatrix;
            }
            private set => _worldToClipSpaceTransformationMatrix = value;
        }
        public Matrix4x4 WorldToCameraSpaceTransformationMatrix
        {
            get
            {
                if (_isNeedsUpdate)
                    UpdateWorldToClipSpaceTransformationMatrix();
                return _worldToCameraSpaceTransformationMatrix;
            }
            private set => _worldToCameraSpaceTransformationMatrix = value;
        }

        public static Camera ConstructAPerspectiveCamera(float fovAngle, float screenAspectRatio, float nearPlaneDistance, float farPlaneDistance, Vector3 location, Vector3 targetToLookAt, Vector3 upDirection)
        {
            var camera = new Camera(Matrix4x4.CreatePerspectiveFieldOfView(fovAngle, screenAspectRatio, nearPlaneDistance, farPlaneDistance), location, targetToLookAt, upDirection);
            camera.SetNearAndFarPlanes(nearPlaneDistance, farPlaneDistance);
            return camera;
        }
        public Camera(Matrix4x4 toProjectionMatrix, Vector3 location, Vector3 targetToLookAt, Vector3 upDirection)
        {
            _cameraToClipSpaceTransformationMatrix = toProjectionMatrix;
            _location = location;
            _targetToLookAt = targetToLookAt;
            _upDirection = upDirection;
            _isNeedsUpdate = true;
        }

        //TODO:it was public but not used, in this form it is useless
        private void SetNearAndFarPlanes(float nearPlaneDistance, float farPlaneDistance)
        {
            _nearPlaneDistance = nearPlaneDistance;
            _farPlaneDistance = farPlaneDistance;
        }

        //TODO:Useless - why not just set new location and the result is the same?
        public void ApplyTranslation(Vector3 translationVector)
        {
            _location = _location + translationVector;
            _isNeedsUpdate = true;
        }
        public void UpdateWorldToClipSpaceTransformationMatrix()
        {
            _worldToCameraSpaceTransformationMatrix = Matrix4x4.CreateLookAt(_location, _targetToLookAt, _upDirection);
            _worldToClipSpaceTransformationMatrix = _cameraToClipSpaceTransformationMatrix * _worldToCameraSpaceTransformationMatrix;
            _isNeedsUpdate = false;
        }
        public Vector3 GetMouseOnNearPlaneWorldSpace(float mouseXClipSpace, float mouseYClipSpace)
        {
            Vector4 mouseOnNearPlaneClipSpace = new Vector4(mouseXClipSpace * _nearPlaneDistance, mouseYClipSpace * _nearPlaneDistance, -1.0f * _nearPlaneDistance, _nearPlaneDistance);

            Matrix4x4 clipToWorldSpaceMatrix;
            //If we cant compute world to clip matrix inverse (Highly unlikely it would have to have determinant of 0) then we cant compute position of mouse cursor in world space
            if (!Matrix4x4.Invert(CameraToClipSpaceTransformationMatrix * WorldToCameraSpaceTransformationMatrix, out clipToWorldSpaceMatrix))
                return Vector3.Zero;

            var mouseOnNearPlaneWorld = Vector4.Transform(mouseOnNearPlaneClipSpace, clipToWorldSpaceMatrix);
            return new Vector3(mouseOnNearPlaneWorld.X, mouseOnNearPlaneWorld.Y, mouseOnNearPlaneWorld.Z);
        }
        public Vector3 GetMouseOnFarPlaneWorldSpace(float mouseXClipSpace, float mouseYClipSpace)
        {
            Vector4 mouseOnFarPlaneClipSpace = new Vector4(mouseXClipSpace * _farPlaneDistance, mouseYClipSpace * _farPlaneDistance, _farPlaneDistance, _farPlaneDistance);

            Matrix4x4 clipToWorldSpaceMatrix;
            //If we cant compute world to clip matrix inverse (Highly unlikely it would have to have determinant of 0) then we cant compute position of mouse cursor in world space
            if (!Matrix4x4.Invert(CameraToClipSpaceTransformationMatrix * WorldToCameraSpaceTransformationMatrix, out clipToWorldSpaceMatrix))
                return Vector3.Zero;

            var mouseOnFarPlaneWorld = Vector4.Transform(mouseOnFarPlaneClipSpace, clipToWorldSpaceMatrix);
            return new Vector3(mouseOnFarPlaneWorld.X, mouseOnFarPlaneWorld.Y, mouseOnFarPlaneWorld.Z);
        }

        public void SetWorldToClipMatrix(Matrix4x4 cameraToClipTransfomationMatrix, Matrix4x4 worldToCameraTransfomationMatrix)
        {
            _cameraToClipSpaceTransformationMatrix = cameraToClipTransfomationMatrix;
            _worldToCameraSpaceTransformationMatrix = worldToCameraTransfomationMatrix;
            _worldToClipSpaceTransformationMatrix = _cameraToClipSpaceTransformationMatrix * _worldToCameraSpaceTransformationMatrix;
            _isNeedsUpdate = false;
        }
    }
}
