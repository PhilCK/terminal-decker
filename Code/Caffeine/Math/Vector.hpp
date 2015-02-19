
/*
	Vector.hpp
	--
	This decides what vector implimentation to use.
*/

#ifndef CAFFEINE_MATH_VECTOR_INCLUDED
#define CAFFEINE_MATH_VECTOR_INCLUDED


#include <Caffeine/Common/Platform.hpp>

#define VECTOR_INLINE inline

#ifndef NDEBUG
#include <Caffeine/Math/DebugVector.hpp>
#else
#include <Caffeine/Math/DebugVector.hpp> // We don't have a SIMD one yet!
#endif // Debug check


// Vector Constants
// constexpr Vector2 Vector2Zero() { return Vector2Init(0.f, 0.f); }

// constexpr Vector3 Vector3Zero();
// constexpr Vector3 Vector3Up();
// constexpr Vector3 Vector3Forward();
// constexpr Vector3 Vector3Right();

// constexpr Vector4 Vector4Zero();


#endif // include guard
