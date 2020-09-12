#define _USE_MATH_DEFINES

#include <math.h>
#include "Vector.h"
#include "Matrix4x4.h"


namespace NlMath
{
	/**************************************************************************/
	/*!
		default constructor
	*/
	/**************************************************************************/
	Matrix4x4::Matrix4x4() :	m00{ 0 }, m01{ 0 }, m02{ 0 }, m03{ 0 } ,
								m10{ 0 }, m11{ 0 }, m12{ 0 }, m13{ 0 } ,
								m20{ 0 }, m21{ 0 }, m22{ 0 }, m23{ 0 } ,
								m30{ 0 }, m31{ 0 }, m32{ 0 }, m33{ 0 }
	{
	}
	/**************************************************************************/
	/*!
		assignment constructor
		construct a diagonal matrix with a float
	*/
	/**************************************************************************/

	Matrix4x4::Matrix4x4(float x):	m00 { x }, m01{ 0 }, m02{ 0 }, m03{ 0 },
									m10{ 0 }, m11{ x }, m12{ 0 }, m13{ 0 },
									m20{ 0 }, m21{ 0 }, m22{ x }, m23{ 0 },
									m30{ 0 }, m31{ 0 }, m32{ 0 }, m33{ x }
	{
	}

	/**************************************************************************/
	/*!
		4x4 matrix ctor from arr of floats
	*/
	/**************************************************************************/
	Matrix4x4::Matrix4x4(const float* pArr):	m00{ 0 }, m01{ 0 }, m02{ 0 }, m03{ 0 },
												m10{ 0 }, m11{ 0 }, m12{ 0 }, m13{ 0 },
												m20{ 0 }, m21{ 0 }, m22{ 0 }, m23{ 0 },
												m30{ 0 }, m31{ 0 }, m32{ 0 }, m33{ 0 }
	{
		for (int i = 0; i < 16; ++i, ++pArr)
		{
			m[i] = *pArr;
		}
	}

	/**************************************************************************/
	/*!
		4x4 matrix ctor from 9 floats
	*/
	/**************************************************************************/
	Matrix4x4::Matrix4x4(float _00, float _01, float _02, float _03,
		float _10, float _11, float _12, float _13,
		float _20, float _21, float _22, float _23,
		float _30, float _31, float _32, float _33)
	{
		//first row
		m00 = _00;
		m01 = _01;
		m02 = _02;
		m03 = _03;
		//2nd row
		m10 = _10;
		m11 = _11;
		m12 = _12;
		m13 = _13;
		//3rd row
		m20 = _20;
		m21 = _21;
		m22 = _22;
		m23 = _23;
		//4th row
		m30 = _30;
		m31 = _31;
		m32 = _32;
		m33 = _33;
	}

	/**************************************************************************/
	/*!
		4x4 matrix assignment operator
	*/
	/**************************************************************************/
	Matrix4x4& Matrix4x4::operator=(const Matrix4x4& rhs)
	{
		for (int i = 0; i < 16; ++i)
		{
			m[i] = rhs.m[i];
		}
		return *this;
	}

	/**************************************************************************/
	/*!
		Multiplies 2 3x3 matrices and assign to a 4x4 matrix
	*/
	/**************************************************************************/
	Matrix4x4& Matrix4x4::operator*=(const Matrix4x4& rhs)
	{
		return *this = *this * rhs;
	}

	/**************************************************************************/
	/*!
		Index Operator, return an element from index
	*/
	/**************************************************************************/

	float& Matrix4x4::operator[](int i)
	{
		return m[i];
	}

	float Matrix4x4::operator[](int i) const
	{
		return m[i];
	}

	/**************************************************************************/
	/*!
		Returns the determinant of the 4x4 matrix
	*/
	/**************************************************************************/

