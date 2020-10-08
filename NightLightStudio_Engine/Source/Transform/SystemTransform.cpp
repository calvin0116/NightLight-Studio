#include "SystemTransform.h"

bool SystemTransform::Update()
{
	auto itr = G_MAINCOMPSET->begin<ComponentTransform>();
	auto itrEnd = G_MAINCOMPSET->end<ComponentTransform>();
	while (itr != itrEnd)
	{
		ComponentTransform* compT = G_MAINCOMPSET->getComponent<ComponentTransform>(itr);

		glm::mat4x4 translateMat(1), rotateMat(1), scaleMat(1);

		glm::scale(scaleMat, compT->_scale);

		glm::rotate(rotateMat, compT->_rotation.x, { 1.0f, 0.0f, 0.0f });
		glm::rotate(rotateMat, compT->_rotation.y, { 0.0f, 1.0f, 0.0f });
		glm::rotate(rotateMat, compT->_rotation.z, { 0.0f, 0.0f, 1.0f });

		glm::translate(translateMat, compT->_position);

		compT->_model = translateMat * rotateMat * scaleMat;

		++itr;
	}

	return true;
}
