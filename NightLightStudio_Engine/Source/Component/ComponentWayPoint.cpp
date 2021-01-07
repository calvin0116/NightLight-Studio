#include "ComponentWayPoint.h"

glm::vec3 ComponentWayPoint::GetPos()
{
	return G_ECMANAGER->getEntity(this).getComponent<TransformComponent>()->_position;
}
