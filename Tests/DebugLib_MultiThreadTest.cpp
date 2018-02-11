#include "DebugLib_MultiThreadTest.hpp"
#define TREAD_WORK_SIZE ((unsigned long long)1 << 16)

void info_worker() 
{
	for (std::size_t i = 0; i < TREAD_WORK_SIZE; ++i)
		DEBUG_INFO_MESSAGE
			DEBUG_PRINT("\tINFO message");
		DEBUG_END_MESSAGE
}

void warning_worker() 
{
	for (std::size_t i = 0; i < TREAD_WORK_SIZE; ++i)
		DEBUG_WARNING_MESSAGE
			DEBUG_PRINT("\tWARNING message");
		DEBUG_END_MESSAGE
}

void error_worker()
{
	for (std::size_t i = 0; i < TREAD_WORK_SIZE; ++i)
		DEBUG_ERROR_MESSAGE
			DEBUG_PRINT("\tERROR message");
		DEBUG_END_MESSAGE
}

void user_worker()
{
	for (std::size_t i = 0; i < TREAD_WORK_SIZE; ++i)
		DEBUG_NEW_MESSAGE("USER::" __FILE__ ":"  DEBUG_LIB_AS_C_STRING(__LINE__))
			DEBUG_PRINT("\tUSER message");
		DEBUG_END_MESSAGE
}

void allTest() 
{
	std::array< std::future<void>, 4 > tasks;
	tasks[0] = std::async(std::launch::async, info_worker);
	tasks[1] = std::async(std::launch::async, warning_worker);
	tasks[2] = std::async(std::launch::async, error_worker);
	tasks[3] = std::async(std::launch::async, user_worker);

	for (size_t i = 0; i < 4; i++)
	{
		tasks[i].get();
	}
}

void nooneTest()
{
	DEBUG_NEW_MESSAGE("#Test message")
		DEBUG_PRINT("#Starting test with all threads disabled");
	DEBUG_END_MESSAGE

	::DebugLib::SetGlobalLogLevel(DebugLib::Level::Nothing);

	std::array< std::future<void>, 4 > tasks;
	tasks[0] = std::async(std::launch::async, info_worker);
	tasks[1] = std::async(std::launch::async, warning_worker);
	tasks[2] = std::async(std::launch::async, error_worker);
	tasks[3] = std::async(std::launch::async, user_worker);

	for (size_t i = 0; i < 4; i++)
	{
		tasks[i].get();
	}
}

int main(void)
{
	allTest();
	nooneTest();
	return 0;
}