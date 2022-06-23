#ifndef FILEREADERGLTF_H
#define FILEREADERGLTF_H
#include <fstream>
#include <iostream>
#include <string>
#include <filesystem>
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
#include <nlohmann/json.hpp>
#include "GlTFJsonRepresentation.h"
namespace engine
{
	class FileReaderGlTF
	{
		public:
		Scene* loadScene(const char filepath[], bool containHeader);
		protected:

		template <class T> std::vector<Keyframe<T, glm::quat >> loadRotations(GlTFFormat::GlTFSampler& sampler,
			GlTFFormat::GlTFAccessor& input, GlTFFormat::GlTFAccessor& output,
			std::vector<GlTFFormat::GlTFBufferView>& bufferViews, char** buffers);

		template <class T>
		std::vector<Keyframe<T, glm::vec3 >> loadLocations(GlTFFormat::GlTFSampler& sampler,
			GlTFFormat::GlTFAccessor& input, GlTFFormat::GlTFAccessor& output,
			std::vector<GlTFFormat::GlTFBufferView>& bufferViews, char** buffers);

		template <class T>
		std::map<int, ActionBoneKeyframes<T>> loadAnimationKeyframes(GlTFFormat::GlTFAnimation& animation,
			std::vector<GlTFFormat::GlTFAccessor> &accessors, std::vector<GlTFFormat::GlTFBufferView>& bufferViews,
			char** buffer);

		template <class T>
		std::vector<Action<T>*> loadActions(std::vector<GlTFFormat::GlTFAnimation>& animations,
			std::vector<GlTFFormat::GlTFAccessor> &accessors, std::vector<GlTFFormat::GlTFBufferView>& bufferViews,
			char** buffers);

		Armature* loadArmature(GlTFFormat::GlTFSkin& skin, std::vector<GlTFFormat::GlTFNode>& nodes,
			std::vector<GlTFFormat::GlTFAccessor>& accessors, std::vector<GlTFFormat::GlTFBufferView>& bufferViews,
			char** sceneBuffers);

