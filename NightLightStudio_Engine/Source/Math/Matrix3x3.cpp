#define _USE_MATH_DEFINES

#include <math.h>
#include "Vector.h"
#include "Matrix3x3.h"


namespace NlMath
{
	/**************************************************************************/
	/*!
		default constructor
	*/
	/**************************************************************************/
	Matrix3x3::Matrix3x3() :	m00{ 0 }, m01{ 0 }, m02{ 0 },
								m10{ 0 }, m11{ 0 }, m12{ 0 },
								m20{ 0 }, m21{ 0 }, m22{ 0 }
	{
	}
	/**************************************************************************/
	/*!
		assignment constructor
		construct a diagonal matrix with a float
	*/
	/**************************************************************************/

	Matrix3x3::Matrix3x3(float x):	m00 { x }, m01{ 0 }, m02{ 0 },
									m10 { 0 }, m11{ x }, m12{ 0 },
									m20 { 0 }, m21{ 0 }, m22{ x }
	{
	}

	/**************************************************************************/
	/*!
		4x4 matrix ctor from arr of floats
	*/
	/**************************************************************************/
	Matrix3x3::Matrix3x3(const float* pArr):	m00{ 0 }, m01{ 0 }, m02{ 0 },
												m10{ 0 }, m11{ 0 }, m12{ 0 },
												m20{ 0 }, m21{ 0 }, m22{ 0 }
	{
		for (int i = 0; i < 9; ++i, ++pArr)
		{
			m[i] = *pArr;
		}
	}

	/**************************************************************************/
	/*!
		4x4 matrix ctor from 9 floats
	*/
	/**************************************************************************/
	Matrix3x3::Matrix3x3(float _00, float _01, float _02,
		float _10, float _11, float _12,
		float _20, float _21, float _22)
	{
		//first row
		m00 = _00;
		m01 = _01;
		m02 = _02;

		//2nd row
		m10 = _10;
		m11 = _11;
		m12 = _12;

		//3rd row
		m20 = _20;
		m21 = _21;
		m22 = _22;
	}

