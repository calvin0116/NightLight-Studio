#include <functional>
#include <queue>

#include "CollisionMathLib.h"
#include "../../glm/gtc/matrix_transform.hpp"
#include "../../glm/gtx/euler_angles.hpp"

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
    //Rejection
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

    bool OBBToOBB(const OBBCollider& tBox1, const OBBCollider& tBox2, Vector3D& normal)
    {
            //axis view explaination: (value going from negative to positive)
            //x going from left to right
            //y going from back(into the screen) to front(out of the screen)
            //z going from bottom to top
 
           glm::vec4 _normalX1(1, 0, 0, 0);
           glm::vec4 _normalY1(0, 1, 0, 0);
           glm::vec4 _normalZ1(0, 0, 1, 0);
           //Mtx44RotRad(rotationalMtx, tBox1.rotation);
           
           glm::quat Quaternion(tBox1.rotation);
           glm::mat4 Rotate = glm::mat4_cast(Quaternion);

           //rotate normals to correct position
           _normalX1 = Rotate * _normalX1;
           _normalY1 = Rotate * _normalY1;
           _normalZ1 = Rotate * _normalZ1;
           
           //setting up normals for Box2
           glm::vec4 _normalX2(1, 0, 0, 0);
           glm::vec4 _normalY2(0, 1, 0, 0);
           glm::vec4 _normalZ2(0, 0, 1, 0);
           
           glm::quat Quaternion2(tBox2.rotation);
           Rotate = glm::mat4_cast(Quaternion2);
           
           //rotate normals to correct position
           _normalX2 = Rotate * _normalX2;
           _normalY2 = Rotate * _normalY2;
           _normalZ2 = Rotate * _normalZ2;

           glm::vec3 normalX1 = _normalX1;
           glm::vec3 normalY1 = _normalY1;
           glm::vec3 normalZ1 = _normalZ1;
           
           glm::vec3 normalX2 = _normalX2;
           glm::vec3 normalY2 = _normalY2;
           glm::vec3 normalZ2 = _normalZ2;

           //get distance vector between two box's center
           glm::vec3 centerDistance = tBox2.center - tBox1.center;
           
           //to get the smallest seperating axis
           auto compare = [](const std::pair<float, glm::vec3> lhs, const std::pair<float, glm::vec3> rhs)
           {
               return lhs.first > rhs.first;
           };

           std::priority_queue < std::pair<float, glm::vec3>, std::vector<std::pair<float, glm::vec3>>, decltype(compare)>  checkList(compare);

           // check if there's a separating plane in between the selected axes
           auto getSeparatingPlane = [&](const glm::vec3& normal)
           {
               std::pair<float, glm::vec3> tmp;
               tmp.first = (fabs(glm::dot(centerDistance, normal)) -
                         (fabs(glm::dot((normalX1 * tBox1.extend.x), normal)) +
                       fabs(glm::dot((normalY1 * tBox1.extend.y), normal)) +
                       fabs(glm::dot((normalZ1 * tBox1.extend.z), normal)) +
                       fabs(glm::dot((normalX2 * tBox2.extend.x), normal)) +
                       fabs(glm::dot((normalY2 * tBox2.extend.y), normal)) +
                       fabs(glm::dot((normalZ2 * tBox2.extend.z), normal))));
               bool check = (tmp.first > 0);
               tmp.first = fabs(tmp.first);;
               tmp.second = normal;
               checkList.push(tmp);

               return check;
           };


           //checkList.push(std::pair<float, glm::vec3>(getSeparatingPlane(normalX1), normalX1));
           //checkList.push(std::pair<float, glm::vec3>(getSeparatingPlane(normalY1), normalY1));
           //checkList.push(std::pair<float, glm::vec3>(getSeparatingPlane(normalZ1), normalZ1));
           //checkList.push(std::pair<float, glm::vec3>(getSeparatingPlane(normalX2), normalX2));
           //checkList.push(std::pair<float, glm::vec3>(getSeparatingPlane(normalY2), normalY2));
           //checkList.push(std::pair<float, glm::vec3>(getSeparatingPlane(normalZ2), normalZ2));

           //checkList.push(std::pair<float, glm::vec3>(getSeparatingPlane(glm::cross(normalX1, normalX2)), glm::cross(normalX1, normalX2)));
           //checkList.push(std::pair<float, glm::vec3>(getSeparatingPlane(glm::cross(normalX1, normalY2)), glm::cross(normalX1, normalX2)));
           //checkList.push(std::pair<float, glm::vec3>(getSeparatingPlane(glm::cross(normalX1, normalZ2)), glm::cross(normalX1, normalX2)));
           //                                                                                                                   
           //checkList.push(std::pair<float, glm::vec3>(getSeparatingPlane(glm::cross(normalY1, normalX2)), glm::cross(normalY1, normalX2)));
           //checkList.push(std::pair<float, glm::vec3>(getSeparatingPlane(glm::cross(normalY1, normalY2)), glm::cross(normalY1, normalY2)));
           //checkList.push(std::pair<float, glm::vec3>(getSeparatingPlane(glm::cross(normalY1, normalZ2)), glm::cross(normalY1, normalZ2)));
           //                                                                                                                   
           //checkList.push(std::pair<float, glm::vec3>(getSeparatingPlane(glm::cross(normalZ1, normalX2)), glm::cross(normalZ1, normalX2)));
           //checkList.push(std::pair<float, glm::vec3>(getSeparatingPlane(glm::cross(normalZ1, normalY2)), glm::cross(normalZ1, normalY2)));
           //checkList.push(std::pair<float, glm::vec3>(getSeparatingPlane(glm::cross(normalZ1, normalZ2)), glm::cross(normalZ1, normalZ2)));



           bool check = !(
               getSeparatingPlane(normalX1) ||
               getSeparatingPlane(normalY1) ||
               getSeparatingPlane(normalZ1) ||
               getSeparatingPlane(normalX2) ||
               getSeparatingPlane(normalY2) ||
               getSeparatingPlane(normalZ2) ||
           
               getSeparatingPlane(glm::cross(normalX1, normalX2)) ||
               getSeparatingPlane(glm::cross(normalX1, normalY2)) ||
               getSeparatingPlane(glm::cross(normalX1, normalZ2)) ||
           
               getSeparatingPlane(glm::cross(normalY1, normalX2)) ||
               getSeparatingPlane(glm::cross(normalY1, normalY2)) ||
               getSeparatingPlane(glm::cross(normalY1, normalZ2)) ||
           
               getSeparatingPlane(glm::cross(normalZ1, normalX2)) ||
               getSeparatingPlane(glm::cross(normalZ1, normalY2)) ||
               getSeparatingPlane(glm::cross(normalZ1, normalZ2)));

               normal = checkList.top().second;

           return check;
    }
    bool CapsuleToCapsule(const CapsuleCollider& tCap1, const CapsuleCollider& tCap2, Vector3D& normal)
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

		// normal ?
		normal = penetration_normal;

        return intersects;
    }
	SIDES AABBToCapsule(const AABBCollider& tBox1, const CapsuleCollider& tCap2, NlMath::Vector3D& nromal)
	{
		//                MAX
		//      C--------D
		//    /        / |
		//  /        /   |
		// A--------B    |
		// |        |    |
		// |        |    /
		// |        |  /
		// |________|/
		// MIN

		// all 8 vertices
		Vector3D topA = Vector3D(tBox1.vecMin.x, tBox1.vecMax.y, tBox1.vecMin.z);
		Vector3D topB = Vector3D(tBox1.vecMax.x, tBox1.vecMax.y, tBox1.vecMin.z);
		Vector3D topC = Vector3D(tBox1.vecMin.x, tBox1.vecMax.y, tBox1.vecMax.z);
		Vector3D topD = Vector3D(tBox1.vecMax.x, tBox1.vecMax.y, tBox1.vecMax.z);

		Vector3D botA = Vector3D(tBox1.vecMin.x, tBox1.vecMin.y, tBox1.vecMin.z);
		Vector3D botB = Vector3D(tBox1.vecMax.x, tBox1.vecMin.y, tBox1.vecMin.z);
		Vector3D botC = Vector3D(tBox1.vecMin.x, tBox1.vecMin.y, tBox1.vecMax.z);
		Vector3D botD = Vector3D(tBox1.vecMax.x, tBox1.vecMin.y, tBox1.vecMax.z);

		// check all vertices for closest point to the capsule
		Vector3D closestPointOnCapsuleLine[8];
		closestPointOnCapsuleLine[0] = ClosestPointOnLineSegment(tCap2.base, tCap2.tip, topA);
		closestPointOnCapsuleLine[1] = ClosestPointOnLineSegment(tCap2.base, tCap2.tip, topB);
		closestPointOnCapsuleLine[2] = ClosestPointOnLineSegment(tCap2.base, tCap2.tip, topC);
		closestPointOnCapsuleLine[3] = ClosestPointOnLineSegment(tCap2.base, tCap2.tip, topD);
		closestPointOnCapsuleLine[4] = ClosestPointOnLineSegment(tCap2.base, tCap2.tip, botA);
		closestPointOnCapsuleLine[5] = ClosestPointOnLineSegment(tCap2.base, tCap2.tip, botB);
		closestPointOnCapsuleLine[6] = ClosestPointOnLineSegment(tCap2.base, tCap2.tip, botC);
		closestPointOnCapsuleLine[7] = ClosestPointOnLineSegment(tCap2.base, tCap2.tip, botD);

		// get sq dist
		float dist[8];
		for (int i = 0; i < 8; ++i)
		{
			dist[i] = closestPointOnCapsuleLine[i] * closestPointOnCapsuleLine[i];
		}

		// find smallest dist
		int smallestDistIndex = 0;
		float smallestDistance = dist[0];
		for (int i = 0; i < 7; ++i)
		{
			if (dist[i] > dist[i + 1])
			{
				smallestDistIndex = i + 1;
				smallestDistance = dist[i + 1];
			}
		}

		// test aabb to spehere using sphere on closest point on capsule linesegment

		SphereCollider closestsphere;
		closestsphere.center = closestPointOnCapsuleLine[smallestDistIndex];
		closestsphere.radius = tCap2.radius;

		return AABB_SphereCollision(tBox1, closestsphere, nromal);
		//return SIDES::NO_COLLISION;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////
	//// 2D
	// some 2d checks we may or may not need in the future
	// we can change them to 3d if we need them
	bool Point_Rectangle_2D(Vec2 point, Vec2 rectVertA, Vec2 rectVertB, Vec2 rectVertC, Vec2 rectVertD)
	{
		Vec2 AP = point - rectVertA;
		Vec2 AB = rectVertB - rectVertA;
		float AP_AB = AP * AB;
		float AB_AB = AB * AB;
		Vec2 AD = rectVertD - rectVertA;
		float AP_AD = AP * AD;
		float AD_AD = AD * AD;
		// 0 <= AP.AB <= AB.AB and 0 <= AP.AD <= AD.AD
		return (
			0 <= AP_AB &&
			AP_AB <= AB_AB &&
			0 <= AP_AD &&
			AP_AD <= AD_AD
			);
	}
	bool Line_Circle_2D(Vec2 circleCenter, float circleRadius, Vec2 rectVertA, Vec2 rectVertB)
	{
		// https://stackoverflow.com/questions/1073336/circle-line-segment-collision-detection-algorithm

		Vec2 d = rectVertB - rectVertA;
		Vec2 f = rectVertA - circleCenter;

		float a = d * d;
		float b = 2 * f * d;
		float c = f * f - circleRadius * circleRadius;

		float discriminant = b * b - 4 * a * c; // b^2 - 4ac

		if (discriminant < 0) return false; // no intersection

		discriminant = sqrt(discriminant);

		// time of intersection
		float t1 = (-b - discriminant) / (2 * a);
		if (t1 >= 0 && t1 <= 1)
		{
			// within the toi
			return true;
		}

		float t2 = (-b + discriminant) / (2 * a);
		if (t2 >= 0 && t2 <= 1)
		{
			// inside sphere or past the sphere
			return true;
		}

		return false;
	}
	bool Circle_Rectangle_2D(Vec2 circleCenter, float circleRadius, Vec2 rectVertA, Vec2 rectVertB, Vec2 rectVertC, Vec2 rectVertD)
	{
		return(
			Point_Rectangle_2D(circleCenter, rectVertA, rectVertB, rectVertC, rectVertD) ||
			Line_Circle_2D(circleCenter, circleRadius, rectVertA, rectVertB) ||
			Line_Circle_2D(circleCenter, circleRadius, rectVertB, rectVertC) ||
			Line_Circle_2D(circleCenter, circleRadius, rectVertC, rectVertD) ||
			Line_Circle_2D(circleCenter, circleRadius, rectVertD, rectVertA)
			);
	}
	//// 2D END
	///////////////////////////////////////////////////////////////////////////////////////////////
}