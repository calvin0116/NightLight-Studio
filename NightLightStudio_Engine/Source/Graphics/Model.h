#pragma once
#include <vector>
#include <map>
#include <unordered_map>
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
	const unsigned MAX_BONE_INFLUENCE = 4;

	struct BoneData
	{
		unsigned _boneID;
		std::string _boneName;

		glm::mat4 _boneTransform = glm::mat4(1.0f);
		glm::mat4 _boneTransformOffset = glm::mat4(1.0f);

		std::vector<BoneData> _childrenBones;
	};

	struct Animation
	{
		struct KeyFrames
		{
			std::vector<float> _posTime;
			std::vector<float> _rotateTime;
			std::vector<float> _scaleTime;

			std::vector <glm::vec3> _position;
			std::vector <glm::quat> _rotation;
			std::vector <glm::vec3> _scale;

			KeyFrames() = default;
			~KeyFrames() = default;
		};

		std::string _animName;
		float _time = 0.0f;
		float _ticksPerSecond = 1.0f;
		std::unordered_map<std::string, KeyFrames> _frames;
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

		BoneData _rootBone;
		std::map<std::string, std::pair<unsigned, glm::mat4>> _boneMapping;
		unsigned _boneCount = 0;

		glm::mat4 _globalInverseTransform = glm::mat4(1.0f);
		std::vector<glm::mat4> _poseTransform;

		Model();
		~Model() = default;

		//Calculation for animation
		void GetPose(const std::string& animName, BoneData& bone, float dt, glm::mat4& parentTransform, glm::mat4& globalInverseTransform);
		void InterpTime(unsigned& index, float& time, float& dt, std::vector<float>& times);
	};
}