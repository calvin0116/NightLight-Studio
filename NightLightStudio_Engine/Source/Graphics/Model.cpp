#include "Model.h"

NS_GRAPHICS::Model::Model()
{
	_poseTransform.resize(MAX_BONE_COUNT, glm::mat4(1.0f));
}

void NS_GRAPHICS::Model::GetPose(const std::string& animName, BoneData& bone, double dt, glm::mat4& parentTransform, glm::mat4& globalInverseTransform)
{
	// ASSUMES IF IS BAKED AND ALL BONES AND FRAME EXISTS
	glm::mat4 globalTransform(1.0f);
	glm::mat4 localTransform(1.0f);
	//glm::mat4 localTransform = bone._boneTransform;

	Animation::KeyFrames& currKey = _animations[animName]->_frames[bone._boneName];
	dt = fmod(dt, _animations[animName]->_time);

	unsigned index = 0;
	float timeScale = 0.0f;

	glm::vec3 newPosition;
	glm::vec3 startPos;
	glm::vec3 endPos;
	bool translation = false;
	if (_animations[animName]->_frames[bone._boneName]._posTime.size() > 1)
	{
		InterpTime(index, timeScale, dt, _animations[animName]->_frames[bone._boneName]._posTime);
		startPos = _animations[animName]->_frames[bone._boneName]._position[index - 1];
		endPos = _animations[animName]->_frames[bone._boneName]._position[index];
		newPosition = glm::mix(startPos, endPos, timeScale);
		translation = true;
	}
	else if (_animations[animName]->_frames[bone._boneName]._posTime.size() == 1)
	{
		newPosition = _animations[animName]->_frames[bone._boneName]._position[0];
	}

	index = 0;
	timeScale = 0.0f;

	glm::quat newRotation;
	glm::quat startRot;
	glm::quat endRot;

	bool rotation = false;
	if (_animations[animName]->_frames[bone._boneName]._rotateTime.size() > 1)
	{
		InterpTime(index, timeScale, dt, _animations[animName]->_frames[bone._boneName]._rotateTime);
		startRot = _animations[animName]->_frames[bone._boneName]._rotation[index - 1];
		endRot = _animations[animName]->_frames[bone._boneName]._rotation[index];
		newRotation = glm::slerp(startRot, endRot, timeScale);
		rotation = true;
	}
	else if (_animations[animName]->_frames[bone._boneName]._rotateTime.size() == 1)
	{
		newRotation = _animations[animName]->_frames[bone._boneName]._rotation[0];
	}

	index = 0;
	timeScale = 0.0f;

	glm::vec3 newScale;
	glm::vec3 startScale;
	glm::vec3 endScale;
	bool scale = false;
	if (_animations[animName]->_frames[bone._boneName]._scaleTime.size() > 1)
	{
		InterpTime(index, timeScale, dt, _animations[animName]->_frames[bone._boneName]._scaleTime);
		startScale = _animations[animName]->_frames[bone._boneName]._scale[index - 1];
		endScale = _animations[animName]->_frames[bone._boneName]._scale[index];
		newScale = glm::mix(startScale, endScale, timeScale);
		scale = true;
	}
	else if (_animations[animName]->_frames[bone._boneName]._scaleTime.size() == 1)
	{
		newScale = _animations[animName]->_frames[bone._boneName]._scale[0];
	}

	glm::mat4 positionMat = glm::mat4(1.0);
	glm::mat4 scaleMat = glm::mat4(1.0);

	if (translation)
	{
		positionMat = glm::translate(positionMat, newPosition);
	}

	if (scale)
	{
		scaleMat = glm::scale(scaleMat, newScale);
	}

	glm::mat4 rotationMat(1.0f);
	if (rotation)
	{
		rotationMat = glm::toMat4(newRotation);
	}
	
	if (!translation)
	{
		localTransform = bone._boneTransform;
	}
	else
	{
		localTransform = positionMat * rotationMat * scaleMat;
	}

	globalTransform = parentTransform * localTransform;
	_poseTransform[bone._boneID] = globalInverseTransform * globalTransform * bone._boneTransformOffset;

	//update values for children bones
	for (BoneData& child : bone._childrenBones)
	{
		GetPose(animName, child, dt, globalTransform, globalInverseTransform);
	}
}

void NS_GRAPHICS::Model::InterpTime(unsigned& index, float& time, double& dt, std::vector<double>& times)
{
	//Assumes dt will always be within the duration of animation
	unsigned currIndex = 2;
	if (dt >= times[times.size() - 1])
	{
		time = 1.0f;
		index = times.size() - 1;
		return;
	}
	while (times[currIndex] <= dt)
	{
		++currIndex;
	}

	//Get the time of the next frame and the current frame
	float startTime = times[currIndex - 1];
	float endTime = times[currIndex];

	//Get the scale value of the time between 0.0f to 1.0f between start and end
	time = ((float)dt - startTime) / (endTime - startTime);
	index = currIndex;
}
