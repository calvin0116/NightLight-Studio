#include "Vector.h"

namespace NlMath
{
#ifdef _MSC_VER
	// Supress warning: nonstandard extension used : nameless struct/union
#pragma warning( disable : 4201 )
#endif

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
		Matrix4x4(const float x);
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

		//conversion operator is case we are using open gl, this converts my matrix to glm matrix
		//operator glm::mat4x4() const;
		//operator glm::mat3x3() const;

	} Matrix4x4, Matrix4x4, Mtx44;

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
	void Mtx44Translate(Matrix4x4& pResult, float x, float y);

	/**************************************************************************/
	/*!
		This function creates a scaling matrix from x & y
		and saves it in pResult
	 */
	 /**************************************************************************/
	void Mtx44Scale(Matrix4x4& pResult, float x, float y);

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
	void Mtx44Transpose(Matrix4x4& pResult, const Matrix4x4& pMtx);

	/**************************************************************************/
	/*!
		This function calculates the inverse matrix of pMtx and saves the
		result in pResult. If the matrix inversion fails, pResult
		would be set to NULL.
	*/
	/**************************************************************************/
	void Mtx44Inverse(Matrix4x4* pResult, const Matrix4x4& pMtx);
}