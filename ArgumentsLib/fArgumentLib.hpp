#ifndef ARGUMENT_LIB_HPP_
#define ARGUMENT_LIB_HPP_ "0.0.0@fArgumentLib.hpp"
/**
 * DESCRIPTION:
 *     Module contains implementation of argument parser and helper structures.
 * AUTHOR:
 *     Mikhail Demchenko
 *     mailto:dev.echo.mike@gmail.com
 *     https://github.com/echo-Mike
 */

/**
 * MIT License
 *
 * Copyright (c) 2017-2018 Mikhail Demchenko dev.echo.mike@gmail.com
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

// STD
#include <cstring>

#ifdef TEST
    #include <cstdio>
    struct FArgumentLibLoggerSingleton
    {
        static std::FILE* getInstance(const char* path)
        {
            static FArgumentLibLoggerSingleton s(path);
            return s.h;
        }
      private:
        std::FILE* h;
        FArgumentLibLoggerSingleton(const char* path) : h(std::fopen(path,"w+")) {}
        ~FArgumentLibLoggerSingleton()
        {
            std::fflush(h);
            std::fclose(h);
        }
    };
#endif

#if defined(TEST) && !defined(LOG)
#define LOG(format, ...) \
do { \
    std::fprintf(FArgumentLibLoggerSingleton::getInstance(".\\fArgumentLibTestLog.txt"), "[%-35s: %-25s: line:%-4d] " format "\n", __FILE__, __FUNCTION__, __LINE__, ## __VA_ARGS__); \
} while(0);
#endif

/**
 * @namespace Arguments
 * @brief Main namespace of argument parser library
 */
namespace Arguments 
{
    // Set of possible option prefixes
    static const char* opt_ionPrefixes = "-/";

    // Set of types of options
    enum class ArgTypes : char
    {
        Null = 0, //Type of last option in option list
        Flag,     //The flag option type
        Value     //Option with leading value type
    };

    struct Option
    {
        const char* shortCommand;
        const char* longCommand;
        ArgTypes type;
        int data;
    };

    /**
     * @brief Identifies null-terminated string 'arg' as an option
     * @param[in] arg Pointer to first char of examined null-terminated string
     * @return Pointer to first option char in 'arg' or nullptr if not an option
     */
    char* isOption(char* arg)
    {
        auto size = std::strspn(arg, opt_ionPrefixes);
        return (size == 0) ? nullptr : arg + size;
    }

    /**
     * @brief Attempts to find flag argument in argument list
     * @param[in] argc Argument list length
     * @param[in] argv Argument list
     * @param[in:out] opt Option to find
     * @param[in] start Position in argument list to start with
     */
    void findFlagArg(int argc, char* argv[], Option& opt, int start = 0) 
    {
        opt.data = false;
        // Flag argument must be at maximum at the end
        if (start >= argc) 
            return;
        char* opt_ = nullptr;
        for (auto index = start; index < argc; ++index) 
        {
            opt_ = isOption(argv[index]);
            if (opt_) 
            {
                if (std::strstr(opt_, opt.shortCommand) != NULL || 
                    !std::strcmp(opt_, opt.longCommand)) 
                {
                    opt.data = true;
                    return;
                }
            }
        }
    }

    /**
     * @brief Attempts to find value argument in argument list
     * @param[in] argc Argument list length
     * @param[in] argv Argument list
     * @param[in:out] opt Option to find
     * @param[in] start Position in argument list to start with
     */
    void findValueArg(int argc, char* argv[], Option& opt, int start = 0)
    {
        opt.data = -1;
        // Value argument must have one following argument at minimum
        if (start >= argc - 1) 
            return;
        char* opt_ = nullptr;
        for (auto index = start; index < argc - 1; ++index) 
        {
            opt_ = isOption(argv[index]);
            if (opt_)
            {
                if (!std::strcmp(opt_, opt.shortCommand) || 
                    !std::strcmp(opt_, opt.longCommand)) 
                {
                    opt.data = index + 1;
                    return;
                }
            }
        }
    }

    /**
     * @brief Main argument parsing function.
     * @param[in] argc Argument list length
     * @param[in] argv Argument list
     * @param[in:out] optv Option list
     */
    void parseArgs(int argc, char* argv[], Option optv[]) 
    {
        int index = 0;
        while (optv[index].type != ArgTypes::Null) 
        {
            switch (optv[index].type)
            {
                case ArgTypes::Flag:
                    findFlagArg(argc, argv, optv[index]);
                    break;
                case ArgTypes::Value:
                    findValueArg(argc, argv, optv[index]);
                    break;
                default:
                    return;
            }
            ++index;
        }
    }
}
#endif /* ARGUMENT_LIB_HPP_ */