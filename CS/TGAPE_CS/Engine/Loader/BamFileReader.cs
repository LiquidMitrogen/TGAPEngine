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
            uint bufferSize = binaryReader.ReadUInt32();
            byte[] vertexBuffer = binaryReader.ReadBytes((int)bufferSize);

            uint indicesCount = binaryReader.ReadUInt32();
            byte[] indiceBytes = binaryReader.ReadBytes((int)indicesCount);
            uint[] indices = new uint[indicesCount / sizeof(uint)];
            Buffer.BlockCopy(indiceBytes, 0, indices, 0, indiceBytes.Length);
            var vertexAttributes = new VertexAttributes(vertexBuffer, indices, vaoInitData);
            Entity entity = new Entity(vertexAttributes);
            return entity;

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
                var entityList = new List<Entity>(objectCount);
                entityList.AddRange(Enumerable.Repeat((Entity)null, objectCount));
                //TODO: load actors too
                for (int i = 0; i < objectCount; i++)
                {
                    var header = LoadHeader(binaryReader);
                    if (header.type == EntityType.Armature)
                    {

                    }
                    else
                    {
                        if ((header.configurationFlags & ConfigurationFlags.WithoutBones) > 0)
                        {
                            entityList[i] = LoadNoBoneFile(binaryReader);
                        }
                        else
                        {

                        }
                    }
                    entityList[i].name = header.entityName;
                    entityList[i].buildId = header.id;
                    entityList[i].ApplyTranslation(header.location);
                    entityList[i].ApplyRotation(header.rotation);
                    entityList[i].ApplyScale(header.scale);
                    if (header.type != EntityType.Armature)
                    {
                        //TODO:choose material based on additional metadata
                        entityList[i].entityMaterial = new SingleMatrixMaterial(header.vertexShaderStr, header.fragmentShaderStr);
                    }
                }
                //TODO:AnimatedActors loop here
                foreach (var parent in objectTree)
                {
                    var parentEntity = entityList.FirstOrDefault(x => x.buildId == parent.id);
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
                            var childEntity = entityList.FirstOrDefault(x => x.buildId == childId);
                            if (childEntity == null)
                            {
                                //TODO:how come huh
                                Debugger.Break();
                                return null;
                            }
                            parentEntity.Children.Add(childEntity);
                            childEntity.Parent = parentEntity;
                        }
                    }

                }
                var entitiesToAdd = entityList.Where(x => x != null && x.Parent == null);
                var newScene = new Scene(entitiesToAdd);
                return newScene;
            }
        }
    }
}
