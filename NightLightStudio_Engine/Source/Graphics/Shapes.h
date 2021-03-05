#pragma once
#include "../glm/glm.hpp"   // glm::vec3 namespace

namespace NS_GRAPHICS
{
	// Geometry types
	struct MyPlane
	{
		// Normal
		glm::vec3 Normal;
		// Get the 'd' value (the distance from the origin) which can be computed as Dot(origin - pointOnPlane, normal)
		float Distance;
	};

	struct MyFrustum
	{
		enum : size_t
		{
			Near = 0,
			Far,
			Left,
			Right,
			Top,
			Btm
		};

		// Planes
		MyPlane Plane[6];
		// To easily draw the frustum the original points it was constructed from are stored.
		glm::vec3 Points[8];
	};
}