	/**************************************************************************/
	/*!
		4x4 matrix assignment operator
	*/
	/**************************************************************************/
	Matrix3x3& Matrix3x3::operator=(const Matrix3x3& rhs)
	{
		for (int i = 0; i < 9; ++i)
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
	Matrix3x3& Matrix3x3::operator*=(const Matrix3x3& rhs)
	{
		return *this = *this * rhs;
	}

	/**************************************************************************/
	/*!
		Index Operator, return an elemnt from index
	*/
	/**************************************************************************/

	float& Matrix3x3::operator[](int i)
	{
		return m[i];
	}

	/**************************************************************************/
	/*!
		Returns the determinant of the 3x3 matrix
	*/
	/**************************************************************************/

	float Matrix3x3::determinant() const
	{
		float d1 = m00 * m11 * m22;
		float d2 = m01 * m12 * m20;
		float d3 = m02 * m10 * m21;

		float dd1 = m02 * m11 * m20;
		float dd2 = m00 * m12 * m21;
		float dd3 = m01 * m10 * m22;
		// get determinant
		return (d1 + d2 + d3) - (dd1 + dd2 + dd3);
	}

	Matrix3x3 Matrix3x3::cofactor() const
	{
		Matrix3x3 mtx;
		// 1st row
		mtx.m00 = m11 * m22 - m12 * m21;
		mtx.m01 = -(m10 * m22 - m12 * m20);
		mtx.m02 = m10 * m21 - m11 * m20;
		// 2nd row
		mtx.m10 = -(m01 * m22 - m02 * m21);
		mtx.m11 = m00 * m22 - m02 * m20;
		mtx.m12 = -(m00 * m21 - m01 * m20);
		// 3rd row
		mtx.m20 = m01 * m12 - m02 * m11;
		mtx.m21 = -(m00 * m12 - m02 * m10);
		mtx.m22 = m00 * m11 - m01 * m10;

		return mtx;
	}

	//Matrix3x3::operator glm::mat4x4() const
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
	
	//Matrix3x3::operator glm::mat3x3() const
	//{
	//	glm::mat3x3 tmp{ m00,m01,m02,m10,m11,m12,m20,m21,m22 };
	//	return tmp;
	//}

	std::ostream& operator<<(std::ostream& os, const Matrix3x3 mtx)
	{
		os  << "( " << mtx.m00 << ", " << mtx.m01 << ", " << mtx.m02 << " )" << std::endl
			<< "( " << mtx.m10 << ", " << mtx.m11 << ", " << mtx.m12 << " )" << std::endl
			<< "( " << mtx.m20 << ", " << mtx.m21 << ", " << mtx.m22 << " )" << std::endl;
		return os;
	}

	/**************************************************************************/
	/*!
		Multiplies 2 4x4 matrices
	*/
	/**************************************************************************/
	Matrix3x3 operator*(const Matrix3x3& lhs, const Matrix3x3& rhs)
	{
		Matrix3x3 returnMatrix;
		// 1st row
		returnMatrix.m00 = lhs.m00 * rhs.m00 + lhs.m01 * rhs.m10 + lhs.m02 * rhs.m20;
		returnMatrix.m01 = lhs.m00 * rhs.m01 + lhs.m01 * rhs.m11 + lhs.m02 * rhs.m21;
		returnMatrix.m02 = lhs.m00 * rhs.m02 + lhs.m01 * rhs.m12 + lhs.m02 * rhs.m22;

		// 2nd row
		returnMatrix.m10 = lhs.m10 * rhs.m00 + lhs.m11 * rhs.m10 + lhs.m12 * rhs.m20;
		returnMatrix.m11 = lhs.m10 * rhs.m01 + lhs.m11 * rhs.m11 + lhs.m12 * rhs.m21;
		returnMatrix.m12 = lhs.m10 * rhs.m02 + lhs.m11 * rhs.m12 + lhs.m12 * rhs.m22;

		// 3rd row
		returnMatrix.m20 = lhs.m20 * rhs.m00 + lhs.m21 * rhs.m10 + lhs.m22 * rhs.m20;
		returnMatrix.m21 = lhs.m20 * rhs.m01 + lhs.m21 * rhs.m11 + lhs.m22 * rhs.m21;
		returnMatrix.m22 = lhs.m20 * rhs.m02 + lhs.m21 * rhs.m12 + lhs.m22 * rhs.m22;

		return returnMatrix;
	}

	/**************************************************************************/
	/*!
		This operator multiplies the matrix pMtx with the vector rhs
		and returns the result as a vector
	*/
	/**************************************************************************/
	Vector3D operator*(const Matrix3x3& pMtx, const Vector3D& rhs)
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
	void Mtx33Identity(Matrix3x3& pResult)
	{
		// 1st row
		pResult.m00 = 1;
		pResult.m01 = 0;
		pResult.m02 = 0;

		// 2nd row
		pResult.m10 = 0;
		pResult.m11 = 1;
		pResult.m12 = 0;

		// 3rd row
		pResult.m20 = 0;
		pResult.m21 = 0;
		pResult.m22 = 1;

		// 4th row
		pResult.m20 = 0;
		pResult.m21 = 0;
		pResult.m22 = 0;

	}

	/**************************************************************************/
	/*!
		This function creates a translation matrix from x & y
		and saves it in pResult
	*/
	/**************************************************************************/
	void Mtx33Translate(Matrix3x3& pResult, float x, float y)
	{
		// 1st row
		pResult.m00 = 1;
		pResult.m01 = 0;
		pResult.m02 = x;

		// 2nd row
		pResult.m10 = 0;
		pResult.m11 = 1;
		pResult.m12 = y;

		// 3rd row
		pResult.m20 = 0;
		pResult.m21 = 0;
		pResult.m22 = 1;
	}

	/**************************************************************************/
	/*!
		This function creates a scaling matrix from x & y
		and saves it in pResult
	*/
	/**************************************************************************/
	void Mtx33Scale(Matrix3x3& pResult, float x, float y)
	{
		// 1st row
		pResult.m00 = x;
		pResult.m01 = 0;
		pResult.m02 = 0;

		// 2nd row
		pResult.m10 = 0;
		pResult.m11 = y;
		pResult.m12 = 0;

		// 3rd row
		pResult.m20 = 0;
		pResult.m21 = 0;
		pResult.m22 = 1;

		// 4th row
		pResult.m20 = 0;
		pResult.m21 = 0;
		pResult.m22 = 0;

	}

	/**************************************************************************/
	/*!
		This matrix creates a rotation matrix from "angle" whose value
		is in radian. Save the resultant matrix in pResult.
	*/
	/**************************************************************************/
	void Mtx33RotRad(Matrix3x3& pResult, float angle)
	{
		float cosAngle = cos(angle);
		float sinAngle = sin(angle);
		// 1st row
		pResult.m00 = cosAngle;
		pResult.m01 = -sinAngle;
		pResult.m02 = 0;

		// 2nd row
		pResult.m10 = sinAngle;
		pResult.m11 = cosAngle;
		pResult.m12 = 0;

		// 3rd row
		pResult.m20 = 0;
		pResult.m21 = 0;
		pResult.m22 = 1;

		// 4th row
		pResult.m20 = 0;
		pResult.m21 = 0;
		pResult.m22 = 0;

	}

	/**************************************************************************/
	/*!
		This matrix creates a rotation matrix from "angle" whose value
		is in degree. Save the resultant matrix in pResult.
	*/
	/**************************************************************************/
	void Mtx33RotDeg(Matrix3x3& pResult, float angle)
	{
		float angleDeg = static_cast<float>(angle * M_PI / 180);
		Mtx33RotRad(pResult, angleDeg);
	}

	/**************************************************************************/
	/*!
		This functions calculated the transpose matrix of pMtx
		and saves it in pResult
	*/
	/**************************************************************************/
	void Mtx33Transpose(Matrix3x3& pResult, const Matrix3x3& pMtx)
	{
		// 1st row
		pResult.m00 = pMtx.m00;
		pResult.m01 = pMtx.m10;
		pResult.m02 = pMtx.m20;
		// 2nd row
		pResult.m10 = pMtx.m01;
		pResult.m11 = pMtx.m11;
		pResult.m12 = pMtx.m21;
		// 3rd row
		pResult.m20 = pMtx.m02;
		pResult.m21 = pMtx.m12;
		pResult.m22 = pMtx.m22;
	}

	/**************************************************************************/
	/*!
		This function calculates the inverse matrix of pMtx and saves the
		result in pResult. If the matrix inversion fails, pResult
		would be set to NULL.
	*/
	/**************************************************************************/
	void Mtx33Inverse(Matrix3x3& pResult, const Matrix3x3& pMtx)
	{
		// get determinant
		float det = pMtx.determinant();

		// check det
		if (det == 0)
		{
			pResult = nullptr;
			return;
		}

		//cofactor matrix
		Matrix3x3 mtx = pMtx.cofactor();

		// transpose mtx
		Mtx33Transpose(pResult, mtx);

		// 1/det
		det = 1.0f / det;

		// get final result
		// 1st row
		pResult.m00 = pResult.m00 * det;
		pResult.m01 = pResult.m01 * det;
		pResult.m02 = pResult.m02 * det;
		// 2nd row
		pResult.m10 = pResult.m10 * det;
		pResult.m11 = pResult.m11 * det;
		pResult.m12 = pResult.m12 * det;
		// 3rd row
		pResult.m20 = pResult.m20 * det;
		pResult.m21 = pResult.m21 * det;
		pResult.m22 = pResult.m22 * det;
	}

}

