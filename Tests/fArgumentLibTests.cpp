#include <iostream>
#include <cstdio>
#define TEST
#include "ArgumentsLib/fArgumentsLib.hpp"
using namespace Arguments;

#define TEST_PASSED(cond) if(!(cond)) throw 1;

#define TEST_CASE(testName) \
std::size_t testName() \
{ \
    std::size_t result = 0;

#define TEST_SUCCESS ++result;

#define RUN_TEST(testName) testName();



int main(int argc, char* argv[])
{
    std::size_t result = 0, test_count = 0;

    LOG("\nTotal tests passed %d out of %d", result, test_count)
    std::cout << "All tests done. Total passed tests " << result << " out of " << test_count << ". Press ENTER to exit.";
    std::cin.get();
    return 0;
}