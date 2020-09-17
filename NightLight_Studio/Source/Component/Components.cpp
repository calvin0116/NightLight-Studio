#include "Components.h"

COMPONENT_CONTAINERS G_MAINCOMPSET_CCIDS;


ComponentTransform::ComponentTransform() : _position{ 0.0f, 0.0f, 0.0f },
_rotation{ 0.0f, 0.0f, 0.0f }, _scale{ 1.0f, 1.0f, 1.0f }
{

}

ComponentTransform::~ComponentTransform()
{

}