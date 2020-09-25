#pragma once
//outside lib
#include <vector>

//engine lib
#include "..\\Component\ComponentTransform.h"
#include "..\\Component\ComponentPhysics.h"
//#include "..\\Component\ComponentRegidBody.h"
#include "..\\Math\Vector.h"


struct AABB
{
	NlMath::Vector3D VecMax;
	NlMath::Vector3D VecMin;
	AABB();
	AABB(NlMath::Vector3D max, NlMath::Vector3D min);
};

bool PointInAABB(const AABB& tBox, const NlMath::Vector3D& vecPoint)
{
    //Check if the point is less than max and greater than min
    if (vecPoint.x > tBox.VecMin.x && vecPoint.x < tBox.VecMax.x &&
        vecPoint.y > tBox.VecMin.y && vecPoint.y < tBox.VecMax.y &&
        vecPoint.z > tBox.VecMin.z && vecPoint.z < tBox.VecMax.z)
    {
        return true;
    }

    //If not, then return false
    return false;

}

bool AABBtoAABB(const AABB& tBox1, const AABB& tBox2)
{

    //Check if Box1's max is greater than Box2's min and Box1's min is less than Box2's max
    return(
        tBox1.VecMax.x > tBox2.VecMin.x &&
        tBox1.VecMin.x < tBox2.VecMax.x &&
        tBox1.VecMax.y > tBox2.VecMin.y &&
        tBox1.VecMin.y < tBox2.VecMax.y &&
        tBox1.VecMax.z > tBox2.VecMin.z &&
        tBox1.VecMin.z < tBox2.VecMax.z);

    //If not, it will return false

}