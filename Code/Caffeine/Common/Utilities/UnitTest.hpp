
#ifndef CAFFEINE_UNIT_TEST_INCLUDED
#define CAFFEINE_UNIT_TEST_INCLUDED


#include <vector>
#include <iostream>
#include <sstream>
#include <Caffeine/Utilities/StringHelpers.hpp>


namespace Caffeine {
namespace Utilities {


class TestUnit
{
public:
	virtual ~TestUnit() {};

	virtual void setup() {};
	virtual void tearDown() {};

}; // class


class Test
{
public:
	
	explicit Test(const std::string &name)
		: m_testName(name)
	{}

	inline std::string getName() { return m_testName; }
	virtual void run() = 0;
	virtual void process() = 0;

private:

	const std::string m_testName;

};


class TestApp
{

	explicit TestApp()
	{

	}


public:


	static TestApp& instance()
	{
		static TestApp tests;
		return tests;
	}


	void addTest(Test *test)
	{
		m_tests.push_back(test);
	}


	void runTests()
	{
		for(const auto &test : m_tests)
		{
			m_errorMessages = "";
			std::cout << test->getName() << ": ";
			test->process();
			std::cout << "\n";
			if (!m_errorMessages.empty())
			{
				std::cout << m_errorMessages;
			}

			std::cout << "--\n";
			
		}
	}


	void assertPassed(const std::string &output)
	{
		std::cout << output;
	}

	void assertFailed(const std::string &output, const std::string &message)
	{
		std::cout << output;
		m_errorMessages += message + "\n";
	}


private:

	std::vector<Test*> m_tests;
	std::string m_errorMessages;

};


 #define TEST(unit_test_name, test_name)											\
struct Test##test_name : public Caffeine::Utilities::Test, public unit_test_name 	\
{ 																					\
																					\
	explicit Test##test_name()														\
	: Caffeine::Utilities::Test(#test_name)											\
	{																				\
		Caffeine::Utilities::TestApp::instance().addTest(this); 					\
	}																				\
																					\
	void process() 																	\
	{ 																				\
		setup(); 																	\
		run(); 																		\
		tearDown(); 																\
	}																				\
																					\
	void run();																		\
																					\
} instanceTest##test_name; 															\
																					\
void Test##test_name::run() 														\



template<typename T>
void assertIsEqual(T a, T b)
{
	if(a == b)
	{
		TestApp::instance().assertPassed(".");
	}
	else
	{
		std::stringstream ss;
		ss << "assertIsEqual() failed with values ";
		ss << a;
		ss << " and ";
		ss << b;

		TestApp::instance().assertFailed("[Fail]", ss.str());
	}
}


template<typename T>
void assertIsTrue(T a)
{
	if (a)
	{
		TestApp::instance().assertPassed(".");
	}
	else
	{
		std::stringstream ss;
		ss << "assertIsTrue() failed with value ";
		ss << a;

		TestApp::instance().assertFailed("[Fail]", ss.str());
	}
}


template<typename T>
void assertIsFalse(T a)
{
	if (!a)
	{
		TestApp::instance().assertPassed(".");
	}
	else
	{
		std::stringstream ss;
		ss << "assertIsFalse() failed with value ";
		ss << a;

		TestApp::instance().assertFailed("[Fail]", ss.str());
	}
}


} // namespace
} // namespace


#endif // include
