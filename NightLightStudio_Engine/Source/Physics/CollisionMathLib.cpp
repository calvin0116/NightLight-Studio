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

    //get distance vector between two box's center
    NlMath::Vector3D diffVec = center2 - center1;

    //lamda for sorting
    auto cmp = [](std::pair<float, char> left, std::pair<float, char> right) { return (left.first) < (right.first); };
    std::priority_queue<std::pair<float, char>, std::vector<std::pair<float, char>>, decltype(cmp)> magnitudeQueue(cmp);

    //sort the maginutde of all axis
    magnitudeQueue.push(std::pair<float, char>(fabs(diffVec.x), 'x'));
    magnitudeQueue.push(std::pair<float, char>(fabs(diffVec.y), 'y'));
    magnitudeQueue.push(std::pair<float, char>(fabs(diffVec.z), 'z'));

    //use the longest axis to check
    std::pair<float, char> check = magnitudeQueue.top();
    if (check.second == 'x')
    {
        //if x is positive and longest among all axis, the collision must be happening at LEFT
        if (diffVec.x > 0)
        {
            return SIDES::LEFT;
        }
        //if x is negative and longest among all axis, the collision must be happening at RIGHT
        else
        {
            return SIDES::RIGHT;
        }
    }
    else if (check.second == 'y')
    {
        //if y is positive and longest among all axis, the collision must be happening at FRONT
        if (diffVec.y > 0)
        {
            return SIDES::FRONT;
        }
        //if y is negative and longest among all axis, the collision must be happening at RIGHT
        else
        {
            return SIDES::BACK;
        }
    }
    else if (check.second == 'z')
    {
        //if z is positive and longest among all axis, the collision must be happening at TOP
        if (diffVec.z > 0)
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
