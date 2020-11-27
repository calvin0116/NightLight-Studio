#include "Model.h"

NS_GRAPHICS::Model::Model()
{
	_poseTransform.resize(MAX_BONE_COUNT, glm::mat4(1.0f));
}

void NS_GRAPHICS::Model::GetPose(const std::string& animName, BoneData& bone, float dt, glm::mat4& parentTransform, glm::mat4& globalInverseTransform)
{
	glm::mat4 globalTransform = glm::mat4(1.0f);
	glm::mat4 localTransform = glm::mat4(1.0f);
	if (_animations.find(animName) != _animations.end())
	{
		if (_animations[animName]->_frames.find(bone._boneName) != _animations[animName]->_frames.end())
		{
			Animation::KeyFrames& currKey = _animations[animName]->_frames[bone._boneName];
			dt = fmod(dt, _animations[animName]->_time);

			unsigned index = 0;
			float timeScale = 0.0f;
			InterpTime(index, timeScale, dt, _animations[animName]->_frames[bone._boneName]._posTime);

			glm::vec3 startPos = _animations[animName]->_frames[bone._boneName]._position[index - 1];
			glm::vec3 endPos = _animations[animName]->_frames[bone._boneName]._position[index];

			glm::vec3 newPosition = glm::mix(startPos, endPos, timeScale);

			index = 0;
			timeScale = 0.0f;
			InterpTime(index, timeScale, dt, _animations[animName]->_frames[bone._boneName]._rotateTime);

			glm::quat startRot = _animations[animName]->_frames[bone._boneName]._rotation[index - 1];
			glm::quat endRot = _animations[animName]->_frames[bone._boneName]._rotation[index];

			glm::quat newRotation = glm::slerp(startRot, endRot, timeScale);

			index = 0;
			timeScale = 0.0f;
			InterpTime(index, timeScale, dt, _animations[animName]->_frames[bone._boneName]._scaleTime);

			glm::vec3 startScale = _animations[animName]->_frames[bone._boneName]._scale[index - 1];
			glm::vec3 endScale = _animations[animName]->_frames[bone._boneName]._scale[index];

			glm::vec3 newScale = glm::mix(startScale, endScale, timeScale);

			glm::mat4 positionMat = glm::mat4(1.0);
			glm::mat4 scaleMat = glm::mat4(1.0);

			positionMat = glm::translate(positionMat, newPosition);
			scaleMat = glm::scale(scaleMat, newScale);

			glm::mat4 rotationMat = glm::toMat4(newRotation);

			localTransform = positionMat * rotationMat * scaleMat;
			globalTransform = parentTransform * localTransform;

			_poseTransform[bone._boneID] = globalInverseTransform * globalTransform * bone._boneTransformOffset;
		}
	}
	else
	{
		globalTransform = parentTransform * localTransform;
	}



	//update values for children bones
	for (BoneData& child : bone._childrenBones)
	{
		GetPose(animName, child, dt, globalTransform, globalInverseTransform);
	}
}

void NS_GRAPHICS::Model::InterpTime(unsigned& index, float& time, float& dt, std::vector<float>& times)
{
	//Assumes dt will always be within the duration of animation
	unsigned currIndex = 0;
	while (times[currIndex] < dt)
	{
		++currIndex;
	}

	//Get the time of the next frame and the current frame
	float startTime = times[currIndex - 1];
	float endTime = times[currIndex];

	//Get the scale value of the time between 0.0f to 1.0f between start and end
	time = (dt - startTime) / (endTime - startTime);
	index = currIndex;
}