	float Matrix4x4::determinant() const
	{
		return
			m03 * m12 * m21 * m30 - m02 * m13 * m21 * m30 -
			m03 * m11 * m22 * m30 + m01 * m13 * m22 * m30 +
			m02 * m11 * m23 * m30 - m01 * m12 * m23 * m30 -
			m03 * m12 * m20 * m31 + m02 * m13 * m20 * m31 +
			m03 * m10 * m22 * m31 - m00 * m13 * m22 * m31 -
			m02 * m10 * m23 * m31 + m00 * m12 * m23 * m31 +
			m03 * m11 * m20 * m32 - m01 * m13 * m20 * m32 -
			m03 * m10 * m21 * m32 + m00 * m13 * m21 * m32 +
			m01 * m10 * m23 * m32 - m00 * m11 * m23 * m32 -
			m02 * m11 * m20 * m33 + m01 * m12 * m20 * m33 +
			m02 * m10 * m21 * m33 - m00 * m12 * m21 * m33 -
			m01 * m10 * m22 * m33 + m00 * m11 * m22 * m33;
	}

	//Matrix4x4::operator glm::mat4x4() const
	//{
	//	glm::mat4x4 tmp;
	//	for (glm::mat4x4::length_type i = 0; i < 4; i++)
	//	{
	//		for (glm::mat4x4::length_type j = 0; j < 4; j++)
	//		{
	//			tmp[i][j] = m2[i][j];
	//		}
	//	}
	
	//	return tmp;
	//}
	
	//Matrix4x4::operator glm::mat3x3() const
	//{
	//	glm::mat3x3 tmp{ m00,m01,m02,m10,m11,m12,m20,m21,m22 };
	//	return tmp;
	//}

	/**************************************************************************/
	/*!
		Multiplies 2 4x4 matrices
	*/
	/**************************************************************************/
	Matrix4x4 operator*(const Matrix4x4& lhs, const Matrix4x4& rhs)
	{
		Matrix4x4 returnMatrix;
		// 1st row
		returnMatrix.m00 = lhs.m00 * rhs.m00 + lhs.m01 * rhs.m10 + lhs.m02 * rhs.m20 + lhs.m03 * rhs.m30;
		returnMatrix.m01 = lhs.m00 * rhs.m01 + lhs.m01 * rhs.m11 + lhs.m02 * rhs.m21 + lhs.m03 * rhs.m31;
		returnMatrix.m02 = lhs.m00 * rhs.m02 + lhs.m01 * rhs.m12 + lhs.m02 * rhs.m22 + lhs.m03 * rhs.m32;
		returnMatrix.m03 = lhs.m00 * rhs.m03 + lhs.m01 * rhs.m13 + lhs.m02 * rhs.m23 + lhs.m03 * rhs.m33;
		// 2nd row
		returnMatrix.m10 = lhs.m10 * rhs.m00 + lhs.m11 * rhs.m10 + lhs.m12 * rhs.m20 + lhs.m13 * rhs.m30;
		returnMatrix.m11 = lhs.m10 * rhs.m01 + lhs.m11 * rhs.m11 + lhs.m12 * rhs.m21 + lhs.m13 * rhs.m31;
		returnMatrix.m12 = lhs.m10 * rhs.m02 + lhs.m11 * rhs.m12 + lhs.m12 * rhs.m22 + lhs.m13 * rhs.m32;
		returnMatrix.m13 = lhs.m10 * rhs.m03 + lhs.m11 * rhs.m13 + lhs.m12 * rhs.m23 + lhs.m13 * rhs.m32;
		// 3rd row
		returnMatrix.m20 = lhs.m20 * rhs.m00 + lhs.m21 * rhs.m10 + lhs.m22 * rhs.m20 + lhs.m23 * rhs.m30;
		returnMatrix.m21 = lhs.m20 * rhs.m01 + lhs.m21 * rhs.m11 + lhs.m22 * rhs.m21 + lhs.m23 * rhs.m31;
		returnMatrix.m22 = lhs.m20 * rhs.m02 + lhs.m21 * rhs.m12 + lhs.m22 * rhs.m22 + lhs.m23 * rhs.m32;
		returnMatrix.m23 = lhs.m20 * rhs.m03 + lhs.m21 * rhs.m13 + lhs.m22 * rhs.m23 + lhs.m23 * rhs.m33;
		//4th row
		returnMatrix.m30 = lhs.m30 * rhs.m00 + lhs.m31 * rhs.m10 + lhs.m32 * rhs.m20 + lhs.m33 * rhs.m30;
		returnMatrix.m31 = lhs.m30 * rhs.m01 + lhs.m31 * rhs.m11 + lhs.m32 * rhs.m21 + lhs.m33 * rhs.m31;
		returnMatrix.m32 = lhs.m30 * rhs.m02 + lhs.m31 * rhs.m12 + lhs.m32 * rhs.m22 + lhs.m33 * rhs.m32;
		returnMatrix.m33 = lhs.m30 * rhs.m03 + lhs.m31 * rhs.m13 + lhs.m32 * rhs.m23 + lhs.m33 * rhs.m33;

		return returnMatrix;
	}

