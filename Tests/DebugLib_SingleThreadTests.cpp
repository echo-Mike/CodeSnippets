#include "DebugLib_SingleThreadTests.hpp"

AUTO_TEST_CASE(LogLevelFunctionsTest, 4, ::DebugLib::Level)
	AUTO_TEST(1,
	{
		AUTO_TEST_GET_FIXTURE(LogLevelFunctionsTest) = ::DebugLib::GetGlobalLogLevel();
		TEST_PASSED(AUTO_TEST_GET_FIXTURE(LogLevelFunctionsTest) == DEBUG_LIB_DEFAULT_LOG_LEVEL);
		AUTO_TEST_INCREMENT;
	})
	AUTO_TEST(2,
	{
		TEST_PASSED(::DebugLib::try_SetGlobalLogLevel(::DebugLib::Level::Nothing));
		AUTO_TEST_INCREMENT;
		AUTO_TEST_GET_FIXTURE(LogLevelFunctionsTest) = ::DebugLib::GetGlobalLogLevel();
		TEST_PASSED(AUTO_TEST_GET_FIXTURE(LogLevelFunctionsTest) == ::DebugLib::Level::Nothing);
		AUTO_TEST_INCREMENT;
	})
	AUTO_TEST(3,
	{
		::DebugLib::SetGlobalLogLevel(::DebugLib::Level::All);
		AUTO_TEST_GET_FIXTURE(LogLevelFunctionsTest) = ::DebugLib::GetGlobalLogLevel();
		TEST_PASSED(AUTO_TEST_GET_FIXTURE(LogLevelFunctionsTest) == ::DebugLib::Level::All);
		AUTO_TEST_INCREMENT;
	})
AUTO_TEST_CASE_END

AUTO_TEST_CASE(WriteMacroTests, 5, ::DebugLibTests::OutputCounter)
#define DEBUG_OUT AUTO_TEST_GET_FIXTURE(WriteMacroTests)
	AUTO_TEST(1,
	{
		DEBUG_WRITE1(1);
		TEST_PASSED(AUTO_TEST_GET_FIXTURE(WriteMacroTests).count == 1);
		AUTO_TEST_INCREMENT;
		AUTO_TEST_GET_FIXTURE(WriteMacroTests).clear();
	})
	AUTO_TEST(2,
	{
		DEBUG_WRITE2(1,2);
		TEST_PASSED(AUTO_TEST_GET_FIXTURE(WriteMacroTests).count == 2);
		AUTO_TEST_INCREMENT;
		AUTO_TEST_GET_FIXTURE(WriteMacroTests).clear();
	})
	AUTO_TEST(3,
	{
		DEBUG_WRITE3(1,2,3);
		TEST_PASSED(AUTO_TEST_GET_FIXTURE(WriteMacroTests).count == 3);
		AUTO_TEST_INCREMENT;
		AUTO_TEST_GET_FIXTURE(WriteMacroTests).clear();
	})
	AUTO_TEST(4,
	{
		DEBUG_WRITE4(1,2,3,4);
		TEST_PASSED(AUTO_TEST_GET_FIXTURE(WriteMacroTests).count == 4);
		AUTO_TEST_INCREMENT;
		AUTO_TEST_GET_FIXTURE(WriteMacroTests).clear();
	})
	AUTO_TEST(5,
	{
		DEBUG_WRITE5(1,2,3,4,5);
		TEST_PASSED(AUTO_TEST_GET_FIXTURE(WriteMacroTests).count == 5);
		AUTO_TEST_INCREMENT;
		AUTO_TEST_GET_FIXTURE(WriteMacroTests).clear();
	})
AUTO_TEST_CASE_END

