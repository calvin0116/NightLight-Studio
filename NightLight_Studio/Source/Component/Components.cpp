#include "Components.h"

COMPONENTS G_MAINCOMPSET;
COMPONENTS G_UICOMPSET;


ComponentTransform::ComponentTransform() : _position{ 0.0f, 0.0f, 0.0f },
_rotation{ 0.0f, 0.0f, 0.0f }, _scale{ 1.0f, 1.0f, 1.0f }
{

}

ComponentTransform::~ComponentTransform()
{

}