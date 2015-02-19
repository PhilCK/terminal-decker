
#ifndef CAFF_MATH_QUATERNION_INCLUDED
#define CAFF_MATH_QUATERNION_INCLUDED



#include <Caffeine/Platform.hpp>
#include <Caffeine/Math/Math.hpp>
#include <Caffeine/Math/Vector.hpp>
#include <Caffeine/Math/Matrix.hpp>
#include <iostream>



namespace Caffeine {
namespace Math {



struct Quaternion
{
	float w = 1.f;
	float x = 0.f;
	float y = 0.f;
	float z = 0.f;
};



inline Quaternion QuaternionInit(float w, float x, float y, float z)
{
	Quaternion q;

	q.w = w;
	q.x = x;
	q.y = y;
	q.z = z;

	return q;
}


inline float QuaternionGetW(const Quaternion &quat) { return quat.w; }
inline float QuaternionGetX(const Quaternion &quat) { return quat.x; }
inline float QuaternionGetY(const Quaternion &quat) { return quat.y; }
inline float QuaternionGetZ(const Quaternion &quat) { return quat.z; }


inline Quaternion QuaternionInitFromAngleAxis(const float thetaRadians, const Vector3 &axis)
{
	// TODO: Normalize Axis.

	const float halfAngle = 0.5f * thetaRadians;
	const float sinAngle  = Sin(halfAngle);

	const float w = Cos(halfAngle);
	const float x = sinAngle * axis.x;
	const float y = sinAngle * axis.y;
	const float z = sinAngle * axis.z;

	return QuaternionInit(w, x, y, z);
}



inline Quaternion QuaternionInitFromAngleAxis(const float thetaRadians, const float xAxis, const float yAxis, const float zAxis)
{
	const Vector3 axis = {xAxis, yAxis, zAxis};
	return QuaternionInitFromAngleAxis(thetaRadians, axis);
}



inline Quaternion QuaternionInitFromEulerAngles(float xDeg, float yDeg, float zDeg)
{
//	const float halfRadX = 0.5f * CaffMath::DegToRad(xDeg);
//	const float halfRadY = 0.5f * CaffMath::DegToRad(yDeg);
//	const float halfRadZ = 0.5f * CaffMath::DegToRad(zDeg);
//
//	const float cosX = CaffMath::Cos(halfRadX);
//	const float cosY = CaffMath::Cos(halfRadY);
//	const float cosZ = CaffMath::Cos(halfRadZ);
//	
//	const float sinX = CaffMath::Sin(halfRadX);
//	const float sinY = CaffMath::Sin(halfRadY);
//	const float sinZ = CaffMath::Sin(halfRadZ);
//	
//	const float w = cosZ * cosY * cosX + sinZ * sinY * sinX;
//	const float i = cosZ * cosY * sinX - sinZ * sinY * cosX;
//	const float j = cosZ * sinY * cosX + sinZ * cosY * sinX;
//	const float k = sinZ * cosY * cosX - cosZ * sinY * sinX;
//	
//	Quaternion quat;
//	quat.w = w;
//	quat.x = i;
//	quat.y = j;
//	quat.z = k;
//
//	return quat;
	
	float cz = CaffMath::Cos(CaffMath::DegToRad(zDeg) * 0.5f);
    float sz = CaffMath::Sin(CaffMath::DegToRad(zDeg) * 0.5f);
    float cy = CaffMath::Cos(CaffMath::DegToRad(yDeg) * 0.5f);
    float sy = CaffMath::Sin(CaffMath::DegToRad(yDeg) * 0.5f);
    float cx = CaffMath::Cos(CaffMath::DegToRad(xDeg) * 0.5f);
    float sx = CaffMath::Sin(CaffMath::DegToRad(xDeg) * 0.5f);

    float w = cz*cy*cx + sz*sy*sz;
  	float x = cz*cy*sx - sz*sy*cz;
	float y = cz*sy*cx + sz*cy*sz;
	float z = sz*cy*cx - cz*sy*sz;
    
	return QuaternionInit(w, x, y, z);
}



inline Vector3 QuaternionGetEulerAnglesInRadians(const Quaternion &quat)
{
	const float poleTest = 2.f * (quat.y * quat.w - quat.x * quat.z);
	
	const float errorMarginTest = 0.0001f;
	
	if(IsNear(poleTest, 1.f, errorMarginTest))
	{
		const float x = 0;
		const float y = QuartTau();
		const float z = -2.f * ATan2(quat.x, quat.w);

		const Vector3 retVec = {x, y, z};

		return retVec;
	}
	else if(IsNear(poleTest, -1.f, errorMarginTest))
	{
		const float x = 0;
		const float y = -QuartTau();
		const float z = 2.f * ATan2(quat.x, quat.w);

		const Vector3 retVec = {x, y, z};

		return retVec;
	}
	else
	{
		const float wSq = quat.w * quat.w;
		const float xSq = quat.x * quat.x;
		const float ySq = quat.y * quat.y;
		const float zSq = quat.z * quat.z;

		const float x = ATan2(2.f * (quat.y * quat.z + quat.x * quat.w), (-xSq - ySq + zSq + wSq));
		const float y = ASin(Clamp(poleTest, -1.f, 1.f));
		const float z = ATan2(2.f * (quat.x * quat.y + quat.z * quat.w), (xSq - ySq - zSq + wSq));

		const Vector3 retVec = {x, y, z};

		return retVec;
	}
}



inline Vector3 QuaternionGetEulerAnglesInDegrees(const Quaternion &quat)
{
	const Vector3 inRads(QuaternionGetEulerAnglesInRadians(quat));

	const Vector3 inDegs = {RadToDeg(inRads.x), RadToDeg(inRads.y), RadToDeg(inRads.z)};
	return inDegs;
}



inline float QuaternionLength(const Quaternion toLength)
{
	return CaffMath::SquareRoot((toLength.w * toLength.w) + (toLength.x * toLength.x) + (toLength.y * toLength.y) + (toLength.z * toLength.z));
}



inline Quaternion QuaternionNormalize(const Quaternion toNormal)
{
	const float length = QuaternionLength(toNormal);
	assert(length); // Can't have zero length.

	Quaternion quat;
	quat.w = toNormal.w / length;
	quat.x = toNormal.x / length;
	quat.y = toNormal.y / length;
	quat.z = toNormal.z / length;

	return quat;
}



