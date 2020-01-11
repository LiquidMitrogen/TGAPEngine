using CSGL;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Numerics;
using System.Text;
using System.Threading.Tasks;
using TGAPE_CS.Engine.VectorMath;

namespace TGAPE_CS.Engine.Game
{

    public struct BoneStruct
    {
        public uint[] childrenIndices;
        public Matrix4x4 invertedMatrix;
        public string name;
        public BoneStruct(uint[] childrenIndices, Matrix4x4 invertedMatrix, string name)
        {
            this.childrenIndices = childrenIndices;
            this.name = name;
            this.invertedMatrix = invertedMatrix;
        }
    }

    public struct QuatLoc
    {
        public Quaternion quaternion;
        public Vector3 location;
    }

    public class Bone
    {
        protected string name;
        protected uint index;
        protected Matrix4x4 invertedBoneMatrix;
        protected Quaternion boneRotation;
        protected Vector3 boneLocation;
        protected List<Bone> childrenBones;

        protected bool boneNeedsUniformUpdate = true;

        public Quaternion BoneRotation { get => boneRotation; set => boneRotation = value; }
        public Vector3 BoneLocation { get => boneLocation; set => boneLocation = value; }
        public string Name { get => name; private set => name = value; }
        public uint Index { get => index; private set => index = value; }

        public Bone(BoneStruct[] bones, uint boneIndex)
        {
            index = boneIndex;
            boneLocation = Vector3.Zero;
            boneRotation = Quaternion.Identity;
            boneNeedsUniformUpdate = true;
            invertedBoneMatrix = bones[boneIndex].invertedMatrix;
            name = bones[boneIndex].name;
            childrenBones = new List<Bone>();
            
            foreach(var childBoneIndex in bones[boneIndex].childrenIndices)
            {
                childrenBones.Add(new Bone(bones, childBoneIndex));
            }
        }
        public void ApplyAction(ActorAction actorAction, uint frame)
        {
            boneNeedsUniformUpdate = true;
            var keyframe = actorAction.GetQuatLoc(index, frame);
            boneLocation = keyframe.location;
            boneRotation = keyframe.rotation;

            foreach(var child in childrenBones)
            {
                child.ApplyAction(actorAction, frame);
            }
        }
        public void ModifyBoneUniform(int[] boneUniforms)
        {
            if (boneNeedsUniformUpdate)
            {
                boneNeedsUniformUpdate = false;
                var originToLocationTranlsation = Matrix4x4.CreateTranslation(boneLocation);
                var rotationAroundCurrentLocation = Matrix4x4.CreateFromQuaternion(boneRotation);
                var finalTransformation = invertedBoneMatrix * rotationAroundCurrentLocation * originToLocationTranlsation;
                var transformationAsArray = MatrixHelper.ConvertToAnArray(finalTransformation);
                OpenGL.glUniformMatrix4fv(boneUniforms[index], 1, OpenGL.GL_FALSE, ref transformationAsArray[0]);
                
                foreach(var child in childrenBones)
                {
                    child.ModifyBoneUniform(boneUniforms);
                }
            }
        }
        public Bone FindBoneByName(string name)
        {
            if (this.name.Equals(name))
            {
                return this;
            }
            else
            {
                foreach(var child in childrenBones)
                {
                    var bone = child.FindBoneByName(name);
                    if (bone != null)
                        return bone;
                }
            }
            return null;
        }
        //wtf does this does? 
        //this is some genious (or stupid) shit
        //pivot is a pivot
        //rotation is around that pivot
        //so old bone is translated to pivot and rotated and translated back
        //all of this is because rotations are additive in bone hierarchy
        //but locations are all related to root so you have to update them when rotating parent
        public void RotateLocationAroundAPointForFrame(Vector3 pivot, Quaternion rotation)
        {
            boneNeedsUniformUpdate = true;
            var translated = new Vector3(boneLocation.X - pivot.X, 
                boneLocation.Y - pivot.Y, 
                boneLocation.Z - pivot.Z);
            var rotated = Vector3.Transform(translated, rotation);
            var newLocation = new Vector3(rotated.X + pivot.X,
                rotated.Y + pivot.Y,
                rotated.Z + pivot.Z);
            boneLocation = newLocation;
        }
        public void ApplyArmatureRotation(Quaternion rotation)
        {
            boneNeedsUniformUpdate = true;
            boneRotation = rotation * boneRotation;
            ApplyChildrenTransformation(boneLocation, rotation);


        }
        private void ApplyChildrenTransformation(Vector3 pivot, Quaternion rotation)
        {
            foreach(var child in childrenBones)
            {
                child.boneNeedsUniformUpdate = true;
                child.BoneRotation = rotation * child.BoneRotation;
                child.RotateLocationAroundAPointForFrame(pivot, rotation);
                child.ApplyChildrenTransformation(pivot, rotation);
            }
        } 

        
    }
}
