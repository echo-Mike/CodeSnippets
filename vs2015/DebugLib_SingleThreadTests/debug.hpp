#pragma once
#define DEBUG
#define DEBUG_LIB_TEST
namespace DebugLibTests
{
	struct FlushTag {};
}
#define DEBUG_LIB_FLUSH ::DebugLibTests::FlushTag()