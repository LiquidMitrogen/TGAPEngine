using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Numerics;
using System.Text;
using System.Threading.Tasks;
using TGAPE_CS.Engine.Game;
using TGAPE_CS.Engine.Renderer;
using TGAPE_CS.Engine.Renderer.MaterialPremade;
using TGAPE_CS.Engine.VectorMath;

namespace TGAPE_CS.Engine.Loader
{
    public enum EntityType
    {
        Mesh,
        Armature
    }
    [Flags]
    public enum ConfigurationFlags : byte
    {
        WithoutBones = 0x04
    }
    public struct Header
    {
        public EntityType type;
        public uint id;
        public ConfigurationFlags configurationFlags;
        public Quaternion rotation;
        public Vector3 location;
        public Vector3 scale;
        public string entityName;
        public string vertexShaderStr;
        public string fragmentShaderStr;
    }
    //TODO:actors TBD
    public class EntityWithHeader : Entity
    {
        public EntityWithHeader(VertexAttributes vertexAttributes, Header header) : base(vertexAttributes)
        {
            Header = header;
        }

        public EntityWithHeader(EntityWithHeader other) : base(other)
        {
            Header = other.Header;
        }

        public Header Header { get; set; }

    }
    public struct ObjectTreeElement
    {
        public byte id;
        public List<byte> childrenIds;
        public byte childrenCount;
    }
    public static class BamFileReader
    {
        private static VaoInitData ReadVaoInitData(BinaryReader binaryReader)
        {
            VaoInitData vaoInitData = new VaoInitData();
            vaoInitData.AttributeCount = binaryReader.ReadByte();
            vaoInitData.AttributeSize = binaryReader.ReadBytes(16);
            vaoInitData.AttributeType = new uint[16];
            for (int i = 0; i < 16; i++)
            {
                vaoInitData.AttributeType[i] = binaryReader.ReadUInt32();
            }
            vaoInitData.AttributeNormal = binaryReader.ReadBytes(16);
            vaoInitData.AttributeOffset = new uint[16];
            for (int i = 0; i < 16; i++)
            {
                vaoInitData.AttributeOffset[i] = binaryReader.ReadUInt32();
            }
            return vaoInitData;
        }
        private static VertexAttributes ReadVertexAttributes(BinaryReader binaryReader)
        {
            VaoInitData vaoInitData = ReadVaoInitData(binaryReader);


            uint bufferSize = binaryReader.ReadUInt32();
            byte[] vertexBuffer = binaryReader.ReadBytes((int)bufferSize);

            uint indicesCount = binaryReader.ReadUInt32();
            byte[] indiceBytes = binaryReader.ReadBytes((int)indicesCount);
            uint[] indices = new uint[indicesCount / sizeof(uint)];
            Buffer.BlockCopy(indiceBytes, 0, indices, 0, indiceBytes.Length);
            return new VertexAttributes(vertexBuffer, indices, vaoInitData);
        }
        public static Header LoadHeader(BinaryReader binaryReader)
        {
            var header = new Header();
            ushort tag = binaryReader.ReadUInt16();
            switch (tag)
            {
                case 0x4f42:
                    {
                        header.type = EntityType.Mesh;
                        break;
                    }
                case 0x4152:
                    {
                        header.type = EntityType.Armature;
                        break;
                    }
                default:
                    {
                        //TODO:exception
                        return new Header();
                    }
            }
            byte entityId = binaryReader.ReadByte();
            header.id = entityId;
            byte configurationFlags = binaryReader.ReadByte();
            header.configurationFlags = (ConfigurationFlags)configurationFlags;
            uint entityNameLength = binaryReader.ReadUInt32();
            byte[] entityName = binaryReader.ReadBytes((int)entityNameLength);
            string entityNameStr = Encoding.ASCII.GetString(entityName);
            header.entityName = entityNameStr;

            var lx = binaryReader.ReadSingle();
            var ly = binaryReader.ReadSingle();
            var lz = binaryReader.ReadSingle();
            header.location = new Vector3(lx, ly, lz);

            var qw = binaryReader.ReadSingle();
            var qx = binaryReader.ReadSingle();
            var qy = binaryReader.ReadSingle();
            var qz = binaryReader.ReadSingle();
            header.rotation = new Quaternion(qx, qy, qz, qw);

            var sx = binaryReader.ReadSingle();
            var sy = binaryReader.ReadSingle();
            var sz = binaryReader.ReadSingle();
            header.scale = new Vector3(sx, sy, sz);

            uint entityVertexLength = binaryReader.ReadUInt32();
            if (entityVertexLength > 0)
            {
                byte[] entityVertexBytes = binaryReader.ReadBytes((int)entityVertexLength);
                string entityVertexStr = Encoding.ASCII.GetString(entityVertexBytes);
                header.vertexShaderStr = entityVertexStr;
            }
            else
            {
                header.vertexShaderStr = "vertex.vert";
            }


            uint entityFragmentLength = binaryReader.ReadUInt32();
            if (entityFragmentLength > 0)
            {
                byte[] entityFragmentBytes = binaryReader.ReadBytes((int)entityFragmentLength);
                string entityFragmentStr = Encoding.ASCII.GetString(entityFragmentBytes);
                header.fragmentShaderStr = entityFragmentStr;
            }
            else
            {
                header.fragmentShaderStr = "fragment.frag";
            }
            return header;

        }
        public static Entity LoadNoBoneFile(BinaryReader binaryReader)
        {
            var vertexAttributes = ReadVertexAttributes(binaryReader);
            Entity entity = new Entity(vertexAttributes);
            return entity;

        }
        public static AnimatedActor LoadAnimatedActor(BinaryReader binaryReader)
        {
            var vertexAttributes = ReadVertexAttributes(binaryReader);
            uint armatureNameLength = binaryReader.ReadUInt32();
            byte[] armatureName = binaryReader.ReadBytes((int)armatureNameLength);
            string armatureNameStr = Encoding.ASCII.GetString(armatureName);
            return new AnimatedActor(vertexAttributes, armatureNameStr);
           
        }
        public static void AssignMaterial(Tuple<Entity, Header> entityAndHeader)
        {
            //TODO:choose material based on additional metadata
            if(entityAndHeader.Item2.type == EntityType.Mesh)
            {
                if ((entityAndHeader.Item2.configurationFlags & ConfigurationFlags.WithoutBones) > 0)
                {
                    entityAndHeader.Item1.entityMaterial = new SingleMatrixMaterial(entityAndHeader.Item2.vertexShaderStr, entityAndHeader.Item2.fragmentShaderStr);
                }
                else
                {
                    entityAndHeader.Item1.entityMaterial = new SingleMatrixMaterial(entityAndHeader.Item2.vertexShaderStr, entityAndHeader.Item2.fragmentShaderStr, );
                }
            }
        }
        public static Scene LoadScene(string filepath)
        {
            if (!File.Exists(filepath))
            {
                //TODO:exception
                Debugger.Break();
                return null;
            }
            using (var fileStream = File.OpenRead(filepath))
            {
                //TODO:check for endianess and reverse if necessary
                var binaryReader = new BinaryReader(fileStream);
                ushort tag = binaryReader.ReadUInt16();
                if (tag != 0x4241)
                {
                    //TODO:throw exception
                    Debugger.Break();
                    return null;
                }
                byte objectCount = binaryReader.ReadByte();
                var objectTree = new ObjectTreeElement[objectCount];
                for (int i = 0; i < objectCount; i++)
                {
                    byte objectId = binaryReader.ReadByte();
                    if (objectId >= objectCount)
                    {
                        //TODO:exception
                        Debugger.Break();
                        return null;
                    }
                    objectTree[objectId].id = objectId;
                    byte childrenCount = binaryReader.ReadByte();
                    objectTree[objectId].childrenCount = childrenCount;
                    if (childrenCount > 0)
                    {
                        objectTree[objectId].childrenIds = new List<byte>(childrenCount);
                        objectTree[objectId].childrenIds.AddRange(Enumerable.Repeat((byte)0, childrenCount));
                        for (int j = 0; j < childrenCount; j++)
                        {
                            byte childId = binaryReader.ReadByte();
                            objectTree[objectId].childrenIds[j] = childId;
                        }
                    }
                }
                var entityList = new List<Tuple<Entity,Header>>(objectCount);
                entityList.AddRange(Enumerable.Repeat((Tuple<Entity,Header>)null, objectCount));
                var armatures = new List<Armature>();
                var animatedActors = new List<AnimatedActor>();

                for (int i = 0; i < objectCount; i++)
                {
                    var header = LoadHeader(binaryReader);
                    if (header.type == EntityType.Armature)
                    {
                        var armature = LoadArmature(binaryReader);
                        armature.isDisableDrawing = true;
                        entityList[i] = new Tuple<Entity, Header>(armature, header);
                        armatures.Add(armature);
                    }
                    else
                    {
                        if ((header.configurationFlags & ConfigurationFlags.WithoutBones) > 0)
                        {
                            entityList[i] = new Tuple<Entity, Header>(LoadNoBoneFile(binaryReader), header);
                        }
                        else
                        {
                            var animatedActor = LoadAnimatedActor(binaryReader);
                            entityList[i] = new Tuple<Entity, Header>(animatedActor, header);
                            animatedActors.Add(animatedActor);
                        }
                    }
                    entityList[i].Item1.name = header.entityName;
                    entityList[i].Item1.buildId = header.id;
                    entityList[i].Item1.ApplyTranslation(header.location);
                    entityList[i].Item1.ApplyRotation(header.rotation);
                    entityList[i].Item1.ApplyScale(header.scale);

                }

                foreach(var actor in animatedActors)
                {
                    var armature = armatures.FirstOrDefault(x => x.name == actor.ArmatureName);
                    actor.Armature = armature;
                }

                foreach (var parent in objectTree)
                {
                    var parentEntity = entityList.FirstOrDefault(x => x.Item1.buildId == parent.id);
                    if (parentEntity == null)
                    {
                        //TODO:how come huh
                        Debugger.Break();
                        return null;
                    }
                    if (parent.childrenIds != null)
                    {
                        foreach (var childId in parent.childrenIds)
                        {
                            var childEntity = entityList.FirstOrDefault(x => x.Item1.buildId == childId);
                            if (childEntity == null)
                            {
                                //TODO:how come huh
                                Debugger.Break();
                                return null;
                            }
                            parentEntity.Item1.Children.Add(childEntity.Item1);
                            childEntity.Item1.Parent = parentEntity.Item1;
                        }
                    }

                }
                var entitiesToAdd = entityList.Where(x => x != null && x.Item1.Parent == null).Select(x => x.Item1);
                var newScene = new Scene(entitiesToAdd);
                return newScene;
            }
        }
        public static Bone LoadBones(BinaryReader binaryReader, uint boneCount)
        {
            if(boneCount == 0)
            {
                Debugger.Break();
                return null;
            }
            var boneStructArray = new BoneStruct[boneCount];
            for(uint i = 0; i < boneCount; i++)
            {
                uint boneNameLength = binaryReader.ReadUInt32();
                byte[] boneName = binaryReader.ReadBytes((int)boneNameLength);
                string boneNameStr = Encoding.ASCII.GetString(boneName);
                boneStructArray[i].name = boneNameStr;
                var childrenCount = binaryReader.ReadUInt32();
                boneStructArray[i].childrenIndices = new uint[childrenCount];
                for(uint childIndex = 0; childIndex < childrenCount; childIndex++)
                {
                    boneStructArray[i].childrenIndices[childIndex] = binaryReader.ReadUInt32();
                }
                var matrixTempArray = new float[16];
                for(uint matrixIndex = 0; matrixIndex < 16; matrixIndex++)
                {
                    matrixTempArray[matrixIndex] = binaryReader.ReadSingle();
                }
                boneStructArray[i].invertedMatrix = MatrixHelper.CreateFromAnArray(matrixTempArray);
                

            }
            return new Bone(boneStructArray, 0);
        }
        public static Keyframe LoadKeyframe(BinaryReader binaryReader)
        {
            var qw = binaryReader.ReadSingle();
            var qx = binaryReader.ReadSingle();
            var qy = binaryReader.ReadSingle();
            var qz = binaryReader.ReadSingle();
            var rotation = new Quaternion(qx, qy, qz, qw);

            var lx = binaryReader.ReadSingle();
            var ly = binaryReader.ReadSingle();
            var lz = binaryReader.ReadSingle();
            var location = new Vector3(lx, ly, lz);

            var frame = binaryReader.ReadUInt32();

            return new Keyframe(frame, location, rotation);
        }
        public static ActorAction LoadAction(BinaryReader binaryReader, uint boneCount)
        {
            var actionNameLength = binaryReader.ReadUInt32();
            var acionName = binaryReader.ReadBytes((int)actionNameLength);
            string actionNameStr = Encoding.ASCII.GetString(acionName);
            var rangeStart = binaryReader.ReadUInt32();
            var rangeEnd = binaryReader.ReadUInt32();
            var boneKeyframes = new List<ActionBoneKeyframe>();
            for(uint boneIterator = 0; boneIterator < boneCount; boneIterator++)
            {
                var boneIndex = binaryReader.ReadUInt32();
                var keyframeCount = binaryReader.ReadUInt32();
                var keyframes = new List<Keyframe>();
                for(uint keyframeIterator = 0; keyframeIterator < keyframeCount; keyframeIterator++)
                {
                    var keyframe = LoadKeyframe(binaryReader);
                    keyframes.Add(keyframe);
                }
                boneKeyframes.Add(new ActionBoneKeyframe(boneIndex, keyframes));
            }

            return new ActorAction(actionNameStr, rangeStart, rangeEnd, boneKeyframes);
        }
        public static Armature LoadArmature(BinaryReader binaryReader)
        {
            var boneCount = binaryReader.ReadUInt32();
            var rootBone = LoadBones(binaryReader, boneCount);
            var actionCount = binaryReader.ReadUInt32();
            var armature = new Armature(rootBone);
            for (uint actionIndex = 0; actionIndex < actionCount; actionIndex++)
            {
                var actorAction = LoadAction(binaryReader, boneCount);
                armature.AddAction(actorAction);
            }
            return armature;
        }
    }
}
