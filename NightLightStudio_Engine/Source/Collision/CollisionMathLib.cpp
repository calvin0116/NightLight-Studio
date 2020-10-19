#include <functional>
#include <queue>

#include "CollisionMathLib.h"


namespace NlMath
{
    Vector3D ClosestPointOnLineSegment(Vector3D segmentPointA, Vector3D segmentPointB, Vector3D CheckPoint)
    {
        //construct linesegment AB
        Vector3D AB = segmentPointB - segmentPointA;
        //find projection
        float t = Vector3DDotProduct(CheckPoint - segmentPointA, AB) / Vector3DDotProduct(AB, AB);
        return segmentPointA + t * AB;
    }

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

        //find penetration depth
        NlMath::Vector3D penetrationDepth = totalExtend - absCtrDistance;


        if (penetrationDepth.x <= 0 || penetrationDepth.y <= 0 || penetrationDepth.z <= 0)
        {
            return SIDES::NO_COLLISION;
        }


        if (penetrationDepth.x <= penetrationDepth.y && penetrationDepth.x <= penetrationDepth.z)
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
        else if (penetrationDepth.y <= penetrationDepth.x && penetrationDepth.y <= penetrationDepth.z)
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
        else if (penetrationDepth.z <= penetrationDepth.y && penetrationDepth.z <= penetrationDepth.x)
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

        //total penetration depth
        NlMath::Vector3D penetrationDepth = totalExtend - absCtrDistance;

        if (penetrationDepth.x <= 0 || penetrationDepth.y <= 0 || penetrationDepth.z <= 0)
        {
            return SIDES::NO_COLLISION;
        }

        //return collision vector for circle
        circleColNormal = -centerDistance;


        if (penetrationDepth.x <= penetrationDepth.y && penetrationDepth.x <= penetrationDepth.z)
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
        else if (penetrationDepth.y <= penetrationDepth.x && penetrationDepth.y <= penetrationDepth.z)
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
        else if (penetrationDepth.z <= penetrationDepth.y && penetrationDepth.z <= penetrationDepth.x)
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

        //float rayD;
        //Matrix4x4 absRayM, rayM;

        //Vector3D tmpD = tBox1.center - tBox2.center;


        //Vector3D trans{ tmpD * normalX1,tmpD * normalY1 ,tmpD * normalZ1 };

        //std::vector<Vector3D> faceNormals1 = { normalX1,normalY1,normalZ1 };
        //std::vector<Vector3D> faceNormals2 = { normalX2,normalY2,normalZ2 };

        //for (int i = 0; i < 3; i++)
        //{
        //    for (int j = 0; j < 3; j++)
        //    {
        //        rayM[i, j] = faceNormals1[i] * faceNormals2[j];
        //        absRayM[i, j] = std::fabsf(rayM[i, j]);
        //    }
        //}

        ///// Test axes lhs side normal
        //rayD = tBox1.extend.x + tBox2.extend.x * absRayM[0, 0] + tBox2.extend.y * absRayM[0, 1] + tBox2.extend.z * absRayM[0,  2];
        //if (std::fabsf(trans.x) > rayD) return false;


        ///// Test axes lhs up normal
        //rayD = tBox1.extend.y + tBox2.extend.x * absRayM[1, 0] + tBox2.extend.y * absRayM[1, 1] + tBox2.extend.z * absRayM[1, 2];
        //if (std::fabsf(trans.y) > rayD) return false;

        ///// Test axes lhs forward normal
        //rayD = tBox1.extend.z + tBox2.extend.x * absRayM[2, 0] + tBox2.extend.y * absRayM[2, 1] + tBox2.extend.z * absRayM[2, 2];
        //if (std::fabsf(trans.z) > rayD) return false;


        ///// Test axes rhs side normal
        //rayD = tBox2.extend.x + tBox1.extend.x * absRayM[0, 0] + tBox1.extend.y * absRayM[1, 0] + tBox1.extend.z * absRayM[2, 0];
        //if (std::fabsf(trans.x * rayM[0, 0] + trans.y * rayM[1, 0] + trans.z * rayM[2, 0]) > rayD) return false;

        ///// Test axes rhs up normal
        //rayD = tBox2.extend.y + tBox1.extend.x * absRayM[0, 1] + tBox1.extend.y * absRayM[1, 1] + tBox1.extend.z * absRayM[2, 1];
        //if (std::fabsf(trans.x * rayM[0, 1] + trans.y * rayM[1, 1] + trans.z * rayM[2, 1]) > rayD) return false;


        ///// Test axes rhs forward normal
        //rayD = tBox2.extend.z + tBox1.extend.x * absRayM[0,  2] + tBox1.extend.y * absRayM[1, 2] + tBox1.extend.z * absRayM[2, 2];
        //if (std::fabsf(trans.x * rayM[0, 2] + trans.y * rayM[1, 2] + trans.z * rayM[2, 2]) > rayD) return false;



        ///// Test axes lhs side normal cross rhs side normal
        //rayD = tBox1.extend.y * absRayM[2, 0] + tBox1.extend.z * absRayM[1, 0];
        //rayD += tBox2.extend.y * absRayM[0,  2] + tBox2.extend.z * absRayM[0, 1];
        //if (std::fabsf(trans[2] * rayM[1, 0] - trans[1] * rayM[2, 0]) > rayD) return false;


        ///// Test axes lhs side normal cross rhs up normal
        //rayD = tBox1.extend.y * absRayM[2, 1] + tBox1.extend.z * absRayM[1, 1];
        //rayD += tBox2.extend.x * absRayM[0,  2] + tBox2.extend.z * absRayM[0, 0];
        //if (std::fabsf(trans[2] * rayM[1, 1] - trans[1] * rayM[2, 1]) > rayD) return false;

