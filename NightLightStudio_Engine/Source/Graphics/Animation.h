#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include "../glm/mat4x4.hpp"
#include "../glm/gtx/quaternion.hpp"

namespace NS_GRAPHICS
{
	struct Animation
	{
		struct KeyFrames
		{
			std::vector<double> _posTime;
			std::vector<double> _rotateTime;
			std::vector<double> _scaleTime;

			std::vector <glm::vec3> _position;
			std::vector <glm::quat> _rotation;
			std::vector <glm::vec3> _scale;

			KeyFrames() = default;
			~KeyFrames() = default;
		};

		std::string _animName;
		double _time = 0.0f;
		std::unordered_map<std::string, KeyFrames> _frames;
	};
}