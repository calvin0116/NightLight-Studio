#include "ComponentTransform.h"
#include "../../glm/gtc/matrix_transform.hpp"
#include "../../glm/gtc/quaternion.hpp"

#include "Components.h"

ComponentTransform::ComponentTransform() : 
	_nextPos(0),_position{ 0.0f, 0.0f, 0.0f },
_rotation{ 0.0f, 0.0f, 0.0f }, _scale{ 1.0f, 1.0f, 1.0f }
{

}

ComponentTransform::~ComponentTransform()
{

}

glm::mat4 ComponentTransform::GetModelMatrix()
{
	glm::mat4 Translate = glm::translate(glm::mat4(1.f), _position);
	
	// Perform rotation using Quarternions
	glm::quat Quaternion(glm::radians(_rotation));
	glm::mat4 Rotate = glm::mat4_cast(Quaternion);

	glm::mat4 Scale = glm::scale(glm::mat4(1.f), _scale);

	return (Translate * Rotate * Scale);
}

void ComponentTransform::Read(Value& val)
{
	auto pos = val["Position"].GetArray();

	_position.x = pos[0].GetFloat();
	_position.y = pos[1].GetFloat();
	_position.z = pos[2].GetFloat();
}

Value ComponentTransform::Write()
{
	return Value();
}
