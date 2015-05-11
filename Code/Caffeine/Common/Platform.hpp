
/*
	Bitmask Games
	--
	Please refer to the LICENSE.txt in Doc folder to see license details.
	
	Website:
	Email:


	Platform.hpp
	--
	Foundation include that has default includes and platform specific includes.
	Along with some defines to help with platform defines.
*/

#ifndef CAFFEINE_UTILITIES_PLATFORM_INCLUDED
#define CAFFEINE_UTILITIES_PLATFORM_INCLUDED



// **** Caffeine **** //
#ifdef __APPLE__

#define CAFF_ON_MAC
#define CAFF_ON_NIX

#endif

#ifdef WIN32

#define CAFF_ON_WINDOWS

#endif



// **** Stage **** //

#ifndef NDEBUG

#define CAFF_DEBUG

#else

#define CAFF_RELEASE

#endif

// CAFF_TESTING



// **** Default Includes **** //

#include <cstdint>
#include <assert.h>

// To keep inline with fixed int sizes.
typedef float float32_t;
typedef double float64_t;


#ifdef CAFF_ON_MAC
#include <sys/stat.h>
#ifndef st_mtime
#define st_mtime st_mtimespec.tv_sec
#endif
#endif // CAFF_ON_MAC



// **** SDL2 **** //

#ifndef CAFF_UNIT_TEST

#include <SDL2/SDL.h>

#endif // CAFF_UNIT_TEST

#ifdef CAFF_ON_WINDOWS
//#pragma comment(lib, "SDL2.lib")
//#pragma comment(lib, "SDL2main.lib")
#endif // CAFF_ON_WINDOWS



// **** OpenGL **** //

// If OSX.
#ifdef CAFF_ON_MAC

#define GL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED

#include <OpenGL/gl3.h>
#include <OpenGL/glu.h>

#endif // CAFF_ON_MAC

#ifdef CAFF_ON_WINDOWS

#ifndef WIN32_LEAN_AND_MEAN   // Stops a lot of files being included.
#define WIN32_LEAN_AND_MEAN
#endif

#ifndef _WIN32_WINNT
#define _WIN32_WINNT   0x0501 // XP
#endif

#ifndef NOMENUS
#define NOMENUS
#endif

#ifndef NOSCROLL  // Scrolling stuff.
#define NOSCROLL
#endif

#ifndef NOHELP    // Help engine interface.
#define NOHELP
#endif

#ifndef NOMCX   // Modem config.
#define NOMCX
#endif

#ifndef NOMINMAX  // Definatly want this one.
#define NOMINMAX  // Uses macros for min and max, conflicts with using namespace std people.
#endif

#ifndef CAFF_UNIT_TEST

/*#define GLEW_STATIC*/

#include <Windows.h>
#include <GL/GLew.h>

#endif

#undef near
#undef far

#pragma warning(disable : 4068)

#endif // CAFF_ON_WINDOWS


// Util Methods
// Temp move to util and make based on compiler
#ifdef CAFF_ON_WINDOWS
#define ALIGN(byteAlign, code) __declspec(align(byteAlign)) code;
#elif defined(CAFF_ON_NIX)

#endif


// **** Namespaces Alias's **** //

namespace Caffeine {

namespace Application	{}
namespace Services		{}
namespace Systems		{ namespace Properties {} }
namespace Interface     {}
namespace Event			{}
namespace Utilities		{}
namespace Math			{}
namespace Components	{}
namespace Entity		{}
namespace Library		{}
namespace ScriptGlue	{}

} // namespace

namespace Caff		= ::Caffeine;
namespace CaffApp	= ::Caffeine::Application;
namespace CaffServ	= ::Caffeine::Services;
namespace CaffSys	= ::Caffeine::Systems;
namespace CaffProp	= ::Caffeine::Systems::Properties;
namespace CaffInter = ::Caffeine::Interface;
namespace CaffEvent = ::Caffeine::Event;
namespace CaffUtil	= ::Caffeine::Utilities;
namespace CaffMath	= ::Caffeine::Math;
namespace CaffComp	= ::Caffeine::Components;
namespace CaffEnt	= ::Caffeine::Entity;
namespace CaffLib	= ::Caffeine::Library;
namespace CaffGlue  = ::Caffeine::ScriptGlue;


namespace caffeine {
namespace application {}
namespace utilities {}
}

namespace caff          = ::caffeine;
namespace caff_app      = ::caffeine::application;
namespace caff_util     = ::caffeine::utilities;

#endif // include guard
