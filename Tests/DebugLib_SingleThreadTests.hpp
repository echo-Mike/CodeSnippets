#pragma once
/// STD
#include <iostream>
#include <strstream>
#include <exception>
// CodeSnippets
#include <DebugLib/mDebugLib.hpp>

namespace DebugLibTests
{
	struct OutputCounter
	{

		template < typename T >
		OutputCounter& operator<<(T)
		{
			++count;
			return *this;
		}

		OutputCounter& operator<<(const char*)
		{
			++count;
			return *this;
		}

		template <>
		OutputCounter& operator<<<FlushTag>(FlushTag)
		{
			manipulatorFlag = true;
			return *this;
		}

		void clear()
		{
			count = 0;
			manipulatorFlag = false;
		}

		std::size_t count = 0;
		bool manipulatorFlag = false;
	};

}

#define TEST_PASSED(cond) if(!(cond)) throw 1

#define TEST_IF(cond, fail, success) \
if (!(cond)) { \
	fail \
} else { \
	success \
}

#define TEST_RESULT(name) std::size_t name = 0

#define AUTO_TEST_RESULT_NAME result

#define AUTO_TEST_INCREMENT ++AUTO_TEST_RESULT_NAME

#define AUTO_TEST_GET_FIXTURE(funcName) funcName##_fixture

#define AUTO_TEST_CASE(funcName, testCount, fixture) \
static const std::size_t funcName##_test_count = testCount;\
std::size_t funcName() \
{ \
	TEST_RESULT(AUTO_TEST_RESULT_NAME); \
	fixture AUTO_TEST_GET_FIXTURE(funcName);\
	LOG("Starting test Total test count: " #testCount)

#define AUTO_TEST(testNumber, expression) \
try { \
expression \
} \
catch (const ::std::exception& e) {\
	LOG("\nTest " #testNumber " not passed.")\
	LOG("\tException message: %s", e.what())\
}\
catch (...) { \
	LOG("\nTest " #testNumber " not passed.") \
}

#define AUTO_TEST_CASE_END \
    LOG("Passed tests count: %d\n", AUTO_TEST_RESULT_NAME) \
	return AUTO_TEST_RESULT_NAME; \
}

#define REGISTER_TEST(funcName, tottal, passed) \
tottal += funcName##_test_count; \
passed += funcName()