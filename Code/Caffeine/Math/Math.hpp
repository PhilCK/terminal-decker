
/*
	Math.hpp
	--
	General math functions.

*/

#ifndef CAFFEINE_MATH_MATH_INCLUDED
#define CAFFEINE_MATH_MATH_INCLUDED



#include <math.h>
#include <random>
#include <algorithm>



namespace Caffeine {
namespace Math {



#pragma mark - Math Constants

// Math constants
inline float Pi() 		{ return 3.14159265359f; 	}
inline float TwoPi() 	{ return 2.0f * Pi(); 		}
inline float HalfPi()	{ return 0.5f * Pi(); 		}

inline float Tau() 		  { return TwoPi(); 	}
inline float HalfTau()	{ return Pi(); 		  }
inline float QuartTau()	{ return HalfPi(); 	}

inline float E() 		  { return 2.71828f; 			  }
inline float GRatio()	{ return 1.61803398875f; 	}
inline float Root2()	{ return 1.41421356237f; 	}
inline float Root3()	{ return 1.73205080757f; 	}



#pragma mark - Trig Functions

inline float Sin(const float x)  					        { return std::sin(x); 		  }
inline float ASin(const float x) 					        { return std::asin(x); 		  }
inline float Cos(const float x) 					        { return std::cos(x);		    }
inline float ACos(const float x) 					        { return std::acos(x); 		  }
inline float Tan(const float x) 					        { return std::tan(x);		    }
inline float ATan(const float x) 					        { return std::atan(x);		  }
inline float ATan2(const float x, const float y) 	{ return std::atan2(x, y); 	}



#pragma mark - Log and Exp

float Log2(const float x);

float Log10(const float x);

float Exp(const float x);

float Pow(const float x, const float pow);


#pragma mark - Min, Max, Clamp, Round, Between

template<typename T>
T Max(const T x, const T y) {
	return x > y ? x : y;
}

template<typename T>
T Min(const T x, const T y) {
	return x < y ? x : y;
}

template<typename T>
T Clamp(const T a, const T lower, const T upper)
{
	return std::max(lower, std::min(a, upper));
}

template<typename T>
T Ceil(const T x) {
	return std::ceil(x);
}

template<typename T>
T Floor(const T x) {
	return std::floor(x);
}

inline float FMod(const float x, const float max)
{
	return std::fmod(x, max);
}

// Is 'x' between 'a' and 'b'
template<typename T>
bool InRange(const T x, const T a, const T b)
{
	const float smallest = CaffMath::Min(a, b);
	const float biggest  = CaffMath::Max(a, b);
	
	return (x < smallest || x > biggest);
}


#pragma mark - Degrees and Radians

inline float DegToRad(const float x) { return x * (HalfTau() / 180); }
inline float RadToDeg(const float x) { return x * (180 / HalfTau()); }



#pragma mark - Random

inline float RandFloatRange(const float start, const float end)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(start, end);
	
	return dis(gen);
}



#pragma mark - Other

inline float SquareRoot(const float x) {
	return (float)sqrt(x);
}



inline bool IsPOW2(const int i)
{
	return (i & -i) == i;
}



template<typename T>
T Abs(const T x)
{
	if(x < 0)
	{
		return x * -1;
	}
	
	return x;
}



inline bool IsNear(const float a, const float b, const float errorMargin)
{
	// TODO: HAHAHA Phil -10 and 10 will be near you muppet!
	return (Abs(Abs(b) - Abs(a)) <= errorMargin);
}


    
} // namespace
} // namespace

#endif // include guard
