#include <functional>
#include <queue>

#include "CollisionMathLib.h"

bool PointInAABB(const AABBCollider& tBox, const NlMath::Vector3D& vecPoint)
{
    //Check if the point is less than max and greater than min
    if (vecPoint.x > tBox.vecMin.x && vecPoint.x < tBox.vecMax.x &&
        vecPoint.y > tBox.vecMin.y && vecPoint.y < tBox.vecMax.y &&
        vecPoint.z > tBox.vecMin.z && vecPoint.z < tBox.vecMax.z)
    {
        return true;
    }

    //If not, then return false
    return false;

}

bool AABBtoAABB(const AABBCollider& tBox1, const AABBCollider& tBox2)
{

    //Check if Box1's max is greater than Box2's min and Box1's min is less than Box2's max
    return(
        tBox1.vecMax.x > tBox2.vecMin.x &&
        tBox1.vecMin.x < tBox2.vecMax.x&&
        tBox1.vecMax.y > tBox2.vecMin.y &&
        tBox1.vecMin.y < tBox2.vecMax.y&&
        tBox1.vecMax.z > tBox2.vecMin.z &&
        tBox1.vecMin.z < tBox2.vecMax.z);

    //If not, it will return false

}

std::vector<NlMath::Vector3D> AllPointsOf(const AABBCollider& tBox)
{
    std::vector<NlMath::Vector3D> points;
    points.push_back(NlMath::Vector3D(tBox.vecMax.x, tBox.vecMax.y, tBox.vecMax.z));
    points.push_back(NlMath::Vector3D(tBox.vecMax.x, tBox.vecMin.y, tBox.vecMax.z));
    points.push_back(NlMath::Vector3D(tBox.vecMax.x, tBox.vecMax.y, tBox.vecMin.z));
    points.push_back(NlMath::Vector3D(tBox.vecMax.x, tBox.vecMin.y, tBox.vecMin.z));
    points.push_back(NlMath::Vector3D(tBox.vecMin.x, tBox.vecMax.y, tBox.vecMax.z));
    points.push_back(NlMath::Vector3D(tBox.vecMin.x, tBox.vecMin.y, tBox.vecMax.z));
    points.push_back(NlMath::Vector3D(tBox.vecMin.x, tBox.vecMax.y, tBox.vecMin.z));
    points.push_back(NlMath::Vector3D(tBox.vecMin.x, tBox.vecMin.y, tBox.vecMin.z));

    return points;
}

SIDES AABBCheckCollisionSides(const AABBCollider& tBox1, const AABBCollider& tBox2)
{
    //axis view explaination: (value going from negative to positive)
    //x going from left to right
    //y going from back(into the screen) to front(out of the screen)
    //z going from bottom to top


    //get the center between two box
    NlMath::Vector3D center1 = NlMath::Vector3DMidPoint(tBox1.vecMax, tBox1.vecMin);
    NlMath::Vector3D center2 = NlMath::Vector3DMidPoint(tBox2.vecMax, tBox2.vecMin);

    //get absolute distance vector between two box's center
    NlMath::Vector3D centerDistance = center2 - center1;
    centerDistance.x = fabsf(centerDistance.x);
    centerDistance.y = fabsf(centerDistance.y);
    centerDistance.z = fabsf(centerDistance.z);
    
    //get scaling factor
    NlMath::Vector3D extend1 = tBox1.vecMax - center1;
    NlMath::Vector3D extend2 = tBox2.vecMax - center2;

    //calculate total extend
    NlMath::Vector3D totalExtend = extend1 + extend2;

    //total 
    NlMath::Vector3D diffVec = totalExtend - centerDistance;


    if (diffVec.x <= 0 || diffVec.y <= 0 || diffVec.z <= 0)
    {
        return SIDES::NO_COLLISION;
    }


    if (diffVec.x <= diffVec.y && diffVec.x <= diffVec.z)
    {
        //if x is positive and longest among all axis, the collision must be happening at LEFT
        if (centerDistance.x > 0)
        {
            return SIDES::RIGHT;
        }
        //if x is negative and longest among all axis, the collision must be happening at RIGHT
        else
        {
            return SIDES::LEFT;
        }
    }
    else if (diffVec.y <= diffVec.x && diffVec.y <= diffVec.z)
    {
        //if y is positive and longest among all axis, the collision must be happening at FRONT
        if (centerDistance.y > 0)
        {
            return SIDES::FRONT;
        }
        //if y is negative and longest among all axis, the collision must be happening at RIGHT
        else
        {
            return SIDES::BACK;
        }
    }
    else if (diffVec.z <= diffVec.y && diffVec.z <= diffVec.x)
    {
        //if z is positive and longest among all axis, the collision must be happening at TOP
        if (centerDistance.z > 0)
        {
            return SIDES::TOP;
        }
        //if z is negative and longest among all axis, the collision must be happening at BOTTOM
        else
        {
            return SIDES::BOTTOM;
        }
    }
    else
    {
        //shouldnt reach here!
        throw;
    }
}
