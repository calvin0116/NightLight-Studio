#include <functional>
#include <queue>

#include "CollisionMathLib.h"


namespace NlMath
{
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

    bool PlaneToPlane(const PlaneCollider& tPlane1, const PlaneCollider& tPlane2)
    {
        Matrix4x4 rotationalMtx;

        //setting up normals for plane1
        Vector3D normalX1(1, 0, 0);
        Vector3D normalY1(0, 1, 0);
        Vector3D normalZ1(0, 0, 1);
        Mtx44RotRad(rotationalMtx, tPlane1.rotation);
        //rotate normals to correct position
        normalX1 = rotationalMtx * normalX1;
        normalY1 = rotationalMtx * normalY1;
        normalZ1 = rotationalMtx * normalZ1;

        //setting up normals for plane2
        Vector3D normalX2(1, 0, 0);
        Vector3D normalY2(0, 1, 0);
        Vector3D normalZ2(0, 0, 1);
        Mtx44RotRad(rotationalMtx, tPlane2.rotation);
        //rotate normals to correct position
        normalX2 = rotationalMtx * normalX2;
        normalY2 = rotationalMtx * normalY2;
        normalZ2 = rotationalMtx * normalZ2;

        Vector3D nearPoint;
        Vector3D farPoint;
        Vector3D maxDistance;
        Vector3D maxDistanceToCtr;
        float farLength = -FLT_MAX;
        float nearLength = FLT_MAX;

        float tmpLength = 0;

        maxDistance = tPlane1.center + normalX1 * tPlane1.extend.x + normalZ1 * tPlane1.extend.z;
        maxDistanceToCtr = tPlane1.center - maxDistance;



        return false;
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

    SIDES AABB_AABBCollision(const AABBCollider& tBox1, const AABBCollider& tBox2)
    {
        //axis view explaination: (value going from negative to positive)
        //x going from left to right
        //y going from bottom to top
        //z going from back(into the screen) to front(out of the screen)
        

        //get the center between two box
        NlMath::Vector3D center1 = NlMath::Vector3DMidPoint(tBox1.vecMax, tBox1.vecMin);
        NlMath::Vector3D center2 = NlMath::Vector3DMidPoint(tBox2.vecMax, tBox2.vecMin);

        //get distance vector between two box's center
        NlMath::Vector3D centerDistance = center2 - center1;

        //get absolute distance vector between two box's center
        NlMath::Vector3D absCtrDistance = 0;
        absCtrDistance.x = fabsf(centerDistance.x);
        absCtrDistance.y = fabsf(centerDistance.y);
        absCtrDistance.z = fabsf(centerDistance.z);

        //get scaling factor
        NlMath::Vector3D extend1 = tBox1.vecMax - center1;
        NlMath::Vector3D extend2 = tBox2.vecMax - center2;

        //calculate total extend
        NlMath::Vector3D totalExtend = extend1 + extend2;

        //total differnce
        NlMath::Vector3D diffVec = totalExtend - absCtrDistance;


        if (diffVec.x <= 0 || diffVec.y <= 0 || diffVec.z <= 0)
        {
            return SIDES::NO_COLLISION;
        }


        if (diffVec.x <= diffVec.y && diffVec.x <= diffVec.z)
        {
            //if x is positive and shortest among all axis, the collision must be happening at LEFT
            if (centerDistance.x > 0)
            {
                return SIDES::RIGHT;
            }
            //if x is negative and shortest among all axis, the collision must be happening at RIGHT
            else
            {
                return SIDES::LEFT;
            }
        }
        else if (diffVec.y <= diffVec.x && diffVec.y <= diffVec.z)
        {
            //if y is positive and shortest among all axis, the collision must be happening at FRONT
            if (centerDistance.y > 0)
            {
                return SIDES::TOP;
            }
            //if y is negative and shortest among all axis, the collision must be happening at RIGHT
            else
            {
                return SIDES::BOTTOM;
            }
        }
        else if (diffVec.z <= diffVec.y && diffVec.z <= diffVec.x)
        {
            //if z is positive and shortest among all axis, the collision must be happening at TOP
            if (centerDistance.z > 0)
            {
                return SIDES::FRONT;
            }
            //if z is negative and shortest among all axis, the collision must be happening at BOTTOM
            else
            {
                return SIDES::BACK;
            }
        }
        else
        {
            //shouldnt reach here!
            throw;
        }
    }

    NlMath::Vector3D Sphere_SphereCollision(const SphereCollider& tSpr1, const SphereCollider& tSpr2)
    {
        NlMath::Vector3D distance = tSpr1.center - tSpr2.center;
        float radiusExtend = tSpr1.radius + tSpr2.radius;

        if (distance.length() < radiusExtend)
        {
            return distance;
        }
        //no collision
        return NlMath::Vector3D(0);
    }

    SIDES AABB_SphereCollision(const AABBCollider& tBox1, const SphereCollider& tSpr2, NlMath::Vector3D& circleColNormal)
    {
        //axis view explaination: (value going from negative to positive)
        //x going from left to right
        //y going from bottom to top
        //z going from back(into the screen) to front(out of the screen)

        //get the center between two box
        NlMath::Vector3D center1 = NlMath::Vector3DMidPoint(tBox1.vecMax, tBox1.vecMin);
        NlMath::Vector3D center2 = tSpr2.center;

        //get distance vector between two box's center
        NlMath::Vector3D centerDistance = center2 - center1;

        //get absolute distance vector between two box's center
        NlMath::Vector3D absCtrDistance = 0;
        absCtrDistance.x = fabsf(centerDistance.x);
        absCtrDistance.y = fabsf(centerDistance.y);
        absCtrDistance.z = fabsf(centerDistance.z);

        //get scaling factor
        NlMath::Vector3D extend1 = tBox1.vecMax - center1;
        NlMath::Vector3D extend2 = NlMath::Vector3D(tSpr2.radius);

        //calculate total extend
        NlMath::Vector3D totalExtend = extend1 + extend2;

        //total difference
        NlMath::Vector3D diffVec = totalExtend - absCtrDistance;

        if (diffVec.x <= 0 || diffVec.y <= 0 || diffVec.z <= 0)
        {
            return SIDES::NO_COLLISION;
        }

        //return collision vector for circle
        circleColNormal = -centerDistance;


        if (diffVec.x <= diffVec.y && diffVec.x <= diffVec.z)
        {
            //if x is positive and shortest among all axis, the collision must be happening at LEFT
            if (centerDistance.x > 0)
            {
                return SIDES::RIGHT;
            }
            //if x is negative and shortest among all axis, the collision must be happening at RIGHT
            else
            {
                return SIDES::LEFT;
            }
        }
        else if (diffVec.y <= diffVec.x && diffVec.y <= diffVec.z)
        {
            //if y is positive and shortest among all axis, the collision must be happening at FRONT
            if (centerDistance.y > 0)
            {
                return SIDES::TOP;
            }
            //if y is negative and shortest among all axis, the collision must be happening at RIGHT
            else
            {
                return SIDES::BOTTOM;
            }
        }
        else if (diffVec.z <= diffVec.y && diffVec.z <= diffVec.x)
        {
            //if z is positive and shortest among all axis, the collision must be happening at TOP
            if (centerDistance.z > 0)
            {
                return SIDES::FRONT;
            }
            //if z is negative and shortest among all axis, the collision must be happening at BOTTOM
            else
            {
                return SIDES::BACK;
            }
        }
        else
        {
            //shouldnt reach here!
            throw;
        }
    }

    bool OBBToOBB(const OBBCollider& tBox1, const OBBCollider& tBox2)
    {
        //axis view explaination: (value going from negative to positive)
        //x going from left to right
        //y going from back(into the screen) to front(out of the screen)
        //z going from bottom to top

        Matrix4x4 rotationalMtx;

        //setting up normals for Box1
        Vector3D normalX1(1, 0, 0);
        Vector3D normalY1(0, 1, 0);
        Vector3D normalZ1(0, 0, 1);
        Mtx44RotRad(rotationalMtx, tBox1.rotation);
        //rotate normals to correct position
        normalX1 = rotationalMtx * normalX1;
        normalY1 = rotationalMtx * normalY1;
        normalZ1 = rotationalMtx * normalZ1;

        //setting up normals for Box2
        Vector3D normalX2(1, 0, 0);
        Vector3D normalY2(0, 1, 0);
        Vector3D normalZ2(0, 0, 1);
        Mtx44RotRad(rotationalMtx, tBox2.rotation);
        //rotate normals to correct position
        normalX2 = rotationalMtx * normalX2;
        normalY2 = rotationalMtx * normalY2;
        normalZ2 = rotationalMtx * normalZ2;

        //get distance vector between two box's center
        Vector3D centerDistance = tBox2.center - tBox1.center;

        // check if there's a separating plane in between the selected axes
        auto getSeparatingPlane = [=](const Vector3D& Plane)
        {
            return (fabs(centerDistance * Plane) >
                (fabs((normalX1 * tBox1.extend.x) * Plane) +
                    fabs((normalY1 * tBox1.extend.y) * Plane) +
                    fabs((normalZ1 * tBox1.extend.z) * Plane) +
                    fabs((normalX2 * tBox2.extend.x) * Plane) +
                    fabs((normalY2 * tBox2.extend.y) * Plane) +
                    fabs((normalZ2 * tBox2.extend.z) * Plane)));
        };

        return !(getSeparatingPlane(normalX1) ||
            getSeparatingPlane(normalY1) ||
            getSeparatingPlane(normalZ1) ||
            getSeparatingPlane(normalX2) ||
            getSeparatingPlane(normalY2) ||
            getSeparatingPlane(normalZ2) ||
            
            getSeparatingPlane(Vector3DCrossProduct(normalX1, normalX2)) ||
            getSeparatingPlane(Vector3DCrossProduct(normalX1, normalY2)) ||
            getSeparatingPlane(Vector3DCrossProduct(normalX1, normalZ2)) ||

            getSeparatingPlane(Vector3DCrossProduct(normalY1,normalX2)) ||
            getSeparatingPlane(Vector3DCrossProduct(normalY1,normalY2)) ||
            getSeparatingPlane(Vector3DCrossProduct(normalY1,normalZ2)) ||

            getSeparatingPlane(Vector3DCrossProduct(normalZ1,normalX2)) ||
            getSeparatingPlane(Vector3DCrossProduct(normalZ1,normalY2)) ||
            getSeparatingPlane(Vector3DCrossProduct(normalZ1,normalZ2)));

    }
}