#pragma once
#include <vector>
#include <map>
#include "Mesh.h"
#include "../Window/WndUtils.h"

namespace NS_GRAPHICS
{
	//Local Path For Custom Graphics Files
	static std::string s_LocalPathName = "Asset\\Model\\";
	static std::string s_ModelFileType = ".model";

	//struct Animation
	//{
	//	struct KeyFrames
	//	{
	//		struct JointTransforms
	//		{
	//			glm::vec3 _position;
	//			glm::vec3 _rotation;
	//		};

	//		std::vector<JointTransforms> _jointTransforms;
	//		float _time;
	//	};

	//	std::vector<KeyFrames> _frames;
	//	std::string _animName;

	//};

	struct Model
	{
		GLuint ModelMatrixBO = NULL; // Buffer Object ID for Model Matrix

		std::string _fileName;

		//For static non-animated meshes
		std::vector<Mesh*> _meshes;

		//For animated meshes
		std::vector<AnimatedMesh*> _animatedMeshes;

		bool  _isAnimated;

		//std::map<std::string, Animation*> _animations;

		Model() = default;
		~Model() = default;
	};
}