AUTO_TEST_CASE(VariadicWriteMacroTests, 5, ::DebugLibTests::OutputCounter)
#define DEBUG_OUT AUTO_TEST_GET_FIXTURE(VariadicWriteMacroTests)
	AUTO_TEST(1,
	{
		DEBUG_WRITE(1);
		TEST_PASSED(AUTO_TEST_GET_FIXTURE(VariadicWriteMacroTests).count == 1);
		AUTO_TEST_INCREMENT;
		AUTO_TEST_GET_FIXTURE(VariadicWriteMacroTests).clear();
	})
	AUTO_TEST(2,
	{
		DEBUG_WRITE(1,2);
		TEST_PASSED(AUTO_TEST_GET_FIXTURE(VariadicWriteMacroTests).count == 2);
		AUTO_TEST_INCREMENT;
		AUTO_TEST_GET_FIXTURE(VariadicWriteMacroTests).clear();
	})
	AUTO_TEST(3,
	{
		DEBUG_WRITE(1,2,3);
		TEST_PASSED(AUTO_TEST_GET_FIXTURE(VariadicWriteMacroTests).count == 3);
		AUTO_TEST_INCREMENT;
		AUTO_TEST_GET_FIXTURE(VariadicWriteMacroTests).clear();
	})
	AUTO_TEST(4,
	{
		DEBUG_WRITE(1,2,3,4);
		TEST_PASSED(AUTO_TEST_GET_FIXTURE(VariadicWriteMacroTests).count == 4);
		AUTO_TEST_INCREMENT;
		AUTO_TEST_GET_FIXTURE(VariadicWriteMacroTests).clear();
	})
	AUTO_TEST(5,
	{
		DEBUG_WRITE(1,2,3,4,5);
		TEST_PASSED(AUTO_TEST_GET_FIXTURE(VariadicWriteMacroTests).count == 5);
		AUTO_TEST_INCREMENT;
		AUTO_TEST_GET_FIXTURE(VariadicWriteMacroTests).clear();
	})
AUTO_TEST_CASE_END

AUTO_TEST_CASE(PrintMacroTests, 4, ::DebugLibTests::OutputCounter)
#define DEBUG_OUT AUTO_TEST_GET_FIXTURE(PrintMacroTests)
	AUTO_TEST(1,
	{
		DEBUG_PRINT1(1);
		TEST_PASSED(AUTO_TEST_GET_FIXTURE(PrintMacroTests).count == 2);
		AUTO_TEST_INCREMENT;
		AUTO_TEST_GET_FIXTURE(PrintMacroTests).clear();
	})
	AUTO_TEST(2,
	{
		DEBUG_PRINT2(1,2);
		TEST_PASSED(AUTO_TEST_GET_FIXTURE(PrintMacroTests).count == 3);
		AUTO_TEST_INCREMENT;
		AUTO_TEST_GET_FIXTURE(PrintMacroTests).clear();
	})
	AUTO_TEST(3,
	{
		DEBUG_PRINT3(1,2,3);
		TEST_PASSED(AUTO_TEST_GET_FIXTURE(PrintMacroTests).count == 4);
		AUTO_TEST_INCREMENT;
		AUTO_TEST_GET_FIXTURE(PrintMacroTests).clear();
	})
	AUTO_TEST(4,
	{
		DEBUG_PRINT4(1,2,3,4);
		TEST_PASSED(AUTO_TEST_GET_FIXTURE(PrintMacroTests).count == 5);
		AUTO_TEST_INCREMENT;
		AUTO_TEST_GET_FIXTURE(PrintMacroTests).clear();
	})
AUTO_TEST_CASE_END

