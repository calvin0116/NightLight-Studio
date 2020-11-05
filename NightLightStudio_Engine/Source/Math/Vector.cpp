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
	Vector3D::Vector3D() :x{ 0 }, y{ 0 }, z{ 0 }
	{
	}

	Vector3D::Vector3D(float set) : x{ set }, y{ set }, z{ set }
	{
	}


	/**************************************************************************/
	/*!
		3D vector ctor from 2 floats
	*/
	/**************************************************************************/
	Vector3D::Vector3D(float x, float y, float z) : x(x), y(y), z(z)
	{
	}

	/**************************************************************************/
	/*!
		Copy constructor
	*/
	/**************************************************************************/
	Vector3D::Vector3D(const Vector3D& rhs) : x(rhs.x), y(rhs.y), z(rhs.z)
	{
	}

	/**************************************************************************/
	/*!
		3D vector add and assign operator
	*/
	/**************************************************************************/
	Vector3D& Vector3D::operator+=(const Vector3D& rhs)
	{
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		return *this;
	}

	/**************************************************************************/
	/*!
		3D vector sub and assign operator
	*/
	/**************************************************************************/
	Vector3D& Vector3D::operator-=(const Vector3D& rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;
		return *this;
	}

	/**************************************************************************/
	/*!
		3D vector multiply with float adn assign operator
	*/
	/**************************************************************************/
	Vector3D& Vector3D::operator*=(float rhs)
	{
		x *= rhs;
		y *= rhs;
		z *= rhs;
		return *this;
	}

	/**************************************************************************/
	/*!
		3D vector divide with float adn assign operator
	*/
	/**************************************************************************/
	Vector3D& Vector3D::operator/=(float rhs)
	{
		x /= rhs;
		y /= rhs;
		z /= rhs;
		return *this;
	}

	float& Vector3D::operator[](int i)
	{
		return m[i];
	}

	bool Vector3D::operator==(const Vector3D& rhs)
	{
		return (x == rhs.x) && (y == rhs.y) && (z == rhs.z);
	}

	/**************************************************************************/
	/*!
		3D vector unary minus operator
	*/
	/**************************************************************************/
	Vector3D Vector3D::operator-() const
	{
		return Vector3D(-x, -y, -z);
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

	void Vector3D::set(float _x, float _y, float _z)
	{
		x = _x;
		y = _y;
		z = _z;
	}

	Vector3D Vector3D::abs() const
	{
		return Vector3D(fabs(x), fabs(y), fabs(z));
	}


	Vector3D& Vector3D::operator=(const glm::vec3& rhs)
	{
		x = rhs.x;
		y = rhs.y;
		z = rhs.z;

		return *this;
		// TODO: insert return statement here
	}

	//conversion operator is case we are using open gl, this converts my vector to glm vectors
	Vector3D::operator glm::vec2() const
	{
		return glm::vec2(x, y);
	}

	Vector3D::operator glm::vec3() const
	{
		return glm::vec3(x, y, z);
	}

	Vector3D::operator glm::vec4() const
	{
		return glm::vec4(x, y, z, 1.0);
	}



	/**************************************************************************/
	/*!
		3D vector addition operator
	*/
	/**************************************************************************/
	Vector3D operator+(const Vector3D& lhs, const Vector3D& rhs)
	{
		return Vector3D(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
	}

	/**************************************************************************/
	/*!
		3D vector subtraction operator
	*
	/**************************************************************************/
	Vector3D operator-(const Vector3D& lhs, const Vector3D& rhs)
	{
		return Vector3D(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
	}

	/**************************************************************************/
	/*!
		3D vector multiplication with float operator
	*/
	/**************************************************************************/
	Vector3D operator*(const Vector3D& lhs, float rhs)
	{
		return Vector3D(lhs.x * rhs, lhs.y * rhs , lhs.z * rhs);
	}

	Vector3D operator*(float lhs, const Vector3D& rhs)
	{
		return rhs * lhs;
	}
	/**************************************************************************/
	/*!
		3D vector Dot Product
	*/
	/**************************************************************************/
	float operator*(const Vector3D& lhs, const Vector3D& rhs)
	{
		return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
	}

	/**************************************************************************/
	/*!
		3D vector division with float operator
	*/
	/**************************************************************************/
	Vector3D operator/(const Vector3D& lhs, float rhs)
	{
		return Vector3D(lhs.x / rhs, lhs.y / rhs, lhs.z / rhs);
	}
	/**************************************************************************/
	/*!
		In this function, the return value is the mid point of
		point pVec0 and point pVec1
	 */
	 /**************************************************************************/
	Vector3D Vector3DMidPoint(const Vector3D& pVec0, const Vector3D& pVec1)
	{
		Vector3D minpoint = (pVec0 + pVec1) / 2;
		return minpoint;
	}

	/**************************************************************************/
	/*!
		In this function, pResult will be the unit vector of pVec0
	*/
	/**************************************************************************/
	Vector3D Vector3DNormalize(const Vector3D& pVec0)
	{
		Vector3D pResult;
		// find magnitude
		float m = sqrt(pVec0.x * pVec0.x + pVec0.y * pVec0.y + pVec0.z * pVec0.z);
		// get unit vector
		pResult.x = pVec0.x / m;
		pResult.y = pVec0.y / m;
		pResult.z = pVec0.z / m;
		return pResult;
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
		return pVec0.x * pVec0.x + pVec0.y * pVec0.y + pVec0.z * pVec0.z;
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
		return pVec0.x * pVec1.x + pVec0.y * pVec1.y + pVec0.z * pVec1.z;
	}

	/**************************************************************************/
	/*!
		This function returns the cross product between pVec0 and pVec1
	*/
	/**************************************************************************/
	Vector3D Vector3DCrossProduct(const Vector3D& pVec0, const Vector3D& pVec1)
	{
		return Vector3D(
			pVec0.y * pVec1.z - pVec0.z * pVec1.y, 
			pVec0.z * pVec1.x - pVec0.x * pVec1.z, 
			pVec0.x * pVec1.y - pVec0.y * pVec1.x);
	}

	/**************************************************************************/
	/*!
		This function projects Vector1 onto Vector0
	*/
	/**************************************************************************/
	Vector3D Vector3DProjection(const Vector3D& pVec0, const Vector3D& pVec1)
	{
		Vector3D normVec0 = Vector3DNormalize(pVec0);
		Vector3D resultVector = normVec0 * Vector3DDotProduct(normVec0, pVec1);
		return resultVector;
	}
}