		Bone* loadBones(std::vector<GlTFFormat::GlTFNode>& nodes, std::map<int, glm::mat4*>& inverseMatrices, int nodeIndex, std::map<int, int>& nodeIndexToJointIndexMap);
	};
	template <class T>
	std::vector<Keyframe<T, glm::quat >> FileReaderGlTF::loadRotations(GlTFFormat::GlTFSampler& sampler,
		GlTFFormat::GlTFAccessor& input, GlTFFormat::GlTFAccessor& output,
		std::vector<GlTFFormat::GlTFBufferView>& bufferViews, char** buffers)
	{
		std::vector<Keyframe<T, glm::quat>> result;
		auto inputBufferView = bufferViews[input.bufferView.value_or(0)];
		auto inputBuffer = buffers[inputBufferView.buffer];
		auto inputBufferOfType = (T*)(inputBuffer + inputBufferView.byteOffset.value_or(0));

		auto outputBufferView = bufferViews[output.bufferView.value_or(0)];
		auto outputBuffer = buffers[outputBufferView.buffer];
		auto outputBufferOfType = (float*)(outputBuffer + outputBufferView.byteOffset.value_or(0));
		//if (input.count != output.count)
		//	__debugbreak();
		for (int i = 0; i < input.count; i++)
		{
			result.emplace_back(inputBufferOfType[i], glm::quat(outputBufferOfType[i * 4 + 3],
				outputBufferOfType[i * 4], outputBufferOfType[i * 4 + 1], outputBufferOfType[i * 4 + 2]));
		}
		return result;
	}
	template <class T>
	std::vector<Keyframe<T, glm::vec3 >> FileReaderGlTF::loadLocations(GlTFFormat::GlTFSampler& sampler,
		GlTFFormat::GlTFAccessor& input, GlTFFormat::GlTFAccessor& output,
		std::vector<GlTFFormat::GlTFBufferView>& bufferViews, char** buffers)
	{
		std::vector<Keyframe<T, glm::vec3>> result;
		auto inputBufferView = bufferViews[input.bufferView.value_or(0)];
		auto inputBuffer = buffers[inputBufferView.buffer];
		auto inputBufferOfType = (T*)(inputBuffer + inputBufferView.byteOffset.value_or(0));

		auto outputBufferView = bufferViews[output.bufferView.value_or(0)];
		auto outputBuffer = buffers[outputBufferView.buffer];
		auto outputBufferOfType = (float*)(outputBuffer + outputBufferView.byteOffset.value_or(0));
		//if (input.count != output.count)
		//	__debugbreak();
		for (int i = 0; i < input.count; i++)
		{
			result.emplace_back(inputBufferOfType[i], glm::vec3(outputBufferOfType[i * 3 + 0],
				outputBufferOfType[i * 3 + 1], outputBufferOfType[i * 3 + 2]));
		}
		return result;
	}
	template <class T>
	std::map<int, ActionBoneKeyframes<T>> FileReaderGlTF::loadAnimationKeyframes(GlTFFormat::GlTFAnimation& animation,
		std::vector<GlTFFormat::GlTFAccessor> &accessors, std::vector<GlTFFormat::GlTFBufferView>& bufferViews,
		char** buffers)
	{
		std::map<int, ActionBoneKeyframes<float>> result;
		for(auto &channel = animation.channels.begin(); channel != animation.channels.end(); ++channel)
		{
			auto path = channel->target.path;
			if (path == "scale")
				continue;
			auto boneNode = channel->target.node.value();
			auto sampler = animation.samplers[channel->sampler];
			auto inputAccessor = accessors[sampler.input];
			auto outputAccessor = accessors[sampler.output];
			Interpolation interpolation = Interpolation::Linear;
			auto interpolationString = sampler.interpolation.value_or("LINEAR");
			if (interpolationString == "STEP") interpolation = Interpolation::Step;
			if (interpolationString == "CUBICSPLINE") interpolation = Interpolation::CubicSpline;

			auto resultForBone = result.find(boneNode);
			auto& actionBoneKeyframe = ActionBoneKeyframes<float>();
			if (resultForBone != result.end())
			{
				actionBoneKeyframe = resultForBone->second;
				if (path == "translation")
				{
					if (inputAccessor.min.has_value())
						actionBoneKeyframe.timeRangeStartLocations = inputAccessor.min.value()[0];
					if (inputAccessor.max.has_value())
						actionBoneKeyframe.timeRangeEndLocations = inputAccessor.max.value()[0];
					actionBoneKeyframe.locations = loadLocations<T>(sampler, inputAccessor, outputAccessor, bufferViews, buffers);
				}
				if (path == "rotation")
				{
					if (inputAccessor.min.has_value())
						actionBoneKeyframe.timeRangeStartRotations = inputAccessor.min.value()[0];
					if (inputAccessor.max.has_value())
						actionBoneKeyframe.timeRangeEndRotations = inputAccessor.max.value()[0];
					actionBoneKeyframe.rotations = loadRotations<T>(sampler, inputAccessor, outputAccessor, bufferViews, buffers);
				}
				result[boneNode] = actionBoneKeyframe;
			}
			else
			{
				if (path == "translation")
				{
					if (inputAccessor.min.has_value())
						actionBoneKeyframe.timeRangeStartLocations = inputAccessor.min.value()[0];
					if (inputAccessor.max.has_value())
						actionBoneKeyframe.timeRangeEndLocations = inputAccessor.max.value()[0];
					actionBoneKeyframe.locations = loadLocations<T>(sampler, inputAccessor, outputAccessor, bufferViews, buffers);
				}
				if (path == "rotation")
				{
					if (inputAccessor.min.has_value())
						actionBoneKeyframe.timeRangeStartRotations = inputAccessor.min.value()[0];
					if (inputAccessor.max.has_value())
						actionBoneKeyframe.timeRangeEndRotations = inputAccessor.max.value()[0];
					actionBoneKeyframe.rotations = loadRotations<T>(sampler, inputAccessor, outputAccessor, bufferViews, buffers);
				}
				actionBoneKeyframe.interpolation = interpolation;
				result[boneNode] = actionBoneKeyframe;
			}

			
		}
		return result;
	}

	template <class T>
	std::vector<Action<T>*> FileReaderGlTF::loadActions(std::vector<GlTFFormat::GlTFAnimation>& animations,
		std::vector<GlTFFormat::GlTFAccessor> &accessors, std::vector<GlTFFormat::GlTFBufferView>& bufferViews,
		char** buffers)
	{
		std::vector<Action<T>*> result;
		int index = 0;
		for (auto animation = animations.begin(); animation != animations.end(); ++animation,++index)
		{
			auto animationName = animation->name.value_or(std::to_string(index));
			result.push_back(new Action<T>(animationName, loadAnimationKeyframes<T>(*animation, accessors, bufferViews, buffers)));
		}
		return result;
	}
}
#endif // FILEREADERGLTF_H