AUTO_TEST_CASE(VariadicPrintMacroTests, 4, ::DebugLibTests::OutputCounter)
#define DEBUG_OUT AUTO_TEST_GET_FIXTURE(VariadicPrintMacroTests)
	AUTO_TEST(1,
	{
		DEBUG_PRINT(1);
		TEST_PASSED(AUTO_TEST_GET_FIXTURE(VariadicPrintMacroTests).count == 2);
		AUTO_TEST_INCREMENT;
		AUTO_TEST_GET_FIXTURE(VariadicPrintMacroTests).clear();
	})
	AUTO_TEST(2,
	{
		DEBUG_PRINT(1,2);
		TEST_PASSED(AUTO_TEST_GET_FIXTURE(VariadicPrintMacroTests).count == 3);
		AUTO_TEST_INCREMENT;
		AUTO_TEST_GET_FIXTURE(VariadicPrintMacroTests).clear();
	})
	AUTO_TEST(3,
	{
		DEBUG_PRINT(1,2,3);
		TEST_PASSED(AUTO_TEST_GET_FIXTURE(VariadicPrintMacroTests).count == 4);
		AUTO_TEST_INCREMENT;
		AUTO_TEST_GET_FIXTURE(VariadicPrintMacroTests).clear();
	})
	AUTO_TEST(4,
	{
		DEBUG_PRINT(1,2,3,4);
		TEST_PASSED(AUTO_TEST_GET_FIXTURE(VariadicPrintMacroTests).count == 5);
		AUTO_TEST_INCREMENT;
		AUTO_TEST_GET_FIXTURE(VariadicPrintMacroTests).clear();
	})
AUTO_TEST_CASE_END

AUTO_TEST_CASE(InfoMessageTest, 4, ::DebugLibTests::OutputCounter)
#define DEBUG_OUT AUTO_TEST_GET_FIXTURE(InfoMessageTest)
	AUTO_TEST(1,
	{
		DEBUG_INFO_MESSAGE
		DEBUG_END_MESSAGE
		TEST_PASSED(AUTO_TEST_GET_FIXTURE(InfoMessageTest).count == 2);
		TEST_PASSED(AUTO_TEST_GET_FIXTURE(InfoMessageTest).manipulatorFlag);
		AUTO_TEST_INCREMENT;
		AUTO_TEST_GET_FIXTURE(InfoMessageTest).clear();
	})
	AUTO_TEST(2,
	{
		DEBUG_INFO_MESSAGE
			DEBUG_WRITE(1);
		DEBUG_END_MESSAGE
		TEST_PASSED(AUTO_TEST_GET_FIXTURE(InfoMessageTest).count == 3);
		TEST_PASSED(AUTO_TEST_GET_FIXTURE(InfoMessageTest).manipulatorFlag);
		AUTO_TEST_INCREMENT;
		AUTO_TEST_GET_FIXTURE(InfoMessageTest).clear();
	})
	AUTO_TEST(3,
	{
		DEBUG_INFO_MESSAGE
			DEBUG_PRINT(1);
		DEBUG_END_MESSAGE
		TEST_PASSED(AUTO_TEST_GET_FIXTURE(InfoMessageTest).count == 4);
		TEST_PASSED(AUTO_TEST_GET_FIXTURE(InfoMessageTest).manipulatorFlag);
		AUTO_TEST_INCREMENT;
		AUTO_TEST_GET_FIXTURE(InfoMessageTest).clear();
	})
	AUTO_TEST(4,
	{
		::DebugLib::SetGlobalLogLevel(::DebugLib::Level::Warning);
		DEBUG_INFO_MESSAGE
			DEBUG_PRINT(1);
		DEBUG_END_MESSAGE
		TEST_PASSED(AUTO_TEST_GET_FIXTURE(InfoMessageTest).count == 0);
		TEST_PASSED(!AUTO_TEST_GET_FIXTURE(InfoMessageTest).manipulatorFlag);
		AUTO_TEST_INCREMENT;
		AUTO_TEST_GET_FIXTURE(InfoMessageTest).clear();
	})
AUTO_TEST_CASE_END

