#pragma once
#include <vector>
#include <map>
#include "Mesh.h"
#include "../Window/WndUtils.h"
#include "Animation.h"
#include "Skeleton.h"

namespace NS_GRAPHICS
{
	//Local Path For Custom Graphics Files
	static std::string s_LocalPathName = "Asset\\Model\\";
	static std::string s_ModelFileType = ".model";
	const unsigned MAX_BONE_COUNT = 100;
	const unsigned MAX_BONE_INFLUENCE = 4;

	//This is just for storing all the ai nodes
	struct Node
	{
		std::string _nodeName;
		glm::mat4 _nodeTransform;

		std::vector<Node> _childrenNode;
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

		Skeleton* _rootBone = nullptr;

		//Should put where for this? Temporary or permanently here for now
		glm::mat4 _globalInverseTransform = glm::mat4(1.0f);
		std::vector<glm::mat4> _poseTransform;

		//Temporary put here first
		Node _rootNode;

		Model();
		~Model() = default;

		//Calculation for animation
		void GetPose(const std::string& animName, Joint& joint, double dt, glm::mat4& parentTransform, glm::mat4& globalInverseTransform);
		void InterpTime(unsigned& index, float& time, double& dt, std::vector<double>& times);
	};
}