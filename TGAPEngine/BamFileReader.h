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
#include "BoneFrameMatrix.h"
#include "AnimatedActor.h"
#include "Config.h"
#include "Renderer.h"
namespace engine{


	struct Header{
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
        AnimatedActor * loadFile();
		Entity * loadNoBoneFile();
		Scene * loadScene(const char filepath[], Renderer * renderer);
		Entity * loadEntity(const char filepath[]);
    protected:
		struct Header loadHeader();
		std::ifstream file;
    private:
};
}
#endif // BAMFILEREADER_H
