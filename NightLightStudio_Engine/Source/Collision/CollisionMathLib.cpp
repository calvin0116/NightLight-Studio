#include <functional>
#include <queue>

#include "CollisionMathLib.h"
#include "../../glm/gtc/matrix_transform.hpp"
#include "../../glm/gtx/euler_angles.hpp"

#define InFront( a ) \
	((a) < float( 0.0 ))

#define Behind( a ) \
	((a) >= float( 0.0 ))

#define On( a ) \
	((a) < float( 0.005 ) && (a) > -float( 0.005 ))

namespace NlMath
{
    Vector3D ClosestPointOnLineSegment(Vector3D segmentPointA, Vector3D segmentPointB, Vector3D CheckPoint)
    {

		// ref : https://math.stackexchange.com/questions/2193720/find-a-point-on-a-line-segment-which-is-the-closest-to-other-point-not-on-the-li
		Vector3D v = segmentPointB - segmentPointA;
		Vector3D u = segmentPointA - CheckPoint;
		float vu = v * u;
		float vv = v * v;
		float t = -vu / vv;
		auto vectorToSegment = [&](float _t, Vector3D _p)
		{
			return Vector3D(
				(1 - _t) * segmentPointA.x + _t * segmentPointB.x - _p.x,
				(1 - _t) * segmentPointA.y + _t * segmentPointB.y - _p.y,
				(1 - _t) * segmentPointA.z + _t * segmentPointB.z - _p.z
			);
		};
		if (t >= 0 && t <= 1) 
			return vectorToSegment(t, Vector3D(0.0f, 0.0f, 0.0f));
		Vector3D g0 = vectorToSegment(0, CheckPoint);
		Vector3D g1 = vectorToSegment(1, CheckPoint);
		float g0sq = g0 * g0;
		float g1sq = g1 * g1;

		if (g0sq <= g1sq)
		{
			return segmentPointA;
		}
		else
		{
			return segmentPointB;
		}

        ////construct linesegment AB
        //Vector3D AB = segmentPointB - segmentPointA;
        ////find projection
        //float t = Vector3DDotProduct(CheckPoint - segmentPointA, AB) / Vector3DDotProduct(AB, AB);
        //return segmentPointA + t * AB;
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

    bool PlaneToPlane(const PlaneCollider& /*tPlane1*/, const PlaneCollider& /*tPlane2*/)
    {
        //Matrix4x4 rotationalMtx;

        ////setting up normals for plane1
        //Vector3D normalX1(1, 0, 0);
        //Vector3D normalY1(0, 1, 0);
        //Vector3D normalZ1(0, 0, 1);
        //Mtx44RotRad(rotationalMtx, tPlane1.rotation);
        ////rotate normals to correct position
        //normalX1 = rotationalMtx * normalX1;
        //normalY1 = rotationalMtx * normalY1;
        //normalZ1 = rotationalMtx * normalZ1;

        ////setting up normals for plane2
        //Vector3D normalX2(1, 0, 0);
        //Vector3D normalY2(0, 1, 0);
        //Vector3D normalZ2(0, 0, 1);
        //Mtx44RotRad(rotationalMtx, tPlane2.rotation);
        ////rotate normals to correct position
        //normalX2 = rotationalMtx * normalX2;
        //normalY2 = rotationalMtx * normalY2;
        //normalZ2 = rotationalMtx * normalZ2;

        //Vector3D nearPoint;
        //Vector3D farPoint;
        //Vector3D maxDistance;
        //Vector3D maxDistanceToCtr;
        //float farLength = -FLT_MAX;
        //float nearLength = FLT_MAX;

        //float tmpLength = 0;

        //maxDistance = tPlane1.center + normalX1 * tPlane1.extend.x + normalZ1 * tPlane1.extend.z;
        //maxDistanceToCtr = tPlane1.center - maxDistance;



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
		// ref: https://stackoverflow.com/questions/28343716/sphere-intersection-test-of-aabb
		// check for aabb sphere
		float dmin = 0;
		if (tSpr2.center.x < tBox1.vecMin.x)
		{
			dmin += (tSpr2.center.x - tBox1.vecMin.x) * (tSpr2.center.x - tBox1.vecMin.x);
		}
		else if (tSpr2.center.x > tBox1.vecMax.x)
		{
			dmin += (tSpr2.center.x - tBox1.vecMax.x) * (tSpr2.center.x - tBox1.vecMax.x);
		}
		if (tSpr2.center.y < tBox1.vecMin.y)
		{
			dmin += (tSpr2.center.y - tBox1.vecMin.y) * (tSpr2.center.y - tBox1.vecMin.y);
		}
		else if (tSpr2.center.y > tBox1.vecMax.y)
		{
			dmin += (tSpr2.center.y - tBox1.vecMax.y) * (tSpr2.center.y - tBox1.vecMax.y);
		}
		if (tSpr2.center.z < tBox1.vecMin.z)
		{
			dmin += (tSpr2.center.z - tBox1.vecMin.z) * (tSpr2.center.z - tBox1.vecMin.z);
		}
		else if (tSpr2.center.z > tBox1.vecMax.z)
		{
			dmin += (tSpr2.center.z - tBox1.vecMax.z) * (tSpr2.center.z - tBox1.vecMax.z);
		}
		if (dmin <= (tSpr2.radius * tSpr2.radius))
		{
			// need to return the aabb collision side ... cont fn
		}
		else  return SIDES::NO_COLLISION; // no collide

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

        if (penetrationDepth.x <= 0 || penetrationDepth.y <= 0 || penetrationDepth.z <= 0) // prob unnecessary now since no col check above
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
 
           Vec3 normalX1 = tBox1.rotation.Column0();
           Vec3 normalY1 = tBox1.rotation.Column1();
           Vec3 normalZ1 = tBox1.rotation.Column2();

           //setting up normals for Box2
           Vec3 normalX2 = tBox2.rotation.Column0();
           Vec3 normalY2 = tBox2.rotation.Column1();
           Vec3 normalZ2 = tBox2.rotation.Column2();

           //get distance vector between two box's center
           Vec3 centerDistance = tBox2.center - tBox1.center;
           
           // check if there's a separating plane in between the selected axes
           auto getSeparatingPlane = [&](const Vec3& normal)
           {
               return (fabs(centerDistance* normal) >
                         (fabs((normalX1 * tBox1.extend.x)* normal)) +
                       fabs((normalY1 * tBox1.extend.y)* normal) +
                       fabs((normalZ1 * tBox1.extend.z)* normal) +
                       fabs((normalX2 * tBox2.extend.x)* normal) +
                       fabs((normalY2 * tBox2.extend.y)* normal) +
                       fabs((normalZ2 * tBox2.extend.z)* normal));
           };

           return !(
               getSeparatingPlane(normalX1) ||
               getSeparatingPlane(normalY1) ||
               getSeparatingPlane(normalZ1) ||
               getSeparatingPlane(normalX2) ||
               getSeparatingPlane(normalY2) ||
               getSeparatingPlane(normalZ2) ||
                                                               
               getSeparatingPlane(Vector3DCrossProduct(normalX1, normalX2)) ||
               getSeparatingPlane(Vector3DCrossProduct(normalX1, normalY2)) ||
               getSeparatingPlane(Vector3DCrossProduct(normalX1, normalZ2)) ||
                                              
               getSeparatingPlane(Vector3DCrossProduct(normalY1, normalX2)) ||
               getSeparatingPlane(Vector3DCrossProduct(normalY1, normalY2)) ||
               getSeparatingPlane(Vector3DCrossProduct(normalY1, normalZ2)) ||
                                              
               getSeparatingPlane(Vector3DCrossProduct(normalZ1, normalX2)) ||
               getSeparatingPlane(Vector3DCrossProduct(normalZ1, normalY2)) ||
               getSeparatingPlane(Vector3DCrossProduct(normalZ1, normalZ2)));
    }
    bool CapsuleToCapsule(const CapsuleCollider& tCap1, const CapsuleCollider& tCap2, Vector3D& normal)
    {
        //Capsule collision is made up of 2 circles, one at tip and the other at base
        
        //start by finding the normal of the capsule 1, which is the tip - base
        Vector3D normal1 = Vector3DNormalize(tCap1.tip - tCap1.base);
        //find the vector to reach the center of circle
        //Vector3D lineOffSet1 = normal1 * tCap1.radius;
        //find the center of tip circle and base circle
        Vector3D baseCircleCtr1 = tCap1.base/* + lineOffSet1*/;
        Vector3D tipCircleCtr1 = tCap1.tip/* + lineOffSet1*/;

        //repeat to find the normal of the capsule 2, which is the tip - base
        Vector3D normal2 = Vector3DNormalize(tCap2.tip - tCap2.base);
        //find the vector to reach the center of circle
        //Vector3D lineOffSet2 = normal2 * tCap2.radius;
        //find the center of tip circle and base circle
        Vector3D baseCircleCtr2 = tCap2.base/* + lineOffSet2*/;
        Vector3D tipCircleCtr2 = tCap2.tip/* + lineOffSet2*/;

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

	bool CapsuleToSphere(const CapsuleCollider& tCap1, const SphereCollider& tSpr2, Vector3D& normal)
	{
		Vector3D closestPoint = ClosestPointOnLineSegment(tCap1.base, tCap1.tip, tSpr2.center);

		SphereCollider capsuleSphere;
		capsuleSphere.center = closestPoint;
		capsuleSphere.radius = tCap1.radius;
		normal = Sphere_SphereCollision(capsuleSphere, tSpr2);

		if (normal.x == 0.0f && normal.y == 0.0f && normal.z == 0.0f)
		{
			return false; // no collision return false
		}
		
		return true;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////
	//// 2D
	// some 2d checks we may or may not need in the future
	// we can change them to 3d if we need them
	bool Point_Rectangle_2D(Vec3 point, Vec3 rectVertA, Vec3 rectVertB, Vec3 /*rectVertC*/, Vec3 rectVertD)
	{
		Vec3 AP = point - rectVertA;
		Vec3 AB = rectVertB - rectVertA;
		float AP_AB = AP * AB;
		float AB_AB = AB * AB;
		Vec3 AD = rectVertD - rectVertA;
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
	bool Line_Circle_2D(Vec3 circleCenter, float circleRadius, Vec3 rectVertA, Vec3 rectVertB)
	{
		// https://stackoverflow.com/questions/1073336/circle-line-segment-collision-detection-algorithm

		Vec3 d = rectVertB - rectVertA;
		Vec3 f = rectVertA - circleCenter;

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
	bool Circle_Rectangle_2D(Vec3 circleCenter, float circleRadius, Vec3 rectVertA, Vec3 rectVertB, Vec3 rectVertC, Vec3 rectVertD)
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


    //TEST

    bool OBB_OBBCollision(const OBBCollider& tBox1, const OBBCollider& tBox2, Manifold& manifold)
    {
        //axis view explaination: (value going from negative to positive)
        //x going from left to right
        //y going from back(into the screen) to front(out of the screen)
        //z going from bottom to top

        Vec3 normalX1(1, 0, 0);
        Vec3 normalY1(0, 1, 0);
        Vec3 normalZ1(0, 0, 1);

        //rotate normals to correct position
        normalX1 = tBox1.rotation * normalX1;
        normalY1 = tBox1.rotation * normalY1;
        normalZ1 = tBox1.rotation * normalZ1;


        //setting up normals for Box2
        Vec3 normalX2(1, 0, 0);
        Vec3 normalY2(0, 1, 0);
        Vec3 normalZ2(0, 0, 1);

        //rotate normals to correct position
        normalX2 = tBox2.rotation * normalX2;
        normalY2 = tBox2.rotation * normalY2;
        normalZ2 = tBox2.rotation * normalZ2;

        // box2's frame in box1's space
        Mtx44 C = NlMath::Mtx44Transpose(tBox1.rotation) * tBox2.rotation;

        Mtx44 absC;
        bool parallel = false;
        const float kCosTol = float(1.0e-6);
        for (unsigned int i = 0; i < 3; ++i)
        {
            for (unsigned int j = 0; j < 3; ++j)
            {
                float val = std::fabs(C.m2[i][j]);
                absC.m2[i][j] = val;

                if (val + kCosTol >= float(1.0))
                    parallel = true;
            }
        }

        //get distance vector between two box's center in box1's space
        //Vec3 centerDistance = Mtx44Transpose(tBox1.rotation) * (tBox2.center - tBox1.center);
        Vec3 centerDistance = tBox2.center - tBox1.center;

        // Query states
        float penetrationDepth;
        float aMax = -FLT_MAX;
        float bMax = -FLT_MAX;
        float eMax = -FLT_MAX;
        unsigned int aAxis = ~0;
        unsigned int bAxis = ~0;
        unsigned int eAxis = ~0;
        Vec3 nA;
        Vec3 nB;
        Vec3 nE;

        // check if there's a separating plane in between the selected axes

        auto getSeparatingPlane = [=](const Vec3& normal)
        {
            return (fabs((centerDistance * normal)) -
                (fabs(((normalX1 * tBox1.extend.x) * normal)) +
                    fabs(((normalY1 * tBox1.extend.y) * normal)) +
                    fabs(((normalZ1 * tBox1.extend.z) * normal)) +
                    fabs(((normalX2 * tBox2.extend.x) * normal)) +
                    fabs(((normalY2 * tBox2.extend.y) * normal)) +
                    fabs(((normalZ2 * tBox2.extend.z) * normal))));
        };

        auto TrackFaceAxis = [&](unsigned int* axis, unsigned int n, float s, float* sMax, const Vec3& normal, Vec3* axisNormal)
        {
            //no collision if penetration is positive
            if (s > float(0.0))
                return true;

            if (s > * sMax)
            {
                *sMax = s;
                *axis = n;
                *axisNormal = normal;
            }

            return false;
        };

        // box 1's x axis
        penetrationDepth = getSeparatingPlane(normalX1);
        if (TrackFaceAxis(&aAxis, 0, penetrationDepth, &aMax, normalX1, &nA))
            return false;

        // box 1's y axis
        penetrationDepth = getSeparatingPlane(normalY1);
        if (TrackFaceAxis(&aAxis, 1, penetrationDepth, &aMax, normalY1, &nA))
            return false;

        // box 1's z axis
        penetrationDepth = getSeparatingPlane(normalZ1);
        if (TrackFaceAxis(&aAxis, 2, penetrationDepth, &aMax, normalZ1, &nA))
            return false;

        // box 2's x axis
        penetrationDepth = getSeparatingPlane(normalX2);
        if (TrackFaceAxis(&bAxis, 3, penetrationDepth, &bMax, normalX2, &nB))
            return false;

        // box 2's y axis
        penetrationDepth = getSeparatingPlane(normalY2);
        if (TrackFaceAxis(&bAxis, 4, penetrationDepth, &bMax, normalY2, &nB))
            return false;

        // box 2's z axis
        penetrationDepth = getSeparatingPlane(normalZ2);
        if (TrackFaceAxis(&bAxis, 5, penetrationDepth, &bMax, normalZ2, &nB))
            return false;

        //// box 1's x axis
        //penetrationDepth = fabs(centerDistance.x) - (tBox1.extend.x + (absC.Column0()* tBox2.extend));
        //if (TrackFaceAxis(&aAxis, 0, penetrationDepth, &aMax, tBox1.rotation.Row0(), &nA))
        //    return false;

        //// box 1's y axis
        //penetrationDepth = fabs(centerDistance.y) - (tBox1.extend.y + (absC.Column1()* tBox2.extend));
        //if (TrackFaceAxis(&aAxis, 1, penetrationDepth, &aMax, tBox1.rotation.Row1(), &nA))
        //    return false;

        //// box 1's z axis
        //penetrationDepth = fabs(centerDistance.z) - (tBox1.extend.z + (absC.Column2()* tBox2.extend));
        //if (TrackFaceAxis(&aAxis, 2, penetrationDepth, &aMax, tBox1.rotation.Row2(), &nA))
        //    return false;

        //// box 2's x axis
        //penetrationDepth = fabs((centerDistance* C.Row0())) - (tBox2.extend.x + (absC.Row0()* tBox1.extend));
        //if (TrackFaceAxis(&bAxis, 3, penetrationDepth, &bMax, tBox2.rotation.Row0(), &nB))
        //    return false;

        //// box 2's y axis
        //penetrationDepth = fabs((centerDistance* C.Row1())) - (tBox2.extend.y + (absC.Row1()* tBox1.extend));
        //if (TrackFaceAxis(&bAxis, 4, penetrationDepth, &bMax, tBox2.rotation.Row1(), &nB))
        //    return false;

        //// box 2's z axis
        //penetrationDepth = fabs((centerDistance* C.Row2())) - (tBox2.extend.z + (absC.Row2()* tBox1.extend));
        //if (TrackFaceAxis(&bAxis, 5, penetrationDepth, &bMax, tBox2.rotation.Row2(), &nB))
        //    return false;

        if (!parallel)
        {
            Vec3 tmp;
            // Edge axis checks

            tmp = Vector3DCrossProduct(normalX1, normalX2);
            penetrationDepth = getSeparatingPlane(tmp);
            if (TrackFaceAxis(&eAxis, 6, penetrationDepth, &eMax, tmp, &nE))
                return false;

            tmp = Vector3DCrossProduct(normalX1, normalY2);
            penetrationDepth = getSeparatingPlane(tmp);
            if (TrackFaceAxis(&eAxis, 7, penetrationDepth, &eMax, tmp, &nE))
                return false;

            tmp = Vector3DCrossProduct(normalX1, normalZ2);
            penetrationDepth = getSeparatingPlane(tmp);
            if (TrackFaceAxis(&eAxis, 8, penetrationDepth, &eMax, tmp, &nE))
                return false;

            tmp = Vector3DCrossProduct(normalY1, normalX2);
            penetrationDepth = getSeparatingPlane(tmp);
            if (TrackFaceAxis(&eAxis, 9, penetrationDepth, &eMax, tmp, &nE))
                return false;

            tmp = Vector3DCrossProduct(normalY1, normalY2);
            penetrationDepth = getSeparatingPlane(tmp);
            if (TrackFaceAxis(&eAxis, 10, penetrationDepth, &eMax, tmp, &nE))
                return false;

            tmp = Vector3DCrossProduct(normalY1, normalZ2);
            penetrationDepth = getSeparatingPlane(tmp);
            if (TrackFaceAxis(&eAxis, 11, penetrationDepth, &eMax, tmp, &nE))
                return false;

            tmp = Vector3DCrossProduct(normalZ1, normalX2);
            penetrationDepth = getSeparatingPlane(tmp);
            if (TrackFaceAxis(&eAxis, 12, penetrationDepth, &eMax, tmp, &nE))
                return false;

            tmp = Vector3DCrossProduct(normalZ1, normalY2);
            penetrationDepth = getSeparatingPlane(tmp);
            if (TrackFaceAxis(&eAxis, 13, penetrationDepth, &eMax, tmp, &nE))
                return false;

            tmp = Vector3DCrossProduct(normalZ1, normalZ2);
            penetrationDepth = getSeparatingPlane(tmp);
            if (TrackFaceAxis(&eAxis, 14, penetrationDepth, &eMax, tmp, &nE))
                return false;
        }

        // Artificial axis bias to improve frame coherence
        const float kRelTol = float(0.95);
        const float kAbsTol = float(0.01);
        unsigned int axis;
        float sMax;
        Vec3 n;
        float faceMax = aMax > bMax ? aMax : bMax;
        if (kRelTol * eMax > faceMax + kAbsTol)
        {
            axis = eAxis;
            sMax = eMax;
            n = nE;
        }

        else
        {
            if (kRelTol * bMax > aMax + kAbsTol)
            {
                axis = bAxis;
                sMax = bMax;
                n = nB;
            }

            else
            {
                axis = aAxis;
                sMax = aMax;
                n = nA;
            }
        }

        if ((n * (tBox2.center - tBox1.center)) < float(0.0))
            n = -n;

        if (axis == ~0)
            return false;

        if (axis < 6)
        {
            OBBCollider referenceBox;
            OBBCollider incidentBox;

            bool flip;

            if (axis < 3)
            {
                referenceBox = tBox1;
                incidentBox = tBox2;
                flip = false;
            }

            else
            {
                referenceBox = tBox2;
                incidentBox = tBox1;
                flip = true;
                n = -n;
            }


            // Compute reference and incident edge information necessary for clipping
            ClipVertex incident[4];
            ComputeIncidentFace(incidentBox, n, incident);
            unsigned char clipEdges[4];
            Matrix4x4 basis;
            Vec3 e;
            ComputeReferenceEdgesAndBasis(referenceBox, n, axis, clipEdges, &basis, &e);

            // Clip the incident face against the reference face side planes
            ClipVertex out[8];
            float depths[8];
            unsigned int outNum;
            outNum = Clip(referenceBox.center, e, clipEdges, basis, incident, out, depths);

            if (outNum)
            {
                manifold.contactCount = outNum;
                manifold.normal = flip ? -n : n;

                for (unsigned int i = 0; i < outNum; ++i)
                {
                    Contact* contact = manifold.contacts + i;

                    FeaturePair pair = out[i].f;

                    if (flip)
                    {
                        std::swap(pair.inI, pair.inR);
                        std::swap(pair.outI, pair.outR);
                    }

                    contact->fp = out[i].f;
                    contact->position = out[i].v;
                    contact->penetration = depths[i];
                }
            }
        }

        else
        {
            n = tBox1.rotation * n;

            if (n * (tBox2.center - tBox1.center) < float(0.06))
                n = -n;

            Vec3 PA, QA;
            Vec3 PB, QB;
            SupportEdge(tBox1, n, &PA, &QA);
            SupportEdge(tBox2, -n, &PB, &QB);

            Vec3 CA, CB;
            EdgesContact(&CA, &CB, PA, QA, PB, QB);

            manifold.normal = n;
            manifold.contactCount = 1;

            Contact* c = manifold.contacts;
            FeaturePair pair;
            pair.key = axis;
            c->fp = pair;
            c->penetration = sMax;
            c->position = (CA + CB) * float(0.5);
        }

        return true;

    }


    void ComputeIncidentFace(const OBBCollider& incidentBox, Vec3 n, ClipVertex* out)
    {
        n = -(Mtx44Transpose(incidentBox.rotation)* n);
        Vec3 absN = n.abs();
        Vec3 e = incidentBox.extend;
    
        if (absN.x > absN.y && absN.x > absN.z)
        {
            if (n.x > float(0.0))
            {
                out[0].v.set(e.x, e.y, -e.z);
                out[1].v.set(e.x, e.y, e.z);
                out[2].v.set(e.x, -e.y, e.z);
                out[3].v.set(e.x, -e.y, -e.z);
    
                out[0].f.inI = 9;
                out[0].f.outI = 1;
                out[1].f.inI = 1;
                out[1].f.outI = 8;
                out[2].f.inI = 8;
                out[2].f.outI = 7;
                out[3].f.inI = 7;
                out[3].f.outI = 9;
            }
    
            else
            {
                out[0].v.set(-e.x, -e.y, e.z);
                out[1].v.set(-e.x, e.y, e.z);
                out[2].v.set(-e.x, e.y, -e.z);
                out[3].v.set(-e.x, -e.y, -e.z);
    
                out[0].f.inI = 5;
                out[0].f.outI = 11;
                out[1].f.inI = 11;
                out[1].f.outI = 3;
                out[2].f.inI = 3;
                out[2].f.outI = 10;
                out[3].f.inI = 10;
                out[3].f.outI = 5;
            }
        }
    
        else if (absN.y > absN.x && absN.y > absN.z)
        {
            if (n.y > float(0.0))
            {
                out[0].v.set(-e.x, e.y, e.z);
                out[1].v.set(e.x, e.y, e.z);
                out[2].v.set(e.x, e.y, -e.z);
                out[3].v.set(-e.x, e.y, -e.z);
    
                out[0].f.inI = 3;
                out[0].f.outI = 0;
                out[1].f.inI = 0;
                out[1].f.outI = 1;
                out[2].f.inI = 1;
                out[2].f.outI = 2;
                out[3].f.inI = 2;
                out[3].f.outI = 3;
            }
    
            else
            {
                out[0].v.set(e.x, -e.y, e.z);
                out[1].v.set(-e.x, -e.y, e.z);
                out[2].v.set(-e.x, -e.y, -e.z);
                out[3].v.set(e.x, -e.y, -e.z);
    
                out[0].f.inI = 7;
                out[0].f.outI = 4;
                out[1].f.inI = 4;
                out[1].f.outI = 5;
                out[2].f.inI = 5;
                out[2].f.outI = 6;
                out[3].f.inI = 5;
                out[3].f.outI = 6;
            }
        }
    
        else
        {
            if (n.z > float(0.0))
            {
                out[0].v.set(-e.x, e.y, e.z);
                out[1].v.set(-e.x, -e.y, e.z);
                out[2].v.set(e.x, -e.y, e.z);
                out[3].v.set(e.x, e.y, e.z);
    
                out[0].f.inI = 0;
                out[0].f.outI = 11;
                out[1].f.inI = 11;
                out[1].f.outI = 4;
                out[2].f.inI = 4;
                out[2].f.outI = 8;
                out[3].f.inI = 8;
                out[3].f.outI = 0;
            }
    
            else
            {
                out[0].v.set(e.x, -e.y, -e.z);
                out[1].v.set(-e.x, -e.y, -e.z);
                out[2].v.set(-e.x, e.y, -e.z);
                out[3].v.set(e.x, e.y, -e.z);
    
                out[0].f.inI = 9;
                out[0].f.outI = 6;
                out[1].f.inI = 6;
                out[1].f.outI = 10;
                out[2].f.inI = 10;
                out[2].f.outI = 2;
                out[3].f.inI = 2;
                out[3].f.outI = 9;
            }
        }
    
        for (unsigned int i = 0; i < 4; ++i)
            out[i].v = incidentBox.rotation * out[i].v + incidentBox.center;
    }

    void ComputeReferenceEdgesAndBasis(const OBBCollider& referenceBox, Vec3 n, unsigned int axis, unsigned char* out, Matrix4x4* basis, Vec3* e)
    {
        n = Mtx44Transpose(referenceBox.rotation) * n;
    
        if (axis >= 3)
            axis -= 3;
    
        switch (axis)
        {
        case 0:
            if (n.x > float(0.0))
            {
                out[0] = 1;
                out[1] = 8;
                out[2] = 7;
                out[3] = 9;
    
                e->set(referenceBox.extend.y, referenceBox.extend.z, referenceBox.extend.x);
                basis->SetColumns(referenceBox.rotation.Column1(), referenceBox.rotation.Column2(), referenceBox.rotation.Column0());
            }
    
            else
            {
                out[0] = 11;
                out[1] = 3;
                out[2] = 10;
                out[3] = 5;
    
                e->set(referenceBox.extend.z, referenceBox.extend.y, referenceBox.extend.x);
                basis->SetColumns(referenceBox.rotation.Column2(), referenceBox.rotation.Column1(), -referenceBox.rotation.Column0());
            }
            break;
    
        case 1:
            if (n.y > float(0.0))
            {
                out[0] = 0;
                out[1] = 1;
                out[2] = 2;
                out[3] = 3;
    
                e->set(referenceBox.extend.z, referenceBox.extend.x, referenceBox.extend.y);
                basis->SetColumns(referenceBox.rotation.Column2(), referenceBox.rotation.Column0(), referenceBox.rotation.Column1());
            }
    
            else
            {
                out[0] = 4;
                out[1] = 5;
                out[2] = 6;
                out[3] = 7;
    
                e->set(referenceBox.extend.z, referenceBox.extend.x, referenceBox.extend.y);
                basis->SetColumns(referenceBox.rotation.Column2(), -referenceBox.rotation.Column0(), -referenceBox.rotation.Column1());
            }
            break;
    
        case 2:
            if (n.z > float(0.0))
            {
                out[0] = 11;
                out[1] = 4;
                out[2] = 8;
                out[3] = 0;
    
                e->set(referenceBox.extend.y, referenceBox.extend.x, referenceBox.extend.z);
                basis->SetColumns(-referenceBox.rotation.Column1(), referenceBox.rotation.Column0(), referenceBox.rotation.Column2());
            }
    
            else
            {
                out[0] = 6;
                out[1] = 10;
                out[2] = 2;
                out[3] = 9;
    
                e->set(referenceBox.extend.y, referenceBox.extend.x, referenceBox.extend.z);
                basis->SetColumns(-referenceBox.rotation.Column1(), -referenceBox.rotation.Column0(), -referenceBox.rotation.Column2());
            }
            break;
        }
    }
    
    unsigned int Orthographic(float sign, float e, unsigned int axis, unsigned int clipEdge, ClipVertex* in, unsigned int inCount, ClipVertex* out)
    {
        unsigned int outCount = 0;
        ClipVertex a = in[inCount - 1];
    
        for (unsigned int i = 0; i < inCount; ++i)
        {
            ClipVertex b = in[i];
    
            float da = sign * a.v[axis] - e;
            float db = sign * b.v[axis] - e;
    
            ClipVertex cv;
    
            // B
            if (((InFront(da) && InFront(db)) || On(da) || On(db)))
            {
                assert(outCount < 8);
                out[outCount++] = b;
            }
    
            // I
            else if (InFront(da) && Behind(db))
            {
                cv.f = b.f;
                cv.v = a.v + (b.v - a.v) * (da / (da - db));
                cv.f.outR = clipEdge;
                cv.f.outI = 0;
                assert(outCount < 8);
                out[outCount++] = cv;
            }
    
            // I, B
            else if (Behind(da) && InFront(db))
            {
                cv.f = a.f;
                cv.v = a.v + (b.v - a.v) * (da / (da - db));
                cv.f.inR = clipEdge;
                cv.f.inI = 0;
                assert(outCount < 8);
                out[outCount++] = cv;
    
                assert(outCount < 8);
                out[outCount++] = b;
            }
    
            a = b;
        }
    
        return outCount;
    }
    
    unsigned int Clip(const Vec3& rPos, const Vec3& e, unsigned char* clipEdges, const Matrix4x4& basis, ClipVertex* incident, ClipVertex* outVerts, float* outDepths)
    {
        int inCount = 4;
        int outCount;
        ClipVertex in[8];
        ClipVertex out[8];
    
        for (unsigned int i = 0; i < 4; ++i)
            in[i].v = Mtx44Transpose(basis) * (incident[i].v - rPos); 
    
        //sign , extend , axis, clipedge represented with int, clipvertex passed in, number of clip
        outCount = Orthographic(float(1.0), e.x, 0, clipEdges[0], in, inCount, out);
    
        if (!outCount)
            return 0;
    
        inCount = Orthographic(float(1.0), e.y, 1, clipEdges[1], out, outCount, in);
    
        if (!inCount)
            return 0;
    
        outCount = Orthographic(float(-1.0), e.x, 0, clipEdges[2], in, inCount, out);
    
        if (!outCount)
            return 0;
    
        inCount = Orthographic(float(-1.0), e.y, 1, clipEdges[3], out, outCount, in);
    
        // Keep incident vertices behind the reference face
        outCount = 0;
        for (unsigned int i = 0; i < inCount; ++i)
        {
            float d = in[i].v.z - e.z;
    
            if (d <= float(0.006))
            {
                outVerts[outCount].v = (basis* in[i].v) + rPos;
                outVerts[outCount].f = in[i].f;
                outDepths[outCount++] = d;
            }
        }
    
        return outCount;
    }
    void SupportEdge(const OBBCollider& tx, Vec3 n, Vec3* aOut, Vec3* bOut)
    {

        n = Mtx44Transpose( tx.rotation) * n;
        Vec3 absN = n.abs();
        Vec3 a, b;

        // x > y
        if (absN.x > absN.y)
        {
            // x > y > z
            if (absN.y > absN.z)
            {
                a.set(tx.extend.x, tx.extend.y, tx.extend.z);
                b.set(tx.extend.x, tx.extend.y, -tx.extend.z);
            }

            // x > z > y || z > x > y
            else
            {
                a.set(tx.extend.x, tx.extend.y, tx.extend.z);
                b.set(tx.extend.x, -tx.extend.y, tx.extend.z);
            }
        }

        // y > x
        else
        {
            // y > x > z
            if (absN.x > absN.z)
            {
                a.set(tx.extend.x, tx.extend.y, tx.extend.z);
                b.set(tx.extend.x, tx.extend.y, -tx.extend.z);
            }

            // z > y > x || y > z > x
            else
            {
                a.set(tx.extend.x, tx.extend.y, tx.extend.z);
                b.set(-tx.extend.x, tx.extend.y, tx.extend.z);
            }
        }

        float signx = (n.x >= float(0.0)) ? float(1.0) : float(-1.0);
        float signy = (n.y >= float(0.0)) ? float(1.0) : float(-1.0);
        float signz = (n.z >= float(0.0)) ? float(1.0) : float(-1.0);

        a.x *= signx;
        a.y *= signy;
        a.z *= signz;
        b.x *= signx;
        b.y *= signy;
        b.z *= signz;

        *aOut = tx.rotation * a + tx.center;
        *bOut = tx.rotation * b + tx.center;
    }

    void EdgesContact(Vec3* CA, Vec3* CB, const Vec3& PA, const Vec3& QA, const Vec3& PB, const Vec3& QB)
    {
        Vec3 DA = QA - PA;
        Vec3 DB = QB - PB;
        Vec3 r = PA - PB;
        float a = (DA * DA);
        float e = (DB * DB);
        float f = (DB * r);
        float c = (DA * r);

        float b = (DA* DB);
        float denom = a * e - b * b;

        float TA = (b * f - c * e) / denom;
        float TB = (b * TA + f) / e;

        *CA = PA + DA * TA;
        *CB = PB + DB * TB;
    }
}
