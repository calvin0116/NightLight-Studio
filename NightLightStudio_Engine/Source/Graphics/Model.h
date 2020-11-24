#pragma once
#include <vector>
#include <map>
#include "Mesh.h"
#include "../Window/WndUtils.h"
#include "../glm/mat4x4.hpp"
#include "../glm/gtx/quaternion.hpp"

namespace NS_GRAPHICS
{
	//Local Path For Custom Graphics Files
	static std::string s_LocalPathName = "Asset\\Model\\";
	static std::string s_ModelFileType = ".model";
	const unsigned MAX_BONE_COUNT = 64;

	struct Animation
	{
		struct KeyFrames
		{
			struct JointTransforms
			{
				glm::vec3 _position;
				glm::quat _rotation;
				glm::vec3 _scale;
			};

			KeyFrames() { _jointTransforms.resize(MAX_BONE_COUNT); _time = 0.0f; }
			~KeyFrames() = default;

			std::vector<JointTransforms> _jointTransforms;
			float _time;
		};

		std::vector<KeyFrames> _frames;
		std::vector<unsigned> _frameCount;
		std::string _animName;

	};

	struct BoneData
	{
		unsigned _boneID;
		std::string _boneName;

		glm::mat4 _boneTransform;
		glm::mat4 _boneTransformOffset;
	};

	struct Model
	{
		GLuint ModelMatrixBO = NULL; // Buffer Object ID for Model Matrix

		std::string _fileName;

		//For static non-animated meshes
		std::vector<Mesh*> _meshes;

		//For animated meshes
		std::vector<AnimatedMesh*> _animatedMeshes;

		bool _isAnimated = false;

		//Holds Animation Data
		std::map<std::string, Animation*> _animations;

		std::vector<BoneData> _bones;
		std::map<std::string, unsigned> _boneMapping;
		unsigned _boneCount = 0;

		glm::mat4 _globalInverseTransform;

		Model() = default;
		~Model() = default;
	};
}