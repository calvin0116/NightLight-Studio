#pragma once
#include "../FBX_SDK/fbxsdk.h"

namespace NS_GRAPHICS
{
	struct Mesh
	{
		FbxScene* fbxScene;

		Mesh() = default;
		~Mesh() = default;
	};
}