	/**************************************************************************/
	/*!
		This operator multiplies the matrix pMtx with the vector rhs
		and returns the result as a vector
	*/
	/**************************************************************************/
	Vector3D operator*(const Matrix4x4& pMtx, const Vector3D& rhs)
	{
		Vector3D returnVector;
		returnVector.x = pMtx.m00 * rhs.x + pMtx.m01 * rhs.y + pMtx.m02 * rhs.z;
		returnVector.y = pMtx.m10 * rhs.x + pMtx.m11 * rhs.y + pMtx.m12 * rhs.z;
		returnVector.z = pMtx.m20 * rhs.x + pMtx.m21 * rhs.y + pMtx.m22 * rhs.z;
		return returnVector;
	}

	/**************************************************************************/
	/*!
		This function sets the matrix pResult to the identity matrix
	*/
	/**************************************************************************/
	void Mtx44Identity(Matrix4x4& pResult)
	{
		// 1st row
		pResult.m00 = 1;
		pResult.m01 = 0;
		pResult.m02 = 0;
		pResult.m03 = 0;
		// 2nd row
		pResult.m10 = 0;
		pResult.m11 = 1;
		pResult.m12 = 0;
		pResult.m13 = 0;
		// 3rd row
		pResult.m20 = 0;
		pResult.m21 = 0;
		pResult.m22 = 1;
		pResult.m23 = 0;
		// 4th row
		pResult.m30 = 0;
		pResult.m31 = 0;
		pResult.m32 = 0;
		pResult.m33 = 1;
	}

	/**************************************************************************/
	/*!
		This function creates a translation matrix from x & y
		and saves it in pResult
	*/
	/**************************************************************************/
	void Mtx44Translate(Matrix4x4& pResult, float x, float y, float z)
	{
		// 1st row
		pResult.m00 = 1;
		pResult.m01 = 0;
		pResult.m02 = 0;
		pResult.m03 = x;
		// 2nd row
		pResult.m10 = 0;
		pResult.m11 = 1;
		pResult.m12 = 0;
		pResult.m13 = y;
		// 3rd row
		pResult.m20 = 0;
		pResult.m21 = 0;
		pResult.m22 = 1;
		pResult.m23 = z;
		// 4th row
		pResult.m30 = 0;
		pResult.m31 = 0;
		pResult.m32 = 0;
		pResult.m33 = 1;
	}

	/**************************************************************************/
	/*!
		This function creates a scaling matrix from x & y
		and saves it in pResult
	*/
	/**************************************************************************/
	void Mtx44Scale(Matrix4x4& pResult, float x, float y, float z)
	{
		// 1st row
		pResult.m00 = x;
		pResult.m01 = 0;
		pResult.m02 = 0;
		pResult.m03 = 0;
		// 2nd row
		pResult.m10 = 0;
		pResult.m11 = y;
		pResult.m12 = 0;
		pResult.m13 = 0;
		// 3rd row
		pResult.m20 = 0;
		pResult.m21 = 0;
		pResult.m22 = z;
		pResult.m23 = 0;
		// 4th row
		pResult.m30 = 0;
		pResult.m31 = 0;
		pResult.m32 = 0;
		pResult.m33 = 1;
	}

