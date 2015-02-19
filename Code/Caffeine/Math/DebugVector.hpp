/*
	Caffeine's Debug Vector implimentation.

	These are used to allow easy viewing of the variables inside the debuggers.
*/

#ifndef CAFFEINE_MATH_DEBUG_VECTOR_INCLUDED
#define CAFFEINE_MATH_DEBUG_VECTOR_INCLUDED



#include <Caffeine/Common/Platform.hpp>
#include <Caffeine/Math/Math.hpp>
#include <assert.h>



namespace Caffeine {
namespace Math {



#pragma mark - Vector2

struct Vector2
{
	// Recomended you do not access these members directly, they are here for debug. //
	union
	{
		struct { float x, y; };
		struct { float s, t; };
		struct { float u, v; };
	};
};



/*
	Initialize the Vector.
*/
VECTOR_INLINE Vector2 Vector2Init(float x, float y)
{
	const Vector2 returnVec = {{x, y}};

	return returnVec;
}



VECTOR_INLINE Vector2 Vector2Init(float f)
{
	return Vector2Init(f, f);
}



/*
	Component math.
*/
VECTOR_INLINE Vector2 Vector2ComponentAdd(const Vector2 left, const Vector2 right) 		{ return Vector2Init(left.x + right.x, left.y + right.y); }
VECTOR_INLINE Vector2 Vector2ComponentSubtract(const Vector2 left, const Vector2 right) { return Vector2Init(left.x - right.x, left.y - right.y); }
VECTOR_INLINE Vector2 Vector2ComponentMultiply(const Vector2 left, const Vector2 right) { return Vector2Init(left.x * right.x, left.y * right.y); }



VECTOR_INLINE Vector2 Vector2ComponentDivide(const Vector2 left, const Vector2 right)
{
	// Check not multiply by zero.
	assert(right.x);
	assert(right.y);

	return Vector2Init(left.x / right.x, left.y / right.y);
}



/*
	Scale of vector.
*/
VECTOR_INLINE Vector2 Vector2Scale(const Vector2 toScale, const float scale)
{
	return Vector2Init(toScale.x * scale, toScale.y * scale);
}



VECTOR_INLINE float Vector2Magnitude(const Vector2 toMagnatude)
{
	return CaffMath::SquareRoot((toMagnatude.x * toMagnatude.x) + (toMagnatude.y * toMagnatude.y));
}



VECTOR_INLINE Vector2 Vector2Normalize(const Vector2 toNormalize)
{
	float length = Vector2Magnitude(toNormalize);
	assert(length); // Can't do zero length vectors.

	return Vector2Init(toNormalize.x / length, toNormalize.y / length);
}



/*
	Cross and Dot.
*/
VECTOR_INLINE float Vector2CrossProduct(const Vector2 left, const Vector2 right)
{
	return (left.x * right.y) - (left.y * right.x);
}



VECTOR_INLINE float Vector2DotProduct(const Vector2 left, const Vector2 right)
{
	return (left.x * right.x) + (left.y * right.y);
}



/*
	Get components of vector.
*/
VECTOR_INLINE float Vector2GetX(const Vector2 getComponent) { return getComponent.x; } 
VECTOR_INLINE float Vector2GetY(const Vector2 getComponent) { return getComponent.y; }

VECTOR_INLINE float Vector2GetU(const Vector2 getComponent) { return getComponent.x; }
VECTOR_INLINE float Vector2GetV(const Vector2 getComponent) { return getComponent.y; }

VECTOR_INLINE float Vector2GetS(const Vector2 getComponent) { return getComponent.x; }
VECTOR_INLINE float Vector2GetT(const Vector2 getComponent) { return getComponent.y; }

#pragma mark - Vector3

struct Vector3
{
	union
	{
		struct { float x, y, z; };
		struct { float r, g, b; };
		struct { float s, t, p; };
	};
};



/*
	Initialize the Vector.
*/
VECTOR_INLINE Vector3 Vector3Init(float x, float y, float z)
{
	Vector3 returnVec = {{x, y, z}};

	return returnVec;
}



VECTOR_INLINE Vector3 Vector3Init(float f)
{
	return Vector3Init(f, f, f);
}



/*
	Component math.
*/
VECTOR_INLINE Vector3 Vector3ComponentAdd(const Vector3 left, const Vector3 right) 			{ return Vector3Init(left.x + right.x, left.y + right.y, left.z + right.z); }
VECTOR_INLINE Vector3 Vector3ComponentSubtract(const Vector3 left, const Vector3 right)		{ return Vector3Init(left.x - right.x, left.y - right.y, left.z - right.z); }
VECTOR_INLINE Vector3 Vector3ComponentMultiply(const Vector3 left, const Vector3 right)		{ return Vector3Init(left.x * right.x, left.y * right.y, left.z * right.z); }



VECTOR_INLINE Vector3 Vector3ComponentDivide(const Vector3 left, const Vector3 right)
{
	// Div by zero check
	assert(right.x);
	assert(right.y);
	assert(right.z);

	return Vector3Init(left.x / right.x, left.y / right.y, left.z / right.z);
}



/*
	Scale of vector.
*/
VECTOR_INLINE Vector3 Vector3Scale(const Vector3 toScale, const float scale)
{
	return Vector3Init(toScale.x * scale, toScale.y * scale, toScale.z * scale);
}



VECTOR_INLINE float Vector3Magnitude(const Vector3 toMagnatude)
{
	return CaffMath::SquareRoot((toMagnatude.x * toMagnatude.x) + (toMagnatude.y * toMagnatude.y) + (toMagnatude.z * toMagnatude.z));
}



VECTOR_INLINE Vector3 Vector3Normalize(const Vector3 toNormalize)
{
	const float mag = Vector3Magnitude(toNormalize);
	assert(mag); // div by zero check.

	return Vector3Init(toNormalize.x / mag, toNormalize.y / mag, toNormalize.z / mag);
}



/*
	Cross and Dot.
*/
VECTOR_INLINE Vector3 Vector3CrossProduct(const Vector3 left, const Vector3 right)
{
	const float i = (left.y * right.z) - (left.z * right.y);
	const float j = (left.x * right.z) - (left.z * right.x);
	const float k = (left.x * right.y) - (left.y * right.x);

	return Vector3Init(i, -j, k);
}



VECTOR_INLINE float Vector3DotProduct(const Vector3 left, const Vector3 right)
{
	return (left.x * right.x) + (left.y * right.y) + (left.z * right.z);
}



/*
	Get components of vector.
*/
VECTOR_INLINE float Vector3GetX(const Vector3 vec3)	{ return vec3.x; }
VECTOR_INLINE float Vector3GetY(const Vector3 vec3) { return vec3.y; }
VECTOR_INLINE float Vector3GetZ(const Vector3 vec3)	{ return vec3.z; }

VECTOR_INLINE float Vector3GetR(const Vector3 vec3)	{ return vec3.x; }
VECTOR_INLINE float Vector3GetG(const Vector3 vec3) { return vec3.y; }
VECTOR_INLINE float Vector3GetB(const Vector3 vec3)	{ return vec3.z; }

VECTOR_INLINE float Vector3GetU(const Vector3 vec3) { return vec3.x; }
VECTOR_INLINE float Vector3GetV(const Vector3 vec3) { return vec3.y; }

VECTOR_INLINE float Vector3GetS(const Vector3 vec3)	{ return vec3.x; }
VECTOR_INLINE float Vector3GetT(const Vector3 vec3) { return vec3.y; }
VECTOR_INLINE float Vector3GetP(const Vector3 vec3)	{ return vec3.z; }


inline float Vector3At(const Vector3 vec, const uint32_t i)
{
  assert(i < 3);

  switch (i)
  {
    case(0):
      return Vector3GetX(vec);
      break;
    case(1):
      return Vector3GetY(vec);
      break;
    case(2):
      return Vector3GetZ(vec);
      break;
  }

  return 0; // this is just to shoosh warnings.
}



inline void Vector3SetAt(Vector3 vec, const float val, const uint32_t i)
{
  assert(i < 3);

  switch (i)
  {
  case(0) :
    vec.x = val;
    break;
  case(1) :
    vec.y = val;
    break;
  case(2) :
    vec.z = val;
    break;
  }
}


#pragma mark - Vector4

struct Vector4
{
	union
	{
		struct { float x, y, z, w; };
		struct { float r, g, b, a; };
		struct { float s, t, p, q; };
	};
};



/*
	Initialize the Vector.
*/
VECTOR_INLINE Vector4 Vector4Init(float x, float y, float z, float w)
{
	Vector4 returnVec = {{x, y, z, w}};

	return returnVec;
}



VECTOR_INLINE Vector4 Vector4Init(float f)
{
	return Vector4Init(f,f,f,f);
}



/*
	Component math.
*/
VECTOR_INLINE Vector4 Vector4ComponentAdd(const Vector4 left, const Vector4 right)		{ return Vector4Init(left.x + right.x, left.y + right.y, left.z + right.z, left.w + right.w); }
VECTOR_INLINE Vector4 Vector4ComponentSubtract(const Vector4 left, const Vector4 right) { return Vector4Init(left.x - right.x, left.y - right.y, left.z - right.z, left.w - right.w); }
VECTOR_INLINE Vector4 Vector4ComponentMultiply(const Vector4 left, const Vector4 right) { return Vector4Init(left.x * right.x, left.y * right.y, left.z * right.z, left.w * right.w); }



VECTOR_INLINE Vector4 Vector4ComponentDivide(const Vector4 left, const Vector4 right)
{
	// Div by zero check
	assert(right.x);
	assert(right.y);
	assert(right.z);
	assert(right.w);

	return Vector4Init(left.x / right.x, left.y / right.y, left.z / right.z, left.w / right.w);
}



/*
	Scale of vector.
*/
VECTOR_INLINE Vector4 Vector4Scale(const Vector4 toScale, const float scale) 	{ return Vector4Init(toScale.x * scale, toScale.y * scale, toScale.z * scale, toScale.w * scale); }
VECTOR_INLINE float Vector4Magnitude(const Vector4 toMagnatude) 				{ return CaffMath::SquareRoot((toMagnatude.x * toMagnatude.x) + (toMagnatude.y * toMagnatude.y) + (toMagnatude.z * toMagnatude.z) + (toMagnatude.w * toMagnatude.w)); }


VECTOR_INLINE Vector4 Vector4Normalize(const Vector4 toNormalize)
{
	const float mag = Vector4Magnitude(toNormalize);
	assert(mag); // div zero check.

	return Vector4Init(toNormalize.x / mag, toNormalize.y / mag, toNormalize.z / mag, toNormalize.w / mag);
}


/*
	Dot
*/
VECTOR_INLINE float Vector4DotProduct(const Vector4 left, const Vector4 right)
{
	return (left.x * right.x) + (left.y * right.y) + (left.z * right.z) + (left.w * right.w);
}



/*
	Get components of vector.
*/
VECTOR_INLINE float Vector4GetX(const Vector4 vec) { return vec.x; }
VECTOR_INLINE float Vector4GetY(const Vector4 vec) { return vec.y; }
VECTOR_INLINE float Vector4GetZ(const Vector4 vec) { return vec.z; }
VECTOR_INLINE float Vector4GetW(const Vector4 vec) { return vec.w; }

VECTOR_INLINE float Vector4GetR(const Vector4 vec) { return vec.x; }
VECTOR_INLINE float Vector4GetG(const Vector4 vec) { return vec.y; }
VECTOR_INLINE float Vector4GetB(const Vector4 vec) { return vec.z; }
VECTOR_INLINE float Vector4GetA(const Vector4 vec) { return vec.w; }

VECTOR_INLINE float Vector4GetU(const Vector4 vec) { return vec.x; }
VECTOR_INLINE float Vector4GetV(const Vector4 vec) { return vec.y; }

VECTOR_INLINE float Vector4GetS(const Vector4 vec) { return vec.x; }
VECTOR_INLINE float Vector4GetT(const Vector4 vec) { return vec.y; }
VECTOR_INLINE float Vector4GetP(const Vector4 vec) { return vec.z; }
VECTOR_INLINE float Vector4GetQ(const Vector4 vec) { return vec.w; }


} // namespace
} // namespace


#endif // include guard
