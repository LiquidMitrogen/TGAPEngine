#ifndef GLTFFORMAT_H
#define GLTFFORMAT_H
#include <nlohmann/json.hpp>
#include <string>
#include <optional>
#include <vector>

using json = nlohmann::json;

namespace GlTFFormat
{
#define OPTIONAL_KEY(Name) \
		if (j.count(#Name) != 0) \
		j.at(#Name).get_to(t.Name);
#define OPTIONAL_KEY_GET(Name) \
		if (j.count(#Name) != 0) \
		t.Name = j.at(#Name).get<decltype (t.Name)::value_type>();
	class GlTFAsset
	{
		public:
		std::string generator;
		std::string version;
	};
	class GlTFScenes
	{
		public:
		std::string name;
		std::vector<int> nodes;
	};
	class GlTFNode
	{
		public:
		std::optional<std::string> name;
		std::optional<std::vector<float>> rotation;
		std::optional<std::vector<float>> scale;
		std::optional<std::vector<float>> translation;
		std::optional<std::vector<int>> children;
		std::optional<int> camera;
		std::optional<int> skin;
		std::optional<std::vector<float>> matrix;
		std::optional<int> mesh;
		std::optional<std::vector<float>> weights;
		//std::optional<std::map<std::string

	};
	class GlTFChannelTarget
	{
		public:
		std::optional<int> node;
		std::string path;
	};
	class GlTFChannel
	{
		public:
		int sampler;
		GlTFChannelTarget target;
	};
	class GlTFSampler
	{
		public:
		int input;
		std::optional<std::string > interpolation;
		int output;
	};
	class GlTFAnimation
	{
		public:
		std::optional<std::string> name;
		std::vector<GlTFChannel> channels;
		std::vector<GlTFSampler> samplers;
	};
	class GlFTTextureInfo
	{
		public:
		int index;
		std::optional<int> texCoord;

	};
	class GlFTMaterialNormalTextureInfo
	{
		public:
		int index;
		std::optional<int> texCoord;
		std::optional<float> scale;
	};
	class GlFTMaterialOcclusionTextureInfo
	{
		public:
		int index;
		std::optional<int> texCoord;
		std::optional<float> strength;
	};
	class GlFTPbrMetallicRoughness
	{
		public:
		std::optional<std::vector<float>> baseColorFactor;
		std::optional< GlFTTextureInfo> baseColorTexture;
		std::optional<float> metallicFactor;
		std::optional<float> roughnessFactor;
		std::optional< GlFTTextureInfo> metallicRoughnessTexture;

	};
	class GlTFMaterial
	{
		public:
		std::optional<std::string> name;
		std::optional<GlFTPbrMetallicRoughness> pbrMetallicRoughness;
		std::optional< GlFTMaterialNormalTextureInfo> normalTexture;
		std::optional< GlFTMaterialOcclusionTextureInfo> occlusionTexture;
		std::optional< GlFTTextureInfo> emissiveTexture;
		std::optional<std::vector<float>> emissiveFactor;
		std::optional<std::string> alphaMode;
		std::optional<float> alphaCutoff;
		std::optional<bool> doubleSided;
	};
	class GlTFPrimitive
	{
		public:
		std::map<std::string, int> attributes;
		std::optional<int> indices;
		std::optional<int> material;
		std::optional<int> mode;//topology of primitives (number of vertices per primitive)
		//targets object[1 - *] An array of morph targets.
	};
	class GlTFMesh
	{
		public:
		std::vector<GlTFPrimitive> primitives;
		std::optional<std::vector<float>> weights;
		std::optional<std::string> name;
	};
	class GlTFSkin
	{
		public:
		std::optional<int> inverseBindMatrices;
		std::optional<int> skeleton;
		std::vector<int> joints;
		std::optional<std::string> name;
	};
	class GlFTAccessorSparseValues
	{
		public:
		int bufferView;
		std::optional<int> byteOffset;
	};
	class GlFTAccessorSparseIndices
	{
		public:
		int bufferView;
		std::optional<int> byteOffset;
		int componentType;
	};
	class GlFTAccessorSparse
	{
		public:
		int count;
		GlFTAccessorSparseIndices indices;
		GlFTAccessorSparseValues values;
	};
	class GlTFAccessor
	{
		public:
		std::optional<int> bufferView;
		std::optional<int> byteOffset;
		int componentType;
		std::optional<bool> normalized;
		int count;
		std::string type;
		std::optional<std::vector<float>> max;
		std::optional<std::vector<float>> min;
		std::optional<GlFTAccessorSparse> sparse;
		std::optional<std::string> name;
	};
	class GlTFBufferView
	{
		public:
		int buffer;
		std::optional<int> byteOffset;
		int byteLength;
		std::optional<int> byteStride;
		std::optional<int> target;
		std::optional<std::string> name;

	};
	class GlTFBuffer
	{
		public:
		std::optional<std::string> uri;
		int byteLength;
		std::optional<std::string> name;

	};
	class GlTFJsonRepresentation
	{
		public:
		GlTFAsset asset;
		std::optional<int> scene;
		std::optional<std::vector<GlTFScenes>> scenes;
		std::optional<std::vector <GlTFNode>> nodes;
		std::optional<std::vector <GlTFAnimation>> animations;
		std::optional<std::vector <GlTFMaterial>> materials;
		std::optional<std::vector <GlTFMesh>> meshes;
		std::optional<std::vector <GlTFSkin>> skins;
		std::optional<std::vector<GlTFAccessor>> accessors;
		std::optional<std::vector<GlTFBufferView>> bufferViews;
		std::optional<std::vector<GlTFBuffer>> buffers;
	};
	NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(GlTFAsset, generator, version)
	NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(GlTFScenes, name, nodes)
	inline void to_json(json& j, const GlTFNode& t)
	{

	}
	inline void from_json(const json& j, GlTFNode& t)
	{
		OPTIONAL_KEY_GET(name)
		OPTIONAL_KEY_GET(rotation)
		OPTIONAL_KEY_GET(scale)
		OPTIONAL_KEY_GET(translation)
		OPTIONAL_KEY_GET(children)
		OPTIONAL_KEY_GET(skin)
		OPTIONAL_KEY_GET(matrix)
		OPTIONAL_KEY_GET(mesh)
		OPTIONAL_KEY_GET(weights)
	}
	inline void to_json(json& j, const GlTFChannelTarget& t)
	{

	}
	inline void from_json(const json& j, GlTFChannelTarget& t)
	{
		OPTIONAL_KEY_GET(node)
		j.at("path").get_to(t.path);
	}
	inline void to_json(json& j, const GlTFChannel& t)
	{

	}
	inline void from_json(const json& j, GlTFChannel& t)
	{
		j.at("sampler").get_to(t.sampler);
		j.at("target").get_to(t.target);
	}
	inline void to_json(json& j, const GlTFSampler& t)
	{

	}
	inline void from_json(const json& j, GlTFSampler& t)
	{
		j.at("input").get_to(t.input);
		OPTIONAL_KEY_GET(interpolation)
		j.at("output").get_to(t.output);
	}
	inline void to_json(json& j, const GlTFAnimation& t)
	{

	}
	inline void from_json(const json& j, GlTFAnimation& t)
	{
		OPTIONAL_KEY_GET(name)
		j.at("channels").get_to(t.channels);
		j.at("samplers").get_to(t.samplers);
	}
	inline void to_json(json& j, const GlFTTextureInfo& t)
	{

	}
	inline void from_json(const json& j, GlFTTextureInfo& t)
	{
		j.at("index").get_to(t.index);
		OPTIONAL_KEY_GET(texCoord)
	}
	inline void to_json(json& j, const GlFTMaterialNormalTextureInfo& t)
	{

	}
	inline void from_json(const json& j, GlFTMaterialNormalTextureInfo& t)
	{
		j.at("index").get_to(t.index);
		OPTIONAL_KEY_GET(texCoord)
		OPTIONAL_KEY_GET(scale)
	}
	inline void to_json(json& j, const GlFTMaterialOcclusionTextureInfo& t)
	{

	}
	inline void from_json(const json& j, GlFTMaterialOcclusionTextureInfo& t)
	{
		j.at("index").get_to(t.index);
		OPTIONAL_KEY_GET(texCoord)
		OPTIONAL_KEY_GET(strength)
	}
	inline void to_json(json& j, const GlFTPbrMetallicRoughness& t)
	{

	}
	inline void from_json(const json& j, GlFTPbrMetallicRoughness& t)
	{
		OPTIONAL_KEY_GET(baseColorFactor)
		OPTIONAL_KEY_GET(baseColorTexture)
		OPTIONAL_KEY_GET(metallicFactor)
		OPTIONAL_KEY_GET(roughnessFactor)
		OPTIONAL_KEY_GET(metallicRoughnessTexture)

	}
	inline void to_json(json& j, const GlTFPrimitive& t)
	{

	}
	inline void from_json(const json& j, GlTFPrimitive& t)
	{
		j.at("attributes").get_to(t.attributes);
		OPTIONAL_KEY_GET(indices)
		OPTIONAL_KEY_GET(material)
		OPTIONAL_KEY_GET(mode)

	}
	inline void to_json(json& j, const GlTFMesh& t)
	{

	}
	inline void from_json(const json& j, GlTFMesh& t)
	{
		j.at("primitives").get_to(t.primitives);
		OPTIONAL_KEY_GET(weights)
		OPTIONAL_KEY_GET(name)

	}

	inline void to_json(json& j, const GlTFMaterial& t)
	{

	}
	inline void from_json(const json& j, GlTFMaterial& t)
	{
		OPTIONAL_KEY_GET(name)
		OPTIONAL_KEY_GET(pbrMetallicRoughness)
		OPTIONAL_KEY_GET(normalTexture)
		OPTIONAL_KEY_GET(occlusionTexture)
		OPTIONAL_KEY_GET(emissiveTexture)
		OPTIONAL_KEY_GET(emissiveFactor)
		OPTIONAL_KEY_GET(alphaMode)
		OPTIONAL_KEY_GET(alphaCutoff)
		OPTIONAL_KEY_GET(doubleSided)
	}

	inline void to_json(json& j, const GlTFSkin& t)
	{

	}
	inline void from_json(const json& j, GlTFSkin& t)
	{
		OPTIONAL_KEY_GET(inverseBindMatrices)
		OPTIONAL_KEY_GET(skeleton)
		j.at("joints").get_to(t.joints);
		OPTIONAL_KEY_GET(name)

	}
	inline void to_json(json& j, const GlFTAccessorSparseValues& t)
	{

	}
	inline void from_json(const json& j, GlFTAccessorSparseValues& t)
	{
		j.at("bufferView").get_to(t.bufferView);
		OPTIONAL_KEY_GET(byteOffset)
	}
	inline void to_json(json& j, const GlFTAccessorSparseIndices& t)
	{

	}
	inline void from_json(const json& j, GlFTAccessorSparseIndices& t)
	{
		j.at("bufferView").get_to(t.bufferView);
		OPTIONAL_KEY_GET(byteOffset)
		j.at("componentType").get_to(t.componentType);
	}
	inline void to_json(json& j, const GlFTAccessorSparse& t)
	{

	}
	inline void from_json(const json& j, GlFTAccessorSparse& t)
	{
		j.at("count").get_to(t.count);
		j.at("indices").get_to(t.indices);
		j.at("values").get_to(t.values);

	}
	inline void to_json(json& j, const GlTFAccessor& t)
	{

	}
	inline void from_json(const json& j, GlTFAccessor& t)
	{
		OPTIONAL_KEY_GET(bufferView)
		OPTIONAL_KEY_GET(byteOffset)
		j.at("componentType").get_to(t.componentType);
		OPTIONAL_KEY_GET(normalized)
		j.at("count").get_to(t.count);
		j.at("type").get_to(t.type);
		OPTIONAL_KEY_GET(max)
		OPTIONAL_KEY_GET(min)
		OPTIONAL_KEY_GET(sparse)
		OPTIONAL_KEY_GET(name)

	}
	inline void to_json(json& j, const GlTFBufferView& t)
	{

	}
	inline void from_json(const json& j, GlTFBufferView& t)
	{
		j.at("buffer").get_to(t.buffer);
		OPTIONAL_KEY_GET(byteOffset)
		j.at("byteLength").get_to(t.byteLength);
		OPTIONAL_KEY_GET(byteStride)
		OPTIONAL_KEY_GET(target)
		OPTIONAL_KEY_GET(name)
	}
	inline void to_json(json& j, const GlTFBuffer& t)
	{

	}
	inline void from_json(const json& j, GlTFBuffer& t)
	{
		OPTIONAL_KEY_GET(uri)
		j.at("byteLength").get_to(t.byteLength);
		OPTIONAL_KEY_GET(name)
	}
	inline void to_json(json& j, const GlTFJsonRepresentation& t)
	{

	}
	inline void from_json(const json& j, GlTFJsonRepresentation& t)
	{
			j.at("asset").get_to(t.asset);
		OPTIONAL_KEY_GET(scene)
		OPTIONAL_KEY_GET(scenes)
		OPTIONAL_KEY_GET(nodes)
		OPTIONAL_KEY_GET(animations)
		OPTIONAL_KEY_GET(materials)
		OPTIONAL_KEY_GET(meshes)
		OPTIONAL_KEY_GET(skins)
		OPTIONAL_KEY_GET(accessors)
		OPTIONAL_KEY_GET(bufferViews)
		OPTIONAL_KEY_GET(buffers)
	}
	
}
#endif // GLTFFORMAT_H