	/**************************************************************************/
	/*!
		This matrix creates a X - axis rotation matrix from "angle" whose value
		is in radian. Save the resultant matrix in pResult.
	*/
	/**************************************************************************/
	void Mtx44RotXRad(Matrix4x4& pResult, float angle)
	{
		float cosAngle = cos(angle);
		float sinAngle = sin(angle);
		// 1st row
		pResult.m00 = 1;
		pResult.m01 = 0;
		pResult.m02 = 0;
		pResult.m03 = 0;
		// 2nd row
		pResult.m10 = 0;
		pResult.m11 = cosAngle;
		pResult.m12 = sinAngle;
		pResult.m13 = 0;
		// 3rd row
		pResult.m20 = 0;
		pResult.m21 = -sinAngle;
		pResult.m22 = cosAngle;
		pResult.m23 = 0;
		// 4th row
		pResult.m30 = 0;
		pResult.m31 = 0;
		pResult.m32 = 0;
		pResult.m33 = 1;
	}

	/**************************************************************************/
/*!
	This matrix creates a Y - axis rotation matrix from "angle" whose value
	is in radian. Save the resultant matrix in pResult.
*/
/**************************************************************************/
	void Mtx44RotYRad(Matrix4x4& pResult, float angle)
	{
		float cosAngle = cos(angle);
		float sinAngle = sin(angle);
		// 1st row
		pResult.m00 = cosAngle;
		pResult.m01 = 0;
		pResult.m02 = sinAngle;
		pResult.m03 = 0;
		// 2nd row
		pResult.m10 = 0;
		pResult.m11 = 1;
		pResult.m12 = 0;
		pResult.m13 = 0;
		// 3rd row
		pResult.m20 = -sinAngle;
		pResult.m21 = 0;
		pResult.m22 = cosAngle;
		pResult.m23 = 0;
		// 4th row
		pResult.m30 = 0;
		pResult.m31 = 0;
		pResult.m32 = 0;
		pResult.m33 = 1;
	}

	/**************************************************************************/
	/*!
		This matrix creates a Z - axis rotation matrix from "angle" whose value
		is in radian. Save the resultant matrix in pResult.
	*/
	/**************************************************************************/
	void Mtx44RotZRad(Matrix4x4& pResult, float angle)
	{
		float cosAngle = cos(angle);
		float sinAngle = sin(angle);
		// 1st row
		pResult.m00 = cosAngle;
		pResult.m01 = -sinAngle;
		pResult.m02 = 0;
		pResult.m03 = 0;
		// 2nd row
		pResult.m10 = sinAngle;
		pResult.m11 = cosAngle;
		pResult.m12 = 0;
		pResult.m13 = 0;
		// 3rd row
		pResult.m20 = 0;
		pResult.m21 = 0;
		pResult.m22 = 1;
		pResult.m23 = 0;
		// 4th row
		pResult.m30 = 0;
		pResult.m31 = 0;
		pResult.m32 = 0;
		pResult.m33 = 1;
	}

	/**************************************************************************/
	/*!
		This matrix creates a X - axis rotation matrix from "angle" whose value
		is in degree. Save the resultant matrix in pResult.
	*/
	/**************************************************************************/
	void Mtx44RotXDeg(Matrix4x4& pResult, float angle)
	{
		float angleDeg = static_cast<float>(angle * M_PI / 180);
		Mtx44RotXRad(pResult, angleDeg);
	}

	/**************************************************************************/
	/*!
		This matrix creates a Y - axis rotation matrix from "angle" whose value
		is in degree. Save the resultant matrix in pResult.
	*/
	/**************************************************************************/
	void Mtx44RotYDeg(Matrix4x4& pResult, float angle)
	{
		float angleDeg = static_cast<float>(angle * M_PI / 180);
		Mtx44RotYRad(pResult, angleDeg);
	}

