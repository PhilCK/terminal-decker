#ifndef CAFFEINE_MATH_MATRIX_INCLUDED
#define CAFFEINE_MATH_MATRIX_INCLUDED


#include <Caffeine/Math/Vector.hpp>
#include <Caffeine/Math/Math.hpp>


namespace Caffeine {
namespace Math {


struct Matrix33
{
	float _11, _12, _13;
	float _21, _22, _23;
	float _31, _32, _33;
};


struct Matrix44
{
	float _11, _12, _13, _14;
	float _21, _22, _23, _24;
	float _31, _32, _33, _34;
	float _41, _42, _43, _44;
};
  

// *** Matrix 33 *** //

inline Matrix33 Matrix33InitScaleMatrix(const Vector3 &scale)
{
	const Matrix33 returnMat = {Vector3GetX(scale), 0, 0, 0, Vector3GetY(scale), 0, 0, 0, Vector3GetZ(scale)};
	
	return returnMat;
}


inline Matrix33 Matrix33Multiply(const Matrix33 &left, const Matrix33 &right)
{
	Matrix33 returnMat = {0,0,0,0,0,0,0,0,0};
	
	returnMat._11 = (left._11 * right._11) + (left._12 * right._21) + (left._13 * right._31);
	returnMat._12 = (left._11 * right._12) + (left._12 * right._22) + (left._13 * right._32);
	returnMat._13 = (left._11 * right._13) + (left._12 * right._23) + (left._13 * right._33);
	
	returnMat._21 = (left._21 * right._11) + (left._22 * right._21) + (left._23 * right._31);
	returnMat._22 = (left._21 * right._12) + (left._22 * right._22) + (left._23 * right._32);
	returnMat._23 = (left._21 * right._13) + (left._22 * right._23) + (left._23 * right._33);
	
	returnMat._31 = (left._31 * right._11) + (left._32 * right._21) + (left._33 * right._31);
	returnMat._32 = (left._31 * right._12) + (left._32 * right._22) + (left._33 * right._32);
	returnMat._33 = (left._31 * right._13) + (left._32 * right._23) + (left._33 * right._33);
	
	return returnMat;
}


// *** Matrix 44 *** //

inline Matrix44 Matrix44Init(const float initValue)
{
  Matrix44 returnMat;

  memset(&returnMat, initValue, sizeof(returnMat));
  
  return returnMat;
}


inline Matrix44 Matrix44InitIdentity()
{
  Matrix44 returnMat;
  memset(&returnMat, 0, sizeof(returnMat));
  
  returnMat._11 = 1.f;
  returnMat._22 = 1.f;
  returnMat._33 = 1.f;
  returnMat._44 = 1.f;
  
  return returnMat;
}

		//T const tanHalfFovy = tan(fovy / static_cast<T>(2));

		//tmat4x4<T, defaultp> Result(static_cast<T>(0));
		//Result[0][0] = static_cast<T>(1) / (aspect * tanHalfFovy);
		//Result[1][1] = static_cast<T>(1) / (tanHalfFovy);
		//Result[2][2] = - (zFar + zNear) / (zFar - zNear);
		//Result[2][3] = - static_cast<T>(1);
		//Result[3][2] = - (static_cast<T>(2) * zFar * zNear) / (zFar - zNear);
inline Matrix44 Matrix44Projection(const float fov, const float width, const float height, const float nearPlane, const float farPlane)
{
  const float aspectRatio = width / height;
  const float oneOverTanHalfFov = 1 / CaffMath::Tan(fov * 0.5f);
  const float planeDiff = farPlane - nearPlane;

  Matrix44 returnMat = Matrix44Init(0);

  returnMat._11 = oneOverTanHalfFov / aspectRatio;
  returnMat._22 = oneOverTanHalfFov;
  returnMat._33 = - (farPlane + nearPlane) / planeDiff;
  returnMat._34 = -1.f;
  returnMat._43 = - (2 * farPlane * nearPlane) / planeDiff;

  //const float viewPort = height / width;
  //const float depth = farPlane - nearPlane;
  //const float focal = 1 / CaffMath::Tan(0.5 * fov);

  //Matrix44 returnMat = Matrix44Init(0);

  //returnMat._11 = focal / viewPort;
  //returnMat._22 = focal;
  //returnMat._33 = (farPlane + nearPlane) / depth;
  //returnMat._34 = (2 * farPlane * nearPlane) / depth;
  //returnMat._43 = -1.f;
  
  return returnMat;
}


inline Matrix44 Matrix44LookAt(const Vector3 eye, const Vector3 up, const Vector3 at)
{
  const Vector3 zAxis = Vector3Normalize(Vector3ComponentSubtract(at, eye));
  const Vector3 xAxis = Vector3Normalize(Vector3CrossProduct(zAxis, up));
  const Vector3 yAxis = Vector3CrossProduct(xAxis, zAxis);

  Matrix44 returnMat = Matrix44Init(0);

  returnMat._11 = Vector3GetX(xAxis);
  returnMat._12 = Vector3GetX(yAxis);
  returnMat._13 = -Vector3GetX(zAxis);

  returnMat._21 = Vector3GetY(xAxis);
  returnMat._22 = Vector3GetY(yAxis);
  returnMat._23 = -Vector3GetY(zAxis);

  returnMat._31 = Vector3GetZ(xAxis);
  returnMat._32 = Vector3GetZ(yAxis);
  returnMat._33 = -Vector3GetZ(zAxis);

  returnMat._41 = -(Vector3DotProduct(xAxis, eye));
  returnMat._42 = -(Vector3DotProduct(yAxis, eye));
  returnMat._43 = +(Vector3DotProduct(zAxis, eye));
  returnMat._44 = 1.f;

  return returnMat;
}


} // namespace
} // namespace


#endif // include guard