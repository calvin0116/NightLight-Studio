#include "LevelEditor_Picking.h"

#include "../../Collision/CollisionMathLib.h"

#include "LevelEditor_ECHelper.h"

#include "../../Graphics/GraphicsSystem.h"
#include "../../Input/SystemInput.h"

#include "LevelEditor_Inspector.h"

bool UnProject(float winX, float winY, float winZ,
    const glm::mat4& invMat, glm::vec3& worldCoordinates);

LE_Picking::LE_Picking()
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

        _hitEntities.clear();

        for (Entity ent : G_ECMANAGER->getEntityContainer())
        {
            AABBCollider aabb;
            aabb.posUpdate(ent.getComponent<ComponentTransform>());
            if (NlMath::RayToAABB(aabb, startRay, endRay))
            {
                if (LE_ECHELPER->GetSelectedEntityID() != ent.getId())
                {
                    _hitEntities.push_back(ent);
                    //LE_ECHELPER->SelectEntity(ent.getId());
                    //break;
                }
            }
        }

        std::sort(std::begin(_hitEntities), std::end(_hitEntities), 
            [&startPos](Entity ent1, Entity ent2)
            { 
                if (glm::dot(startPos, ent1.getComponent<TransformComponent>()->_position) < glm::dot(startPos, ent2.getComponent<TransformComponent>()->_position))
                    return true; 
                return false;
            });

        if (_hitEntities.size())
        {
            LE_ECHELPER->SelectEntity(_hitEntities[0].getId());
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