	/**************************************************************************/
	/*!
		This matrix creates a Z - axis rotation matrix from "angle" whose value
		is in degree. Save the resultant matrix in pResult.
	*/
	/**************************************************************************/
	void Mtx44RotZDeg(Matrix4x4& pResult, float angle)
	{
		float angleDeg = static_cast<float>(angle * M_PI / 180);
		Mtx44RotZRad(pResult, angleDeg);
	}

	/**************************************************************************/
	/*!
		This functions calculated the transpose matrix of pMtx
		and saves it in pResult
	*/
	/**************************************************************************/
	void Mtx44Transpose(Matrix4x4& pResult, const Matrix4x4& pMtx)
	{
		// 1st row
		pResult.m00 = pMtx.m00;
		pResult.m01 = pMtx.m10;
		pResult.m02 = pMtx.m20;
		pResult.m03 = pMtx.m30;
		// 2nd row
		pResult.m10 = pMtx.m01;
		pResult.m11 = pMtx.m11;
		pResult.m12 = pMtx.m21;
		pResult.m13 = pMtx.m31;
		// 3rd row
		pResult.m20 = pMtx.m02;
		pResult.m21 = pMtx.m12;
		pResult.m22 = pMtx.m22;
		pResult.m23 = pMtx.m32;
		// 4th row
		pResult.m30 = pMtx.m03;
		pResult.m31 = pMtx.m13;
		pResult.m32 = pMtx.m23;
		pResult.m33 = pMtx.m33;
	}

	/**************************************************************************/
	/*!
		This function calculates the inverse matrix of pMtx and saves the
		result in pResult. If the matrix inversion fails, pResult
		would be set to NULL.
	*/
	/**************************************************************************/
	void Mtx44Inverse(Matrix4x4* pResult, const Matrix4x4& pMtx)
	{
		// get determinant
		float det = pMtx.determinant();

		// check det
		if (det == 0)
		{
			pResult = nullptr;
			return;
		}

		//cofector matrix
		Matrix4x4 mtx;
		// 1st row
		mtx.m00 = pMtx.m11 * pMtx.m22 - pMtx.m12 * pMtx.m21;
		mtx.m01 = -(pMtx.m10 * pMtx.m22 - pMtx.m12 * pMtx.m20);
		mtx.m02 = pMtx.m10 * pMtx.m21 - pMtx.m11 * pMtx.m20;
		// 2nd row
		mtx.m10 = -(pMtx.m01 * pMtx.m22 - pMtx.m02 * pMtx.m21);
		mtx.m11 = pMtx.m00 * pMtx.m22 - pMtx.m02 * pMtx.m20;
		mtx.m12 = -(pMtx.m00 * pMtx.m21 - pMtx.m01 * pMtx.m20);
		// 3rd row
		mtx.m20 = pMtx.m01 * pMtx.m12 - pMtx.m02 * pMtx.m11;
		mtx.m21 = -(pMtx.m00 * pMtx.m12 - pMtx.m02 * pMtx.m10);
		mtx.m22 = pMtx.m00 * pMtx.m11 - pMtx.m01 * pMtx.m10;
		// transpose mtx
		Mtx44Transpose(*pResult, mtx);

		// 1/det
		det = 1.0f / det;

		// get final result
		// 1st row
		pResult->m00 = pResult->m00 * det;
		pResult->m01 = pResult->m01 * det;
		pResult->m02 = pResult->m02 * det;
		pResult->m03 = pResult->m03 * det;
		// 2nd row
		pResult->m10 = pResult->m10 * det;
		pResult->m11 = pResult->m11 * det;
		pResult->m12 = pResult->m12 * det;
		pResult->m13 = pResult->m13 * det;
		// 3rd row
		pResult->m20 = pResult->m20 * det;
		pResult->m21 = pResult->m21 * det;
		pResult->m22 = pResult->m22 * det;
		pResult->m23 = pResult->m23 * det;
	}

}

