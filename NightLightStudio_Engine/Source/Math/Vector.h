#pragma once

#include <cstdint>
#include <cmath>

#include "../glm/glm.hpp"   
namespace NlMath
{
#ifdef _MSC_VER
	// Supress warning: nonstandard extension used : nameless struct/union
#pragma warning( disable : 4201 )
#endif
	typedef union Vector3D
	{
		struct
		{
			float x, y, z;
		};
	
		float m[3];
	
		// Constructors
		Vector3D();
		Vector3D(float set);
		Vector3D(float x, float y, float z);

		// Copy constructors
		Vector3D(const Vector3D& rhs);

		// Assignment operators
		Vector3D& operator += (const Vector3D& rhs);
		Vector3D& operator -= (const Vector3D& rhs);
		Vector3D& operator *= (float rhs);
		Vector3D& operator /= (float rhs);
		float& operator[](int i);
		// Unary operators
		Vector3D operator -() const;

		/*conversion operator is case we are using open gl, this converts my vector to glm vectors*/
		Vector3D& operator = (const glm::vec3& rhs);
		operator glm::vec2() const;
		operator glm::vec3() const;
		operator glm::vec4() const;
		
		float length();
		void set(float _x, float _y, float _z);
		Vector3D abs() const;
	} Vector3D, Vec3, Point3D, Pt3;
	
	
	#ifdef _MSC_VER
	// Supress warning: nonstandard extension used : nameless struct/union
	#pragma warning( default : 4201 )
	#endif
	
	// Binary operators
	Vector3D operator + (const Vector3D& lhs, const Vector3D& rhs);
	Vector3D operator - (const Vector3D& lhs, const Vector3D& rhs);
	Vector3D operator * (const Vector3D& lhs, float rhs);
	Vector3D operator * (float lhs, const Vector3D& rhs);
	float operator * (const Vector3D& lhs, const Vector3D& rhs);
	Vector3D operator / (const Vector3D& lhs, float rhs);
	

	/**************************************************************************/
	/*!
		In this function, the return value is the mid point of 
		point pVec0 and point pVec1
	 */
	 /**************************************************************************/
	Vector3D	Vector3DMidPoint(const Vector3D& pVec0, const Vector3D& pVec1);

	/**************************************************************************/
	/*!
		In this function, function will return the unit vector of pVec0
	 */
	 /**************************************************************************/
	Vector3D	Vector3DNormalize(const Vector3D& pVec0);
	
	/**************************************************************************/
	/*!
		This function returns the length of the vector pVec0
	 */
	 /**************************************************************************/
	float	Vector3DLength(const Vector3D& pVec0);
	
	/**************************************************************************/
	/*!
		This function returns the square of pVec0's length. Avoid the square root
	 */
	 /**************************************************************************/
	float	Vector3DSquareLength(const Vector3D& pVec0);
	
	/**************************************************************************/
	/*!
		In this function, pVec0 and pVec1 are considered as 2D points.
		The distance between these 2 2D points is returned
	 */
	 /**************************************************************************/
	float	Vector3DDistance(const Vector3D& pVec0, const Vector3D& pVec1);
	
	/**************************************************************************/
	/*!
		In this function, pVec0 and pVec1 are considered as 2D points.
		The squared distance between these 2 2D points is returned.
		Avoid the square root
	 */
	 /**************************************************************************/
	float	Vector3DSquareDistance(const Vector3D& pVec0, const Vector3D& pVec1);
	
	/**************************************************************************/
	/*!
		This function returns the dot product between pVec0 and pVec1
	 */
	 /**************************************************************************/
	float	Vector3DDotProduct(const Vector3D& pVec0, const Vector3D& pVec1);
	
	/**************************************************************************/
	/*!
		This function returns the cross product magnitude
		between pVec0 and pVec1
	 */
	 /**************************************************************************/
	Vector3D	Vector3DCrossProduct(const Vector3D& pVec0, const Vector3D& pVec1);

	/**************************************************************************/
	/*!
		This function projects Vector1 onto Vector0
	*/
	/**************************************************************************/
	Vector3D	Vector3DProjection(const Vector3D& pVec0, const Vector3D& pVec1);
}
	
	