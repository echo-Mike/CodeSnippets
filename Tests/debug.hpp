#pragma once
#define DEBUG
#define DEBUG_LIB_TEST

#ifdef DEBUG_LIB_TEST_MT
#   define DEBUG_LIB_THREAD_SAFETY
#   define DEBUG_LIB_FILE_LOG
#   define DEBUG_LIB_LOG_FILE_NAME "DebugLib_MT.log"
#else
namespace DebugLibTests
{
	struct FlushTag {};
}
#   define DEBUG_LIB_FLUSH ::DebugLibTests::FlushTag()
#endif