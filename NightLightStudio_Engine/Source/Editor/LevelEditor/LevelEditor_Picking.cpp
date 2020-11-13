#include "LevelEditor_Picking.h"

#include "../../Collision/CollisionMathLib.h"

#include "LevelEditor_ECHelper.h"

#include "../../Graphics/GraphicsSystem.h"
#include "../../Input/SystemInput.h"

#include "LevelEditor_Inspector.h"

bool UnProject(float winX, float winY, float winZ,
    const glm::mat4& invMat, glm::vec3& worldCoordinates);

LE_Picking::LE_Picking() : _hitEntityNum {0}
{
}

LE_Picking::~LE_Picking()
{
}

void LE_Picking::LE_PickingRun()
{
    if (!ImGui::IsAnyItemHovered()
        && !ImGuizmo::IsOver()
        && SYS_INPUT->GetSystemKeyPress().GetKeyPress(SystemInput_ns::IMOUSE_LBUTTON))
    {
        //NS_GRAPHICS::CameraSystem& camSys = NS_GRAPHICS::CameraSystem::GetInstance();
        glm::vec2 pos = SYS_INPUT->GetSystemMousePos().GetRelativeLocation();

        glm::vec3 startPos, endPos;
        glm::mat4 invMatr = NS_GRAPHICS::SYS_GRAPHICS->GetInverseViewMatrix();

        UnProject(pos.x, pos.y, /* z = */ 0.0, invMatr, startPos);
        UnProject(pos.x, pos.y, /* z = */ 1.0, invMatr, endPos);

        NlMath::Point3D startRay = { startPos.x, startPos.y, startPos.z }, endRay = { endPos.x, endPos.y, endPos.z };

        std::vector<int> hitList;

        for (Entity ent : G_ECMANAGER->getEntityContainer())
        {
            AABBCollider aabb;
            if (ent.getComponent<ComponentCollider>() && ent.getComponent<ComponentCollider>()->GetColliderT() == COLLIDERS::AABB)
            {
                aabb = ent.getComponent<ComponentCollider>()->collider.aabb;
            }
            else
                aabb.posUpdate(ent.getComponent<ComponentTransform>());

            if (NlMath::RayToAABB(aabb, startRay, endRay))
            {
                hitList.push_back(ent.getId());
            }
        }


        std::sort(std::begin(hitList), std::end(hitList),
            [&startPos](int ent1, int ent2)
            { 
                if (glm::dot(startPos, G_ECMANAGER->getEntity(ent1).getComponent<TransformComponent>()->_position) 
                    < glm::dot(startPos, G_ECMANAGER->getEntity(ent2).getComponent<TransformComponent>()->_position))
                    return true; 
                return false;
            });


        if (_hitEntities.size() && _hitEntities == hitList)
        {
            _hitEntityNum++;
            if (_hitEntityNum >= _hitEntities.size())
                _hitEntityNum = 0;
            LE_ECHELPER->SelectEntity(_hitEntities[_hitEntityNum]);
        }
        else if (hitList.size())
        {
            _hitEntityNum = 0;
            _hitEntities = hitList;
            LE_ECHELPER->SelectEntity(_hitEntities[_hitEntityNum]);
        }
    }
}

bool UnProject(float winX, float winY, float winZ,
    const glm::mat4& invMat,
    glm::vec3& worldCoordinates)
{
    // Transformation of normalized coordinates between -1 and 1:
    glm::vec4 in;
    in[0] = winX * 2.0f - 1.0f;
    in[1] = winY * 2.0f - 1.0f;
    in[2] = 2.0f * winZ - 1.0f;
    in[3] = 1.0f;

    // To world coordinates:
    glm::vec4 out(invMat * in);
    if (out[3] == 0.0) // Avoid a division by zero
    {
        worldCoordinates = glm::vec3();
        return false;
    }

    out[3] = 1.0f / out[3];
    worldCoordinates[0] = (float)out[0] * out[3];
    worldCoordinates[1] = (float)out[1] * out[3];
    worldCoordinates[2] = (float)out[2] * out[3];
    return true;
}
