#include <cstdint>
#include <cmath>
#include "Vector.h"

namespace NlMath
{
	/**************************************************************************/
	/*!
		default constructor
	*/
	/**************************************************************************/
	Vector3D::Vector3D() :_x{ 0 }, _y{ 0 }, _z{ 0 }
	{
	}

	Vector3D::Vector3D(float set) : _x{ set }, _y{ set }, _z{ set }
	{
	}


	/**************************************************************************/
	/*!
		3D vector ctor from 2 floats
	*/
	/**************************************************************************/
	Vector3D::Vector3D(float _x, float _y, float _z) : _x(_x), _y(_y), _z(_z)
	{
	}

	/**************************************************************************/
	/*!
		Copy constructor
	*/
	/**************************************************************************/
	Vector3D::Vector3D(const Vector3D& rhs) : _x(rhs._x), _y(rhs._y), _z(rhs._z)
	{
	}

	/**************************************************************************/
	/*!
		3D vector add and assign operator
	*/
	/**************************************************************************/
	Vector3D& Vector3D::operator+=(const Vector3D& rhs)
	{
		_x += rhs._x;
		_y += rhs._y;
		return *this;
	}

	/**************************************************************************/
	/*!
		3D vector sub and assign operator
	*/
	/**************************************************************************/
	Vector3D& Vector3D::operator-=(const Vector3D& rhs)
	{
		_x -= rhs._x;
		_y -= rhs._y;
		return *this;
	}

	/**************************************************************************/
	/*!
		3D vector multiply with float adn assign operator
	*/
	/**************************************************************************/
	Vector3D& Vector3D::operator*=(float rhs)
	{
		_x *= rhs;
		_y *= rhs;
		return *this;
	}

	/**************************************************************************/
	/*!
		3D vector divide with float adn assign operator
	*/
	/**************************************************************************/
	Vector3D& Vector3D::operator/=(float rhs)
	{
		_x /= rhs;
		_y /= rhs;
		return *this;
	}

	float& Vector3D::operator[](int i)
	{
		return m[i];
	}

	/**************************************************************************/
	/*!
		3D vector unary minus operator
	*/
	/**************************************************************************/
	Vector3D Vector3D::operator-() const
	{
		return Vector3D(-_x, -_y, -_z);
	}

	/**************************************************************************/
	/*!
		3D vector length
	*/
	/**************************************************************************/
	float Vector3D::length()
	{
		return Vector3DLength(*this);
	}


	//conversion operator is case we are using open gl, this converts my vector to glm vectors
	//Vector3D::operator glm::vec2() const
	//{
	//	return glm::vec2(x, y);
	//}

	//Vector3D::operator glm::vec3() const
	//{
	//	return glm::vec3(x, y, z);
	//}

	//Vector3D::operator glm::vec4() const
	//{
	//	return glm::vec4(x, y, z, 1.0);
	//}



	/**************************************************************************/
	/*!
		3D vector addition operator
	*/
	/**************************************************************************/
	Vector3D operator+(const Vector3D& lhs, const Vector3D& rhs)
	{
		return Vector3D(lhs._x + rhs._x, lhs._y + rhs._y, lhs._z + rhs._z);
	}

	/**************************************************************************/
	/*!
		3D vector subtraction operator
	*
	/**************************************************************************/
	Vector3D operator-(const Vector3D& lhs, const Vector3D& rhs)
	{
		return Vector3D(lhs._x - rhs._x, lhs._y - rhs._y, lhs._z - rhs._z);
	}

	/**************************************************************************/
	/*!
		3D vector multiplication with float operator
	*/
	/**************************************************************************/
	Vector3D operator*(const Vector3D& lhs, float rhs)
	{
		return Vector3D(lhs._x * rhs, lhs._y * rhs , lhs._z * rhs);
	}

	Vector3D operator*(const Vector3D& lhs, const Vector3D& rhs)
	{
		return Vector3D(lhs._x * rhs._x, lhs._y * rhs._y, lhs._z * rhs._z);
	}

	/**************************************************************************/
	/*!
		3D vector multiplication with float operator
	*/
	/**************************************************************************/
	Vector3D operator*(float lhs, const Vector3D& rhs)
	{
		return rhs * lhs;
	}

	/**************************************************************************/
	/*!
		3D vector division with float operator
	*/
	/**************************************************************************/
	Vector3D operator/(const Vector3D& lhs, float rhs)
	{
		return Vector3D(lhs._x / rhs, lhs._y / rhs, lhs._z / rhs);
	}

	/**************************************************************************/
	/*!
		In this function, pResult will be the unit vector of pVec0
	*/
	/**************************************************************************/
	void Vector3DNormalize(Vector3D& pResult, const Vector3D& pVec0)
	{
		// find magnitude
		float m = inverseSqrt(pVec0._x * pVec0._x + pVec0._y * pVec0._y + pVec0._z * pVec0._z);
		// get unit vector
		pResult._x = pVec0._x * m;
		pResult._y = pVec0._y * m;
		pResult._z = pVec0._z * m;
	}

	/**************************************************************************/
	/*!
		This function returns the length of the vector pVec0
	*/
	/**************************************************************************/
	float Vector3DLength(const Vector3D& pVec0)
	{
		return sqrt(Vector3DSquareLength(pVec0));
	}

