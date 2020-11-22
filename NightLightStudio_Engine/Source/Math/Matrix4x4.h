#pragma once
#include "Vector.h"
#include <iostream>

#include "Vector.h"
#include "../glm/glm.hpp"   
namespace NlMath
{
#ifdef _MSC_VER
	// Supress warning: nonstandard extension used : nameless struct/union
#pragma warning( disable : 4201 )
#endif

	//predefine
	union Vector3D;
/**************************************************************************/
/*!
	An abstraction of 3x3 matrix
 */
 /**************************************************************************/
	typedef union Matrix4x4
	{
		struct
		{
			float m00, m01, m02, m03;
			float m10, m11, m12, m13;
			float m20, m21, m22, m23;
			float m30, m31, m32, m33;
		};

		float m[16];
		float m2[4][4];

		Matrix4x4();
		Matrix4x4(const float _x);
		Matrix4x4(const float* pArr);
		Matrix4x4(	float _00, float _01, float _02, float _03,
					float _10, float _11, float _12, float _13,
					float _20, float _21, float _22, float _23,
					float _30, float _31, float _32, float _33);
		Matrix4x4& operator=(const Matrix4x4& rhs);

		// Assignment operators
		Matrix4x4& operator *= (const Matrix4x4& rhs);

		float& operator[](int i);

		float operator[](int i) const;

		float determinant() const;

		Matrix4x4 cofactor() const;

		friend std::ostream& operator<<(std::ostream& os, const Matrix4x4 mtx);

		//conversion operator is case we are using open gl, this converts my matrix to glm matrix
		Matrix4x4& operator=(const glm::mat4x4& rhs);
		operator glm::mat4x4() const;
		operator glm::mat3x3() const;

		Vector3D Column0() const;
		Vector3D Column1() const;
		Vector3D Column2() const;

		Vector3D Row0() const;
		Vector3D Row1() const;
		Vector3D Row2() const;

		void SetRows(const Vec3 x, const Vec3 y, const Vec3 z);
		void SetColumns(const Vec3 x, const Vec3 y, const Vec3 z);
		void SetDiagonal(const Vec3& x);

	} Matrix4x4, Matrix4x4, Mtx44;

	const Vec3 MulT(const Matrix4x4& r, const Vec3& v);

	Matrix4x4 operator * (const Matrix4x4& lhs, const Matrix4x4& rhs);

	/**************************************************************************/
	/*!
		This operator multiplies the matrix pMtx with the vector rhs
		and returns the result as a vector
	 */
	 /**************************************************************************/
	Vector3D  operator * (const Matrix4x4& pMtx, const Vector3D& rhs);

	/**************************************************************************/
	/*!
		This function sets the matrix pResult to the identity matrix
	 */
	 /**************************************************************************/
	void Mtx44Identity(Matrix4x4& pResult);

	/**************************************************************************/
	/*!
		This function creates a translation matrix from x & y
		and saves it in pResult
	 */
	 /**************************************************************************/
	void Mtx44Translate(Matrix4x4& pResult, float _x, float _y, float _z);

	/**************************************************************************/
	/*!
		This function creates a scaling matrix from x & y
		and saves it in pResult
	 */
	 /**************************************************************************/
	void Mtx44Scale(Matrix4x4& pResult, float _x, float _y, float _z);

	/**************************************************************************/
	/*!
		This matrix creates a x - axis rotation matrix from "angle" whose value
		is in radian. Save the resultant matrix in pResult.
	 */
	 /**************************************************************************/
	void Mtx44RotXRad(Matrix4x4& pResult, float angle);

	/**************************************************************************/
	/*!
		This matrix creates a y - axis rotation matrix from "angle" whose value
		is in radian. Save the resultant matrix in pResult.
	 */
	 /**************************************************************************/
	void Mtx44RotYRad(Matrix4x4& pResult, float angle);
	
	/**************************************************************************/
	/*!
		This matrix creates a z - axis rotation matrix from "angle" whose value
		is in radian. Save the resultant matrix in pResult.
	 */
	 /**************************************************************************/
	void Mtx44RotZRad(Matrix4x4& pResult, float angle);

	/**************************************************************************/
	/*!
		This matrix creates a rotation matrix from an "angle" vector whose value
		is in radian. Save the resultant matrix in pResult.
	 */
	 /**************************************************************************/
	void Mtx44RotRad(Matrix4x4& pResult, const Vector3D& angle);


	/**************************************************************************/
	/*!
		This matrix creates a x - axis rotation matrix from "angle" whose value
		is in degree. Save the resultant matrix in pResult.
	 */
	 /**************************************************************************/
	void Mtx44RotXDeg(Matrix4x4& pResult, float angle);

	/**************************************************************************/
	/*!
		This matrix creates a y - axis rotation matrix from "angle" whose value
		is in degree. Save the resultant matrix in pResult.
	 */
	 /**************************************************************************/
	void Mtx44RotYDeg(Matrix4x4& pResult, float angle);

	/**************************************************************************/
	/*!
		This matrix creates a z - axis rotation matrix from "angle" whose value
		is in degree. Save the resultant matrix in pResult.
	 */
	 /**************************************************************************/
	void Mtx44RotZDeg(Matrix4x4& pResult, float angle);

	/**************************************************************************/
	/*!
		This functions calculated the transpose matrix of pMtx
		and saves it in pResult
	 */
	 /**************************************************************************/
	Matrix4x4 Mtx44Transpose(const Matrix4x4& pMtx);

	/**************************************************************************/
	/*!
		This function calculates the inverse matrix of pMtx and saves the
		result in pResult. If the matrix inversion fails, pResult
		would be set to NULL.
	*/
	/**************************************************************************/
	void Mtx44Inverse(Matrix4x4& pResult, const Matrix4x4& pMtx);

	/**************************************************************************/
	/*!
		This function returns the tensor matrix
	*/
	/**************************************************************************/
	Matrix4x4	Mtx44OuterProduct(const Vector3D& pVec0, const Vector3D& pVec1);
}