 inline Quaternion QuaternionAdd(const Quaternion left, const Quaternion right)
 {
 	return QuaternionInit(left.w + right.w, left.x + right.x, left.y + right.y, left.z + right.z);
 }



 inline Quaternion QuaternionDivide(const Quaternion left, const Quaternion right)
 {
	 return QuaternionInit(left.w / right.w, left.x / right.x, left.y / right.y, left.z / right.z);
 }



// inline Quaternion QuaternionSubtract(const Quaternion left, const Quaternion right)
// {
// 	return QuaternionInit(left.r + right.r, Vector3ComponentSubtract(left.vec3, right.vec3));
// }



inline Quaternion QuaternionMultiply(const Quaternion left, const Quaternion right)
{
	const float w = (left.w * right.w) - (left.x * right.x) - (left.y * right.y) - (left.z * right.z);
	const float x = (left.w * right.x) + (left.x * right.w) + (left.y * right.z) - (left.z * right.y);
	const float y = (left.w * right.y) + (left.y * right.w) + (left.z * right.x) - (left.x * right.z);
	const float z = (left.w * right.z) + (left.z * right.w) + (left.x * right.y) - (left.y * right.x);

	Quaternion resultQuat;
	resultQuat.w = w;
	resultQuat.x = x;
	resultQuat.y = y;
	resultQuat.z = z;

	return resultQuat;
}



// inline Quaternion QuaternionScale(const Quaternion toScale, const float scale)
// {
// 	return QuaternionInit(toScale.real * scale, Vector3Scale(toScale.vec3, scale));
// }



inline Quaternion QuaternionConjugate(const Quaternion toConjugate)
{
	return QuaternionInit(toConjugate.w, -toConjugate.x, -toConjugate.y, -toConjugate.z);
}



//inline Quaternion QuaternionInverse(const Quaternion toInverse)
//{
//	const Quaternion conjugate	= QuaternionConjugate(toInverse);
//	//const Quaternion abs		= QuaternionInit(Abs(QuaternionGetW(toInverse)), Abs(QuaternionGetX(toInverse)), Abs(QuaternionGetY(toInverse)), Abs(QuaternionGetZ(toInverse)));
//	//const Quaternion inverse	= QuaternionDivide(conjugate, abs);
//
//	return conjugate;
//}



inline Vector3 QuaternionRotate(const Quaternion rotate, const Vector3 point)
{    
//    const Vector3 u = { rotate.x, rotate.y, rotate.z };
//    const float   s = rotate.w;
//    
//    const float   dotUV = Vector3DotProduct(u, point);
//    const float   dotUU = Vector3DotProduct(u, u);
//    const Vector3 crossUV = Vector3CrossProduct(u, point);
//    
//    const Vector3 VA = Vector3Scale(u, (2.f * dotUV));
//    const Vector3 VB = Vector3Scale(point, (s*s - dotUU));
//    const Vector3 VC = Vector3Scale(crossUV, (2.f * s));
//    
//    const Vector3 resultA = Vector3ComponentAdd(VA, VB);
//    const Vector3 resultB = Vector3ComponentAdd(resultA, VC);
//    
//    return resultB;
    
    const float x = rotate.x;
    const float y = rotate.y;
    const float z = rotate.z;
    const float w = rotate.w;
    
	const float pX = w*w*point.x + 2*y*w*point.z - 2*z*w*point.y + x*x*point.x + 2*y*x*point.y + 2*z*x*point.z - z*z*point.x - y*y*point.x;
	const float pY = 2*x*y*point.x + y*y*point.y + 2*z*y*point.z + 2*w*z*point.x - z*z*point.y + w*w*point.y - 2*x*w*point.z - x*x*point.y;
	const float pZ = 2*x*z*point.x + 2*y*z*point.y + z*z*point.z - 2*w*y*point.x - y*y*point.z + 2*w*x*point.y - x*x*point.z + w*w*point.z;
    
    const Vector3 returnVec = { pX, pY, pZ };
    
    return returnVec;
}



inline Matrix33 QuaternionToRotationMatrix(const Quaternion quat)
{
    const float xSq = quat.x * quat.x;
    const float ySq = quat.y * quat.y;
    const float zSq = quat.z * quat.z;
    
	Matrix33 mat;
	{
//		mat._11 = 1 - 2 * ySq - 2 * zSq;
//		mat._12 = 2 * (quat.x * quat.y) - 2 * (quat.z * quat.w);
//		mat._13 = 2 * (quat.x * quat.z) + 2 * (quat.y * quat.w);
//
//		mat._21 = 2 * (quat.x * quat.y) + 2 * (quat.z * quat.w);
//		mat._22 = 1 - 2 * xSq - 2 * zSq;
//		mat._23 = 2 * (quat.y * quat.z) - 2 * (quat.x * quat.w);
//
//		mat._31 = 2 * (quat.x * quat.z) - 2 * (quat.y * quat.w);
//		mat._32 = 2 * (quat.y * quat.z) + 2 * (quat.x * quat.w);
//		mat._33 = 1 - 2 * xSq - 2 * ySq;
        
		mat._11 = 1 - 2 * ySq - 2 * zSq;
		mat._21 = 2 * (quat.x * quat.y) - 2 * (quat.z * quat.w);
		mat._31 = 2 * (quat.x * quat.z) + 2 * (quat.y * quat.w);
        
		mat._12 = 2 * (quat.x * quat.y) + 2 * (quat.z * quat.w);
		mat._22 = 1 - 2 * xSq - 2 * zSq;
		mat._32 = 2 * (quat.y * quat.z) - 2 * (quat.x * quat.w);
        
		mat._13 = 2 * (quat.x * quat.z) - 2 * (quat.y * quat.w);
		mat._23 = 2 * (quat.y * quat.z) + 2 * (quat.x * quat.w);
		mat._33 = 1 - 2 * xSq - 2 * ySq;
	}
	
	return mat;
}



} // namespace
} // namespace


#endif // include guard