        ///// Test axes lhs side normal cross rhs forward normal
        //rayD = tBox1.extend.y * absRayM[2, 2] + tBox1.extend.z * absRayM[1, 2];
        //rayD += tBox2.extend.x * absRayM[0, 1] + tBox2.extend.y * absRayM[0, 0];
        //if (std::fabsf(trans[2] * rayM[1, 2] - trans[1] * rayM[2, 2]) > rayD) return false;





        ///// Test axes lhs up normal cross rhs side normal
        //rayD = tBox1.extend.x * absRayM[2, 0] + tBox1.extend.z * absRayM[0, 0];
        //rayD += tBox2.extend.y * absRayM[1, 2] + tBox2.extend.z * absRayM[1, 1];
        //if (std::fabsf(trans[2] * rayM[2, 1] - trans[2] * rayM[0, 1]) > rayD) return false;

        ///// Test axes lhs up normal cross rhs up normal
        //rayD = tBox1.extend.x * absRayM[2, 1] + tBox1.extend.z * absRayM[0, 1];
        //rayD += tBox2.extend.x * absRayM[1, 2] + tBox2.extend.z * absRayM[1, 0];
        //if (std::fabsf(trans[0] * rayM[2, 1] - trans[2] * rayM[0, 1]) > rayD) return false;

        ///// Test axes lhs up normal cross rhs forward normal
        //rayD = tBox1.extend.x * absRayM[2, 2] + tBox1.extend.z * absRayM[0,  2];
        //rayD += tBox2.extend.x * absRayM[1, 1] + tBox2.extend.y * absRayM[1, 0];
        //if (std::fabsf(trans[0] * rayM[2, 2] - trans[2] * rayM[0, 2]) > rayD) return false;



        ///// Test axes lhs forward normal cross rhs side normal
        //rayD = tBox1.extend.x * absRayM[1, 0] + tBox1.extend.y * absRayM[0, 0];
        //rayD += tBox2.extend.y * absRayM[2, 2] + tBox2.extend.z * absRayM[2, 1];
        //if (std::fabsf(trans[1] * rayM[0, 0] - trans[0] * rayM[1, 0]) > rayD) return false;

        ///// Test axes lhs forward normal cross rhs up normal
        //rayD = tBox1.extend.x * absRayM[1, 1] + tBox1.extend.y * absRayM[0, 1];
        //rayD += tBox2.extend.x * absRayM[2, 2] + tBox2.extend.z * absRayM[2, 0];
        //if (std::fabsf(trans[1] * rayM[0, 1] - trans[0] * rayM[1, 1]) > rayD) return false;

        ///// Test axes lhs forward normal cross rhs forward normal

        //rayD = tBox1.extend.x * absRayM[1, 2] + tBox1.extend.y * absRayM[0,  2];
        //rayD += tBox2.extend.y * absRayM[2, 1] + tBox2.extend.y * absRayM[2, 0];
        //if (std::fabsf(trans[1] * rayM[0, 2] - trans[0] * rayM[1, 2]) > rayD) return false;

        //return true;

    }
    bool CapsuleToCapsule(const CapsuleCollider& tCap1, const CapsuleCollider& tCap2)
    {
        //Capsule collision is made up of 2 circles, one at tip and the other at base
        
        //start by finding the normal of the capsule 1, which is the tip - base
        Vector3D normal1 = Vector3DNormalize(tCap1.tip - tCap1.base);
        //find the vector to reach the center of circle
        Vector3D lineOffSet1 = normal1 * tCap1.radius;
        //find the center of tip circle and base circle
        Vector3D baseCircleCtr1 = tCap1.base + lineOffSet1;
        Vector3D tipCircleCtr1 = tCap1.tip + lineOffSet1;

        //repeat to find the normal of the capsule 2, which is the tip - base
        Vector3D normal2 = Vector3DNormalize(tCap2.tip - tCap2.base);
        //find the vector to reach the center of circle
        Vector3D lineOffSet2 = normal2 * tCap2.radius;
        //find the center of tip circle and base circle
        Vector3D baseCircleCtr2 = tCap2.base + lineOffSet2;
        Vector3D tipCircleCtr2 = tCap2.tip + lineOffSet2;

        // vectors between line endpoints:
        Vector3D dis0 = baseCircleCtr2 - baseCircleCtr1;
        Vector3D dis1 = tipCircleCtr2 - baseCircleCtr1;
        Vector3D dis2 = baseCircleCtr2 - tipCircleCtr1;
        Vector3D dis3 = tipCircleCtr2 - tipCircleCtr1;

        // squared distances:
        float d0 = dis0 * dis0;
        float d1 = dis1 * dis1;
        float d2 = dis2 * dis2;
        float d3 = dis3 * dis3;

        // select closest potential endpoint on capsule 1:
        Vector3D closest1;
        if (d2 < d0 || d2 < d1 || d3 < d0 || d3 < d1)
        {
            closest1 = tipCircleCtr1;
        }
        else
        {
            closest1 = baseCircleCtr1;
        }

        // select point on capsule2 line segment nearest to best potential endpoint on capsule1:
        Vector3D closest2 = ClosestPointOnLineSegment(baseCircleCtr2, tipCircleCtr2, closest1);

        // now do the same for capsule 1 segment:
        closest1 = ClosestPointOnLineSegment(baseCircleCtr1, tipCircleCtr1, closest2);

        //two closest possible candidates on both capsule axes are selected 
        //What remains is to place spheres on those pointsand perform the sphere intersection routine :
        Vector3D penetration_normal = closest1 - closest2;
        float len = penetration_normal.length();
        penetration_normal /= len;  // normalize
        float penetration_depth = tCap1.radius + tCap2.radius - len;
        bool intersects = penetration_depth > 0;

        return intersects;
    }
}