AUTO_TEST_CASE(WarningMessageTest, 4, ::DebugLibTests::OutputCounter)
#define DEBUG_OUT AUTO_TEST_GET_FIXTURE(WarningMessageTest)
	AUTO_TEST(1,
	{
		DEBUG_WARNING_MESSAGE
		DEBUG_END_MESSAGE
		TEST_PASSED(AUTO_TEST_GET_FIXTURE(WarningMessageTest).count == 2);
		TEST_PASSED(AUTO_TEST_GET_FIXTURE(WarningMessageTest).manipulatorFlag);
		AUTO_TEST_INCREMENT;
		AUTO_TEST_GET_FIXTURE(WarningMessageTest).clear();
	})
	AUTO_TEST(2,
	{
		DEBUG_WARNING_MESSAGE
			DEBUG_WRITE(1);
		DEBUG_END_MESSAGE
		TEST_PASSED(AUTO_TEST_GET_FIXTURE(WarningMessageTest).count == 3);
		TEST_PASSED(AUTO_TEST_GET_FIXTURE(WarningMessageTest).manipulatorFlag);
		AUTO_TEST_INCREMENT;
		AUTO_TEST_GET_FIXTURE(WarningMessageTest).clear();
	})
	AUTO_TEST(3,
	{
		DEBUG_WARNING_MESSAGE
			DEBUG_PRINT(1);
		DEBUG_END_MESSAGE
		TEST_PASSED(AUTO_TEST_GET_FIXTURE(WarningMessageTest).count == 4);
		TEST_PASSED(AUTO_TEST_GET_FIXTURE(WarningMessageTest).manipulatorFlag);
		AUTO_TEST_INCREMENT;
		AUTO_TEST_GET_FIXTURE(WarningMessageTest).clear();
	})
	AUTO_TEST(4,
	{
		::DebugLib::SetGlobalLogLevel(::DebugLib::Level::Error);
		DEBUG_WARNING_MESSAGE
			DEBUG_PRINT(1);
		DEBUG_END_MESSAGE
		TEST_PASSED(AUTO_TEST_GET_FIXTURE(WarningMessageTest).count == 0);
		TEST_PASSED(!AUTO_TEST_GET_FIXTURE(WarningMessageTest).manipulatorFlag);
		AUTO_TEST_INCREMENT;
		AUTO_TEST_GET_FIXTURE(WarningMessageTest).clear();
	})
AUTO_TEST_CASE_END

AUTO_TEST_CASE(ErrorMessageTest, 4, ::DebugLibTests::OutputCounter)
#define DEBUG_OUT AUTO_TEST_GET_FIXTURE(ErrorMessageTest)
	AUTO_TEST(1,
	{
		DEBUG_ERROR_MESSAGE
		DEBUG_END_MESSAGE
		TEST_PASSED(AUTO_TEST_GET_FIXTURE(ErrorMessageTest).count == 2);
		TEST_PASSED(AUTO_TEST_GET_FIXTURE(ErrorMessageTest).manipulatorFlag);
		AUTO_TEST_INCREMENT;
		AUTO_TEST_GET_FIXTURE(ErrorMessageTest).clear();
	})
	AUTO_TEST(2,
	{
		DEBUG_ERROR_MESSAGE
			DEBUG_WRITE(1);
		DEBUG_END_MESSAGE
		TEST_PASSED(AUTO_TEST_GET_FIXTURE(ErrorMessageTest).count == 3);
		TEST_PASSED(AUTO_TEST_GET_FIXTURE(ErrorMessageTest).manipulatorFlag);
		AUTO_TEST_INCREMENT;
		AUTO_TEST_GET_FIXTURE(ErrorMessageTest).clear();
	})
	AUTO_TEST(3,
	{
		DEBUG_ERROR_MESSAGE
			DEBUG_PRINT(1);
		DEBUG_END_MESSAGE
		TEST_PASSED(AUTO_TEST_GET_FIXTURE(ErrorMessageTest).count == 4);
		TEST_PASSED(AUTO_TEST_GET_FIXTURE(ErrorMessageTest).manipulatorFlag);
		AUTO_TEST_INCREMENT;
		AUTO_TEST_GET_FIXTURE(ErrorMessageTest).clear();
	})
	AUTO_TEST(4,
	{
		::DebugLib::SetGlobalLogLevel(::DebugLib::Level::User);
		DEBUG_ERROR_MESSAGE
			DEBUG_PRINT(1);
		DEBUG_END_MESSAGE
		TEST_PASSED(AUTO_TEST_GET_FIXTURE(ErrorMessageTest).count == 0);
		TEST_PASSED(!AUTO_TEST_GET_FIXTURE(ErrorMessageTest).manipulatorFlag);
		AUTO_TEST_INCREMENT;
		AUTO_TEST_GET_FIXTURE(ErrorMessageTest).clear();
	})
