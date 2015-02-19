
#ifndef CAFFEINE_UTILITIES_LOGGING_INCLUDED
#define CAFFEINE_UTILITIES_LOGGING_INCLUDED


/*
 Logging
 --
 You can log out to various sources. However by default all logging is off. It must be set using the SetLogLevel(), to say what you want to log.
 and SetOutputLogTargets(), for where you want the log to go.
 
 */


#include <Caffeine/Common/Platform.hpp>
#include <string>
#include <vector>
#include <iostream>



namespace Caffeine {
namespace Utilities {

namespace LogLevel {

enum
{
	INFO_LOG		= 1 << 0,
	WARNING_LOG		= 1 << 1,
	ERROR_LOG		= 1 << 2,
};

}  // namespace LogLevel

namespace LogOutput {

enum
{
	CONSOLE     = 1 << 0,
    BUFFER      = 1 << 1,
	FILE        = 1 << 2,
};

} // namespace

    

// Set where and what to log
void SetLogLevel(const uint32_t logLevelFlags);
void SetOutputLogTargets(const uint32_t outputsFlags);


    
// Log info for the various log types.
void LogInfo(const std::string &log);
void LogWarning(const std::string &log);
void LogError(const std::string &log);
    
    
    
// Log buffer, this allows you to use it for an in-game console or dump.
std::vector<std::string> GetLogBuffer();


} // namespace
} // namespace


// Only for use during loading. this is not subject to
// settings in the application.xml
#define LOADING_LOG(msg)								\
		std::cout << "Loading: " << msg << std::endl;	\


// Stuff to help logs look less balls
#define LOG_LINE_SEPERATOR								\
		std::cout << "-------" << std::endl;			\


#define LOG_BREAK_SEPERATOR								\
		std::cout << std::endl;							\


#define DEBUG_LOG(msg)									\
		std::cout << msg << std::endl;					\


#define ERROR_LOG(msg)									\
		std::cout << "Error: " << msg << std::endl;		\


#define WARNING_LOG(msg)								\
		std::cout << "Warning: " << msg << std::endl;	\


// Only pass string literals to this.
#ifndef NDEBUG

#define SDL_ERROR(msg)																			\
{																								\
	const std::string sdlError = SDL_GetError();												\
																								\
	if(!sdlError.empty())																		\
	{																							\
		const std::string errorID  = "SDL Error: ";												\
		const std::string callMsg  = msg;														\
		const std::string line	   = std::to_string(__LINE__);									\
																								\
		SDL_ClearError();																		\
		std::cout << errorID + sdlError + " : Line " + line + " : " + callMsg << std::endl;		\
	}																							\
}																								\

#else

#define SDL_ERROR(msg)

#endif



// Only pass string literals to this
#ifndef NDEBUG

#define GL_ERROR(msg)																			\
{																								\
																								\
	GLenum glErrorCode = glGetError();															\
																								\
	if(glErrorCode != GL_NO_ERROR)																\
	{																							\
		const std::string errorID = "OpenGL Error: ";											\
		const std::string callMsg = msg;														\
		const std::string line	  = std::to_string(__LINE__);									\
		const std::string glError = std::to_string(glErrorCode);								\
																								\
		std::cout << errorID + glError + " : Line " + line + " : " + callMsg << std::endl;		\
	}																							\
}																								\

#else

#define GL_ERROR(msg)

#endif


#endif // include guard