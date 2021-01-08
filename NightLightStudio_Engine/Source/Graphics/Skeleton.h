#pragma once
#include <vector>
#include <string>
#include <map>
#include "../glm/vec4.hpp"
#include "../glm/vec3.hpp"
#include "../glm/vec2.hpp"
#include "../glm/mat4x4.hpp"

namespace NS_GRAPHICS
{
	struct Joint
	{
		unsigned _boneID;
		std::string _boneName;
		glm::mat4 _boneTransformOffset = glm::mat4(1.0f);
		glm::mat4 _boneTransform = glm::mat4(1.0f);

		std::vector<Joint> _childrenJoints;

		Joint() = default;
		~Joint() = default;
	};

	struct Skeleton
	{
		Joint _rootJoint;

		std::map<std::string, std::pair<unsigned, glm::mat4>> _boneMapping;
		unsigned _boneCount = 0;


		Skeleton() = default;
		~Skeleton() = default;
	};
}