
#include <Caffeine/Common/Utilities/Timer.hpp>

#ifdef WIN32
#include <tchar.h>
#include <sys/types.h>
#include <sys/timeb.h>
#else
#include <sys/time.h>
#endif

// TODO: Use Caffeine platform isntead

namespace
{
	long long int GetTime()
	{
	#ifdef WIN32
	struct _timeb timebuffer;
	_ftime64_s(&timebuffer);
	return (timebuffer.time * 1000LL) + timebuffer.millitm;
	#else
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000LL) + (tv.tv_usec / 1000LL);
	#endif
	}

	long long int timeAtStartup = GetTime();
}

namespace Caffeine {
namespace Utilities {

float GetElapsedTime() {
	return (GetTime() - timeAtStartup) / 1000.0f;
}

Timer::Timer()
: m_counter(0)
, m_start(0)
, m_end(0)
, m_split(0)
{
}


void Timer::start()
{
	m_start = GetTime();
	m_split = m_start;
}


Milliseconds Timer::split()
{
	if(!m_split) {
		m_split = GetTime();
	}
	
	Milliseconds oldSplit = m_split;
	
	m_split = GetTime();
		
	return (m_split - oldSplit);
}


Milliseconds Timer::currentResult()
{
	if(m_end) {
		return m_end - m_start;
	}
	else {
		return GetTime() - m_start;
	}
}


void Timer::stop()
{
	m_end = GetTime();
}

} // namespace
} // namespace
