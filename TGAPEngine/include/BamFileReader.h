#ifndef BAMFILEREADER_H
#define BAMFILEREADER_H
#include <fstream>
#include <iostream>
#include <string>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include "VertexAttributes.h"
#include "Entity.h"
#include "Bone.h"
#include "Action.h"
#include "AnimatedActor.h"
#include "Config.h"
#include "Renderer.h"
#include "Armature.h"
namespace engine{

	enum EntityType  { MeshT, ArmatureT };
	struct ObjectTreeElement {
		unsigned char id;
		unsigned char * childrenIds;
		unsigned char childrenCount;
	};
	struct Header{
		EntityType type;
		unsigned int id;
		unsigned char configurationFlags;
		glm::quat rotation;
		glm::vec3 location, scale;
		std::string entityName,vertexShaderStr,fragmentShaderStr;
	};

class BamFileReader
{
    public:
        BamFileReader();
        virtual ~BamFileReader();
		AnimatedActor * loadMeshFile();
		Entity * loadNoBoneFile();
		Scene * loadScene(const char filepath[]);
		Entity * loadEntity(const char filepath[]);
    protected:
		Keyframe loadKeyframe();
		Action loadAction(unsigned int boneNum);
		Bone * loadBones(unsigned int boneCount);
		Armature * loadArmature();
		struct Header loadHeader();
		std::ifstream file;
    private:
};
}
#endif // BAMFILEREADER_H
