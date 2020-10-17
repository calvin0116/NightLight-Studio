#include "ComponentTransform.h"
#include "../../glm/gtc/matrix_transform.hpp"
#include "../../glm/gtc/quaternion.hpp"

ComponentTransform::ComponentTransform() : _position{ 0.0f, 0.0f, 0.0f },
_rotation{ 0.0f, 0.0f, 0.0f }, _scale{ 1.0f, 1.0f, 1.0f }
{

}

ComponentTransform::~ComponentTransform()
{

}

glm::mat4 ComponentTransform::GetModelMatrix()
{
	glm::mat4 Translate = glm::translate(glm::mat4(), _position);
	
	// Perform rotation using Quarternions
	glm::quat Quaternion(glm::radians(_rotation));
	glm::mat4 Rotate = glm::mat4_cast(Quaternion);

	glm::mat4 Scale = glm::scale(glm::mat4(), _scale);


	return Translate * Rotate * Scale;
	return glm::mat4();
}

void ComponentTransform::Read()
{
}

void ComponentTransform::Write()
{
}
