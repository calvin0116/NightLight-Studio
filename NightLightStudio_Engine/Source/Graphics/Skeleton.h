#pragma once
#include <vector>
#include "../glm/vec4.hpp"
#include "../glm/vec3.hpp"
#include "../glm/vec2.hpp"
#include "../glm/mat4x4.hpp"

namespace NS_GRAPHICS
{
	struct Joint
	{
		unsigned _index;
		glm::mat4 _transform;
		std::vector<Joint> _childrenJoints;

		Joint() = default;
		~Joint() = default;
	};

	struct Skeleton
	{
		Joint _rootJoint;
		int jointCounts;

		Skeleton() = default;
		~Skeleton() = default;
	};
}