AUTO_TEST_CASE_END

AUTO_TEST_CASE(UserMessageTest, 4, ::DebugLibTests::OutputCounter)
#define DEBUG_OUT AUTO_TEST_GET_FIXTURE(UserMessageTest)
	AUTO_TEST(1,
	{
		DEBUG_NEW_MESSAGE(1)
		DEBUG_END_MESSAGE
		TEST_PASSED(AUTO_TEST_GET_FIXTURE(UserMessageTest).count == 2);
		TEST_PASSED(AUTO_TEST_GET_FIXTURE(UserMessageTest).manipulatorFlag);
		AUTO_TEST_INCREMENT;
		AUTO_TEST_GET_FIXTURE(UserMessageTest).clear();
	})
	AUTO_TEST(2,
	{
		DEBUG_NEW_MESSAGE(1,2)
			DEBUG_WRITE(1);
		DEBUG_END_MESSAGE
		TEST_PASSED(AUTO_TEST_GET_FIXTURE(UserMessageTest).count == 4);
		TEST_PASSED(AUTO_TEST_GET_FIXTURE(UserMessageTest).manipulatorFlag);
		AUTO_TEST_INCREMENT;
		AUTO_TEST_GET_FIXTURE(UserMessageTest).clear();
	})
	AUTO_TEST(3,
	{
		DEBUG_NEW_MESSAGE(1,2,3)
			DEBUG_PRINT(1);
		DEBUG_END_MESSAGE
		TEST_PASSED(AUTO_TEST_GET_FIXTURE(UserMessageTest).count == 6);
		TEST_PASSED(AUTO_TEST_GET_FIXTURE(UserMessageTest).manipulatorFlag);
		AUTO_TEST_INCREMENT;
		AUTO_TEST_GET_FIXTURE(UserMessageTest).clear();
	})
	AUTO_TEST(4,
	{
		::DebugLib::SetGlobalLogLevel(::DebugLib::Level::Nothing);
		DEBUG_NEW_MESSAGE(1,2,3,4)
			DEBUG_PRINT(1);
		DEBUG_END_MESSAGE
		TEST_PASSED(AUTO_TEST_GET_FIXTURE(UserMessageTest).count == 0);
		TEST_PASSED(!AUTO_TEST_GET_FIXTURE(UserMessageTest).manipulatorFlag);
		AUTO_TEST_INCREMENT;
		AUTO_TEST_GET_FIXTURE(UserMessageTest).clear();
	})
AUTO_TEST_CASE_END

int main(void)
{
	std::size_t tottal_tests_count = 0;
	std::size_t passed_tests_count = 0;
	//*
	REGISTER_TEST(LogLevelFunctionsTest,	tottal_tests_count, passed_tests_count);
	REGISTER_TEST(WriteMacroTests,			tottal_tests_count, passed_tests_count);
	REGISTER_TEST(VariadicWriteMacroTests,	tottal_tests_count, passed_tests_count);
	REGISTER_TEST(PrintMacroTests,			tottal_tests_count, passed_tests_count);
	REGISTER_TEST(VariadicPrintMacroTests,	tottal_tests_count, passed_tests_count);
	REGISTER_TEST(InfoMessageTest,			tottal_tests_count, passed_tests_count);
	REGISTER_TEST(WarningMessageTest,		tottal_tests_count, passed_tests_count);
	REGISTER_TEST(ErrorMessageTest,			tottal_tests_count, passed_tests_count);
	REGISTER_TEST(UserMessageTest,			tottal_tests_count, passed_tests_count);
	LOG("Total tests passed %d out of %d",	passed_tests_count, tottal_tests_count)
	//*/
	//std::cout << "All tests done. Total passed tests " << passed_tests_count << " out of " << tottal_tests_count << ". Press ENTER to exit.";
	//std::cin.get();
	return tottal_tests_count == passed_tests_count ? 0 : 2;
}