	/**************************************************************************/
	/*!
		This function returns the square of pVec0's length. Avoid the square root
	*/
	/**************************************************************************/
	float Vector3DSquareLength(const Vector3D& pVec0)
	{
		return pVec0._x * pVec0._x + pVec0._y * pVec0._y + pVec0._z * pVec0._z;
	}

	/**************************************************************************/
	/*!
		In this function, pVec0 and pVec1 are considered as 3D points.
		The distance between these 2 3D points is returned
	*/
	/**************************************************************************/
	float Vector3DDistance(const Vector3D& pVec0, const Vector3D& pVec1)
	{
		return Vector3DLength(pVec1 - pVec0);
	}

	/**************************************************************************/
	/*!
		In this function, pVec0 and pVec1 are considered as 3D points.
		The squared distance between these 2 3D points is returned.
		Avoid the square root
	*/
	/**************************************************************************/
	float Vector3DSquareDistance(const Vector3D& pVec0, const Vector3D& pVec1)
	{
		return Vector3DSquareLength(pVec1 - pVec0);
	}

	/**************************************************************************/
	/*!
		This function returns the dot product between pVec0 and pVec1
	*/
	/**************************************************************************/
	float Vector3DDotProduct(const Vector3D& pVec0, const Vector3D& pVec1)
	{
		return pVec0._x * pVec1._x + pVec0._y * pVec1._y + pVec0._z * pVec1._z;
	}

	/**************************************************************************/
	/*!
		This function returns the cross product between pVec0 and pVec1
	*/
	/**************************************************************************/
	float Vector3DCrossProduct(const Vector3D& pVec0, const Vector3D& pVec1)
	{
		return pVec0._x * pVec1._y - pVec0._y * pVec1._x;
	}

	/**************************************************************************/
	/*!
		This function projects Vector1 onto Vector0
	*/
	/**************************************************************************/
	Vector3D Vector3DProjection(const Vector3D& pVec0, const Vector3D& pVec1)
	{
		Vector3D normVec0;
		Vector3DNormalize(normVec0, pVec0);
		Vector3D resultVector = normVec0 * Vector3DDotProduct(normVec0, pVec1);
		return resultVector;
	}

	/**************************************************************************/
	/*!
		This function tells if two vectors are intersecting
	*/
	/**************************************************************************/
	bool Vector3DIntersection(const Vector3D& p, const Vector3D& r, const Vector3D& q, const Vector3D& s)
	{
		//the instersection happens when (p + t r) × s = (q + u s) × s

		//And since s x s = 0, this means
		//t(r × s) = (q − p) × s

		//And therefore, solving for t:
		//t = (q − p) × s / (r × s)

		//find t
		float t = Vector3DCrossProduct(q - p, s) / Vector3DCrossProduct(r, s);

		//In the same way, we can solve for u:

		//(p + t r) × r = (q + u s) × r

		//	u(s × r) = (p − q) × r

		//	u = (p − q) × r / (s × r)

		float u = Vector3DCrossProduct(p - q, r) / Vector3DCrossProduct(s, r);

		//Now there are four cases :
		//If r × s = 0 and (q − p) × r = 0, then the two lines are collinear.
		if ((Vector3DCrossProduct(r, s) == 0) && (Vector3DCrossProduct(q - p, r) == 0))
		{
			//In this case, express the endpoints of the second segment(q and q + s) in terms of the equation of the first line segment(p + t r):
			
			float t0 = Vector3DDotProduct(q - p, r) / Vector3DDotProduct(r, r);

			//t1 = (q + s − p) · r / (r · r) = t0 + s · r / (r · r)
			float t1 = t0 + Vector3DDotProduct(s, r) / Vector3DDotProduct(r, r);

			//If the interval between t0and t1 intersects the interval[0, 1] then the line segments are collinearand overlapping; otherwise they are collinearand disjoint.
			if ((0 <= t0 && t0 <= 1) ||
				(0 <= t1 && t1 <= 1))
			{
				return true;
			}
		}

		//r × s = 0 and (q − p) × r ≠ 0, then the two lines are parallel and non-intersecting.
		if ((Vector3DCrossProduct(r, s) == 0) && (Vector3DCrossProduct(q - p, r) != 0))
		{
			return false;
		}

		//If r × s ≠ 0 and 0 ≤ t ≤ 1 and 0 ≤ u ≤ 1, the two line segments meet at the point p + t r = q + u s.
		if ((Vector3DCrossProduct(r, s) != 0) &&
			(0 <= t && t <= 1) &&
			(0 <= u && u <= 1))
		{
			return true;
		}

		return false;
	}

	float inverseSqrt(float number)
	{
		long i;
		float x2, _y;
		const float threehalfs = 1.5F;

		x2 = number * 0.5F;
		_y = number;
		i = *(long*)&_y;                       // evil floating point bit level hacking
		i = 0x5f3759df - (i >> 1);               // what the fuck? 
		_y = *(float*)&i;
		_y = _y * (threehalfs - (x2 * _y * _y));   // 1st iteration
	//	y  = y * ( threehalfs - ( x2 * y * y ) );   // 2nd iteration, this can be removed

		return _y;
	}
}