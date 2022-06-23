#include "FileReaderGlTF.h"
using json = nlohmann::json;
namespace engine {
	int getIndex(std::vector<int> vector, int value)
	{
		auto iterator = std::find(vector.begin(), vector.end(), value);
		if (iterator != vector.end())
		{
			int index = iterator - vector.begin();
			return index;
		}
		else
			return -1;
	}
	Armature* FileReaderGlTF::loadArmature(GlTFFormat::GlTFSkin &skin, std::vector<GlTFFormat::GlTFNode>& nodes, 
		std::vector<GlTFFormat::GlTFAccessor> &accessors, std::vector<GlTFFormat::GlTFBufferView> &bufferViews,
		char** sceneBuffers)
	{
		//co jak nie ma invMat?
		std::vector<Bone*> boneRoots;
		std::vector<int> jointsCopy = skin.joints;
		std::vector<int> children;
		std::map<int, glm::mat4*> matricesMap;
		std::map<int, int> nodeIndexToJointIndexMap;
		auto accessor = accessors[skin.inverseBindMatrices.value()];
		auto bufferView = bufferViews[accessor.bufferView.value()];
		char* buffer = sceneBuffers[bufferView.buffer];
		int nodeIndexInJoints = 0;
		for (auto jointIndex = skin.joints.begin(); jointIndex != skin.joints.end(); ++jointIndex, ++nodeIndexInJoints)
		{
			nodeIndexToJointIndexMap[*jointIndex] = nodeIndexInJoints;
			float* matrixPtr = (float*)(buffer + bufferView.byteOffset.value_or(0) 
				+ (nodeIndexInJoints * sizeof(float) * 16));
			glm::mat4* invMat;
			invMat = new glm::mat4(1.0f);
			*invMat = glm::make_mat4(matrixPtr);
			matricesMap[*jointIndex] = invMat;
			auto& nodeChildren = nodes[*jointIndex].children.value_or(std::vector<int>());
			for each (auto childIndex in nodeChildren)
			{
				children.push_back(childIndex);
			}
		}
		std::sort(jointsCopy.begin(), jointsCopy.end());
		std::sort(children.begin(), children.end());
		auto last = std::unique(children.begin(), children.end());
		children.erase(last, children.end());
		std::vector<int> notChildren;
		std::set_difference(jointsCopy.begin(), jointsCopy.end(), children.begin(), children.end(),
			std::inserter(notChildren, notChildren.begin()));
		for each (auto index in notChildren)
		{
			auto& rootBone = nodes[index];
			//auto nodeIndexInJoints = getIndex(skin.joints, index);
			boneRoots.push_back(loadBones(nodes, matricesMap, index, nodeIndexToJointIndexMap));
		}
		Armature* newArmature;
		newArmature = new Armature(boneRoots[0]);
		return newArmature;
	}
	Bone* FileReaderGlTF::loadBones(std::vector<GlTFFormat::GlTFNode> &nodes, std::map<int, glm::mat4*> &inverseMatrices, 
		int nodeIndex, std::map<int, int> &nodeIndexToJointIndexMap)
	{
		
		auto& currentNode = nodes[nodeIndex];
		auto& children = currentNode.children.value_or(std::vector<int>());
		std::string boneName;
		if (currentNode.name.has_value())
			boneName = currentNode.name.value();
		else
			boneName = std::to_string(nodeIndex);

		Bone* bone = new Bone(nodeIndexToJointIndexMap.at(nodeIndex), nodeIndex, inverseMatrices.at(nodeIndex), boneName);
		if (currentNode.translation.has_value())
		{
			auto& translation = currentNode.translation.value();
			bone->setLocation(glm::vec3(translation[0], translation[1], translation[2]));
		}
		if (currentNode.rotation.has_value())
		{
			auto& rotation = currentNode.rotation.value();
			bone->setQuaternion(glm::quat(rotation[3], rotation[0], rotation[1], rotation[2]));
		}
		for each (auto childNodeIndex in children)
		{
			bone->children.emplace_back(loadBones(nodes, inverseMatrices, childNodeIndex, nodeIndexToJointIndexMap));
		}
		return bone;
	}
	Scene* FileReaderGlTF::loadScene(const char filepath[], bool containHeader) {
		auto pathToGltf = std::filesystem::path(filepath);
		std::ifstream file;
		file.open(filepath, std::ios::in | std::ios::binary);
		if (file.is_open())
			std::cout << "otwarto plik " << filepath << std::endl;
		else
			std::cout << "Error: " << strerror(errno);
		json gltfJson = json::parse(file);
		auto gltfHeader = gltfJson.get<GlTFFormat::GlTFJsonRepresentation>();
		file.close();
		//Action<float> dupa(std::string("aa"), 1.0f, 1.0f);
		Scene * newScene = new Scene();
		if (gltfHeader.buffers.value().size() > 0)
		{
			char ** sceneBuffers = new char * [gltfHeader.buffers.value().size()];
			int index = 0;
			for (auto gltfBuffer = gltfHeader.buffers.value().begin(); gltfBuffer != gltfHeader.buffers.value().end(); ++gltfBuffer, ++index)
			{
				auto pathToBin = pathToGltf.replace_filename(gltfBuffer->uri.value());
				file.open(pathToBin, std::ios::in | std::ios::binary);
				if (file.is_open())
					std::cout << "otwarto plik " << filepath << std::endl;
				else
					std::cout << "Error: " << strerror(errno);
				char* buffer;
				buffer = new char[gltfBuffer->byteLength];
				file.read(buffer, gltfBuffer->byteLength);
				file.close();
				sceneBuffers[index] = buffer;
			}
			auto nodes = gltfHeader.nodes.value_or(std::vector<GlTFFormat::GlTFNode>());
			auto* armature = loadArmature(gltfHeader.skins.value()[0], nodes, gltfHeader.accessors.value(), 
				gltfHeader.bufferViews.value(), sceneBuffers);

			if (gltfHeader.animations.has_value())
			{
				auto actions = loadActions<float>(gltfHeader.animations.value(),
					gltfHeader.accessors.value(), gltfHeader.bufferViews.value(), sceneBuffers);
				for each (auto action in actions)
				{
					armature->addAction(action);
				}
			}


			for each (auto mesh in gltfHeader.meshes.value())
			{
				if (mesh.name == "g ZBrush_defualt_group_1_0")
					std::cout << std::endl;
				auto& indicesAccessor = gltfHeader.accessors.value()[(mesh.primitives[0].indices.value())];
				auto& indiceBufferView = gltfHeader.bufferViews.value()[indicesAccessor.bufferView.value()];
				char* indiceBuffer = sceneBuffers[indiceBufferView.buffer];

				auto& positionAccessorIndex = mesh.primitives[0].attributes["POSITION"];
				auto& positionAccessor = gltfHeader.accessors.value()[positionAccessorIndex];
				auto& positionBufferView = gltfHeader.bufferViews.value()[positionAccessor.bufferView.value()];
				char* positionBuffer = sceneBuffers[positionBufferView.buffer];
				float* positions = (float*)(positionBuffer + positionBufferView.byteOffset.value());

				auto& normalAccessorIndex = mesh.primitives[0].attributes["NORMAL"];
				auto& normalAccessor = gltfHeader.accessors.value()[normalAccessorIndex];
				auto& normalBufferView = gltfHeader.bufferViews.value()[normalAccessor.bufferView.value()];
				char* normalBuffer = sceneBuffers[normalBufferView.buffer];
				float* normals = (float*)(normalBuffer + normalBufferView.byteOffset.value());

				auto& jointAccessorIndex = mesh.primitives[0].attributes["JOINTS_0"];
				auto& jointAccessor = gltfHeader.accessors.value()[jointAccessorIndex];
				auto& jointBufferView = gltfHeader.bufferViews.value()[jointAccessor.bufferView.value()];
				char* jointBuffer = sceneBuffers[jointBufferView.buffer];
				unsigned char* joints = (unsigned char*)(jointBuffer + jointBufferView.byteOffset.value());

				auto& weightAccessorIndex = mesh.primitives[0].attributes["WEIGHTS_0"];
				auto& weightAccessor = gltfHeader.accessors.value()[weightAccessorIndex];
				auto& weightBufferView = gltfHeader.bufferViews.value()[weightAccessor.bufferView.value()];
				char* weightBuffer = sceneBuffers[weightBufferView.buffer];
				unsigned char* weights = (unsigned char*)(weightBuffer + weightBufferView.byteOffset.value());

				char*positionsAndNormals = (positionBuffer + positionBufferView.byteOffset.value());

				std::vector<char> pos(positionsAndNormals, positionsAndNormals + positionAccessor.count * 3 * 4 + 
					normalAccessor.count * 3 * 4 +
					jointAccessor.count * 4 * 1 +
					weightAccessor.count * 4 * 4);

				VaoInitData* vaoInit = new VaoInitData;
				vaoInit->attrNumber = 4;
				vaoInit->attrOffset[0] = positionBufferView.byteOffset.value_or(0u);
				vaoInit->attrOffset[1] = normalBufferView.byteOffset.value_or(0u);
				vaoInit->attrOffset[2] = jointBufferView.byteOffset.value_or(0u);
				vaoInit->attrOffset[3] = weightBufferView.byteOffset.value_or(0u);
				vaoInit->attrSize[0] = 3;
				vaoInit->attrSize[1] = 3;
				vaoInit->attrSize[2] = 4;
				vaoInit->attrSize[3] = 4;
				vaoInit->attrType[0] = positionAccessor.componentType;
				vaoInit->attrType[1] = normalAccessor.componentType;
				vaoInit->attrType[2] = jointAccessor.componentType;
				vaoInit->attrType[3] = weightAccessor.componentType;
				vaoInit->attrNormBool[0] = GL_FALSE;
				vaoInit->attrNormBool[1] = GL_FALSE;
				vaoInit->attrNormBool[2] = GL_FALSE;
				vaoInit->attrNormBool[3] = GL_FALSE;

				VertexAttributes* VAO;
				if (indicesAccessor.componentType == GL_UNSIGNED_SHORT)
				{
					unsigned short* indices = (unsigned short*)(indiceBuffer + indiceBufferView.byteOffset.value_or(0));
					std::vector<unsigned short> ind(indices, indices + indicesAccessor.count);
					VAO = new VertexAttributes(pos, ind, *vaoInit);
				}
				else
				{
					unsigned int* indices = (unsigned int*)(indiceBuffer + indiceBufferView.byteOffset.value_or(0));
					std::vector<unsigned int> ind(indices, indices + indicesAccessor.count);
					VAO = new VertexAttributes(pos, ind, *vaoInit);
				}
				//TODO:no chyba nie tak to ma byæ::::::
				AnimatedActor* e = new AnimatedActor(VAO, "");
				e->name = mesh.name.value();
				SingleMatrixMaterial* entitymaterial = new SingleMatrixMaterial("vertex.vert", "fragment.frag");
				e->entityMaterial = entitymaterial;
				e->assignArmature(armature);
				/*e->disableDrawing = true;
				if (e->name == "g ZBrush_defualt_group_1_0")
					e->disableDrawing = false;*/
				newScene->addEntity(e);
			}
		}
		else
		{
			std::cout << "plik gltf bez buffers" << std::endl;
			throw;
		}


	/*		primitive.mode
			0 POINTS
			1 LINES
			2 LINE_LOOP
			3 LINE_STRIP
			4 TRIANGLES
			5 TRIANGLE_STRIP
			6 TRIANGLE_FAN*/



		//5120 BYTE

		//	5121 UNSIGNED_BYTE

		//	5122 SHORT

		//	5123 UNSIGNED_SHORT

		//	5125 UNSIGNED_INT

		//	5126 FLOAT


		//std::cout << gltfJson.dump(4) << std::endl;
		//loadNoBoneFile();
		//unsigned short tag;
		//file.read((char*)&tag, sizeof(unsigned short));
		////std::cout << sizeof(unsigned short) << std::endl;
		//std::cout << tag << std::endl;
		//if (tag != 0x4241) {
		//	std::cout << "BAM scene file not recognized" << std::endl;
		//	system("pause");
		//	exit(1);
		//}
		//unsigned char* objectCount = new unsigned char;
		//file.read((char*)objectCount, sizeof(unsigned char));
		//std::cout << "objectCount " << (unsigned int)*objectCount << std::endl;
		//struct ObjectTreeElement* objectTree = new struct ObjectTreeElement[*objectCount];
		//for (int i = 0; i < *objectCount; i++) {
		//	unsigned char objectId = 0;
		//	file.read((char*)&objectId, sizeof(unsigned char));
		//	std::cout << "objectId " << (unsigned int)objectId << std::endl;
		//	unsigned char childrenCount = 0;
		//	file.read((char*)&childrenCount, sizeof(unsigned char));
		//	std::cout << "childrenCount " << (unsigned int)childrenCount << std::endl;
		//	objectTree[objectId].childrenCount = childrenCount;
		//	objectTree[objectId].id = objectId;
		//	if (childrenCount > 0)
		//		objectTree[objectId].childrenIds = new unsigned char[childrenCount];
		//	for (int j = 0; j < childrenCount; j++) {
		//		unsigned char childId = 0;
		//		file.read((char*)&childId, sizeof(unsigned char));
		//		std::cout << "childId " << (unsigned int)childId << std::endl;
		//		objectTree[objectId].childrenIds[j] = childId;
		//	}
		//}

		//Entity** entityArray = new Entity * [*objectCount];

		//std::vector<AnimatedActor*> animatedEntities;
		//std::vector<Armature*> armatures;

		//for (int i = 0; i < *objectCount; i++) {
		//	struct Header header = this->loadHeader();
		//	if (header.type == EntityType::ArmatureT) {
		//		Armature* newArmature = this->loadArmature();
		//		entityArray[i] = newArmature;
		//		armatures.push_back(newArmature);
		//		newArmature->disableDrawing = true;
		//	}
		//	else {
		//		if (header.configurationFlags & 0x04) {//object has no bones
		//			entityArray[i] = this->loadNoBoneFile();
		//		}
		//		else {
		//			AnimatedActor* newActor = this->loadMeshFile();
		//			entityArray[i] = newActor;
		//			animatedEntities.push_back(newActor);
		//		}
		//	}
		//	entityArray[i]->name = header.entityName;
		//	entityArray[i]->buildId = header.id;
		//	entityArray[i]->applyTranslation(header.location);
		//	std::cout << header.location.x << " " << header.location.y << " " << header.location.z << std::endl;
		//	entityArray[i]->applyRotation(header.rotation);
		//	std::cout << header.rotation.x << " " << header.rotation.y << " " << header.rotation.z << std::endl;
		//	entityArray[i]->applyScale(header.scale);
		//	if (header.type != EntityType::ArmatureT) {
		//		SingleMatrixMaterial* entitymaterial = new SingleMatrixMaterial(header.vertexShaderStr.c_str(), header.fragmentShaderStr.c_str());
		//		entityArray[i]->entityMaterial = entitymaterial;
		//	}
		//	//else{
		//	//	entityArray[i] = NULL;//removes aramtures from scene hierarchy
		//	//}

		//}
		//for (std::vector<AnimatedActor*>::iterator anit = animatedEntities.begin(); anit != animatedEntities.end(); anit++) {
		//	for (std::vector<Armature* >::iterator arit = armatures.begin(); arit != armatures.end(); arit++) {
		//		if ((*anit)->armatureName == (*arit)->name) {
		//			(*anit)->assignArmature((*arit));
		//		}
		//	}

		//}
		//for (int i = 0; i < *objectCount; i++) {//iteracja po objectTree
		//	unsigned int entityArrayId = 0;//pozycja w tablicy gotowego obiejtu o danym id
		//	for (int g = 0; g < *objectCount; g++) {
		//		if (entityArray[g]->buildId == i) {
		//			entityArrayId = g;
		//		}
		//	}
		//	for (int j = 0; j < objectTree[i].childrenCount; j++) {//itaracja po potomkach w objectTree
		//		unsigned int childId = objectTree[i].childrenIds[j];
		//		for (int k = 0; k < *objectCount; k++) {//iteracja po elementach entityArray w celu znalezienia potomka
		//			if (entityArray[k]->buildId == childId) {//potomek znaleziony ma pozycje k w tabeli
		//				entityArray[entityArrayId]->addChild(entityArray[k]);
		//				entityArray[k]->setParent(entityArray[entityArrayId]);
		//				std::cout << entityArray[entityArrayId]->buildId << " has child " << entityArray[k]->buildId << std::endl;
		//			}
		//		}
		//	}
		//}


		//for (int i = 0; i < *objectCount; i++) {
		//	std::cout << i << " Has " << (unsigned int)objectTree[i].childrenCount << " children: " << std::endl;
		//	for (int j = 0; j < objectTree[i].childrenCount; j++) {
		//		std::cout << (unsigned int)objectTree[i].childrenIds[j] << std::endl;
		//	}
		//}

		//for (int i = 0; i < *objectCount; i++) {
		//	if (entityArray[i] != NULL && entityArray[i]->getParent() == NULL) {
		//		newScene->addEntity(entityArray[i]);
		//		std::cout << "entity #" << entityArray[i]->buildId << " added to scene" << std::endl;
		//	}
		//}

		////cleanup---------------------------------
		//for (int i = 0; i < *objectCount; i++) {
		//	if (objectTree[i].childrenCount > 0)
		//		delete[] objectTree[i].childrenIds;
		//}
		//delete[] objectTree;
		//delete objectCount;


		return newScene;
	}
}