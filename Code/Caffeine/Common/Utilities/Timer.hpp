
#ifndef CAFFEINE_UTILITIES_TIMER_INCLUDED
#define CAFFEINE_UTILITIES_TIMER_INCLUDED

namespace Caffeine {
namespace Utilities {

typedef long long int Milliseconds;

float GetElapsedTime();

class Timer // : private Noncopyable
{
public:

	explicit		Timer();
	
	void			start();
	Milliseconds	split();
	Milliseconds	currentResult();
	void			stop();
	
private:

	Milliseconds	m_counter;
	Milliseconds	m_start;
	Milliseconds	m_end;
	Milliseconds	m_split;

};

} // namespace
} // namespace

#endif // include guard
