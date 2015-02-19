
/*
	NonCopyable.hpp
	---------------
	
	Project: Caffeine
	
*/

#ifndef UTILITIES_NON_COPYABLE_INCLUDED
#define UTILITIES_NON_COPYABLE_INCLUDED

namespace Caffeine {
namespace Utilities {

/*
	NonCopyable
	-----------
	
	Inherit from this to make a class non-copyable.
*/

struct NonCopyable
{
	NonCopyable()									= default;
	NonCopyable(const NonCopyable &)				= delete;
	NonCopyable & operator=(const NonCopyable &)	= delete;
	
}; // struct

} // namespace
} // namespace

#endif // include guard
