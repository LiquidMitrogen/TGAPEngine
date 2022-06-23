#pragma once
#include <vector>
#include <iostream>
#include <string>
#include <optional>
#include <map>
#include "Keyframe.h"
namespace engine
{
	enum class Interpolation {Step, Linear, CubicSpline	};
	template <class T>
	struct ActionBoneKeyframes
	{
		//int boneID;
		Interpolation interpolation;
		T timeRangeStartRotations, timeRangeEndRotations, timeRangeStartLocations, timeRangeEndLocations;
		std::vector<Keyframe<T, glm::quat>> rotations;
		std::vector<Keyframe<T, glm::vec3>> locations;
	};
	template <class T>
	class Action
	{
		protected:
		std::map<int, ActionBoneKeyframes<T>> boneKeyframes;

		public:
		T rangeStart, rangeEnd;
		std::string name;

		Action(std::string name, std::map<int, ActionBoneKeyframes<T>> boneKeyframes)
		{
			this->name = name;
			T min = 0, max = 0;
			this->boneKeyframes = boneKeyframes;
			for (const auto& [key, value] : boneKeyframes)
			{
				auto lower = std::min(value.timeRangeStartRotations, value.timeRangeStartLocations);
				auto higher = std::max(value.timeRangeEndRotations, value.timeRangeEndLocations);
				min = std::min(min, lower);
				max = std::max(max, higher);
			}
			this->rangeStart = min;
			this->rangeEnd = max;
			//TODO:use ranges in getters
		}

		Action::~Action()
		{
		}

		std::optional<glm::quat> getQuat(int bone, T time)
		{
			auto boneIterator = boneKeyframes.find(bone);
			if (boneIterator != boneKeyframes.end())
			{
				ActionBoneKeyframes<T> boneKeyframes = boneIterator->second;
				if (boneKeyframes.rotations.size() == 0)
				{
					return {};//not animated property
				}
				if (boneKeyframes.rotations.size() == 1)
				{
					return boneKeyframes.rotations[0].value;
				}
				Keyframe<T, glm::quat>* last = nullptr;
				for (std::vector<Keyframe<T, glm::quat>>::iterator kit = boneKeyframes.rotations.begin(); kit != boneKeyframes.rotations.end(); kit++)
				{
					if (kit->time >= time)
					{
						if (kit->time == time)
						{
							return kit->value;
						}
						else
						{
							if (kit == boneKeyframes.rotations.begin())
							{
								return kit->value;//time is before first keyframe
							}
							T interpRatio = (time - last->time) / ((*kit).time - last->time);

							//glm::vec3 vec3Interpolated = glm::mix(last->location, (*kit).location, interpRatio);
							glm::quat quatInterpolated = glm::slerp(last->value, kit->value, interpRatio);
							return quatInterpolated;
						}
					}
					last = &(*kit);
				}
				//time is after first keyframe
				return boneKeyframes.rotations.back().value;
			}
			return {};//not animated
		}

		std::optional<glm::vec3> getLoc(int bone, T time)
		{
			auto boneIterator = boneKeyframes.find(bone);
			if (boneIterator != boneKeyframes.end())
			{
				ActionBoneKeyframes<T> boneKeyframes = boneIterator->second;
				if (boneKeyframes.locations.size() == 0)
				{
					return {};//not animated property
				}
				if (boneKeyframes.locations.size() == 1)
				{
					return boneKeyframes.locations[0].value;
				}
				Keyframe<T, glm::vec3>* last = nullptr;
				for (std::vector<Keyframe<T, glm::vec3>>::iterator kit = boneKeyframes.locations.begin(); kit != boneKeyframes.locations.end(); kit++)
				{
					if (kit->time >= time)
					{
						if (kit->time == time)
						{
							return kit->value;
						}
						else
						{
							if (kit == boneKeyframes.locations.begin())
							{
								return kit->value;//time is before first keyframe
							}
							T interpRatio = (time - last->time) / ((*kit).time - last->time);

							glm::vec3 vec3Interpolated = glm::mix(last->value, kit->value, interpRatio);
							//glm::quat quatInterpolated = glm::slerp(last->quaternion, (*kit).quaternion, interpRatio);
							return vec3Interpolated;
						}
					}
					last = &(*kit);
				}
				//time is after first keyframe
				return boneKeyframes.locations.back().value;
			}
			return {};//not animated
		}
	};
}