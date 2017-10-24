#pragma once
#ifndef DEBUG_LIB_HPP__
#define DEBUG_LIB_HPP__ "multi@mDebuglib.hpp"
/**
*	DESCRIPTION:
*		Module contains implementation of debug output macro-set.
*	AUTHOR:
*		Mikhail Demchenko
*		mailto:dev.echo.mike@gmail.com
*		https://github.com/echo-Mike
*	PAIRED WITH: 
*		Debuglib.cpp
**/
/** 
*   MIT License
*
*   Copyright (c) 2017 Mikhail Demchenko dev.echo.mike@gmail.com
*   
*   Permission is hereby granted, free of charge, to any person obtaining a copy
*   of this software and associated documentation files (the "Software"), to deal
*   in the Software without restriction, including without limitation the rights
*   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
*   copies of the Software, and to permit persons to whom the Software is
*   furnished to do so, subject to the following conditions:
*   
*   The above copyright notice and this permission notice shall be included in all
*   copies or substantial portions of the Software.
*   
*   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
*   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
*   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
*   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
*   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
*   SOFTWARE.
**/

//Create one debug.hpp file in your project with macro presets for this lib
#include "debug.hpp"

#ifdef DEBUG_LIB_FILE_LOG
	#ifndef DEBUG_OUT
		#include <fstream>
		//Output variable name macro def : to avoid name conflict
		#ifndef DEBUG_LIB_LOG_FILE_VAR_NAME
			#define DEBUG_LIB_LOG_FILE_VAR_NAME __Debug_Lib_Log
		#endif
		//Std log file path + name
		#ifndef DEBUG_LIB_LOG_FILE_NAME
			#define DEBUG_LIB_LOG_FILE_NAME "log.dat"
		#endif
		namespace DebugLib {
			extern std::ofstream DEBUG_LIB_LOG_FILE_VAR_NAME;
		}
		#define DEBUG_OUT DebugLib::DEBUG_LIB_LOG_FILE_VAR_NAME
	#endif
#else
	#include <iostream>
	//Output stream
	#ifndef DEBUG_OUT
		#define DEBUG_OUT std::clog
	#endif
#endif

#ifdef DEBUG_LIB_LOG_THREAD_SAFETY
	//Async access control
	//Output mutex name macro def : to avoid name conflict
	#ifndef DEBUG_LIB_LOG_MUTEX_VAR_NAME
		#define DEBUG_LIB_LOG_MUTEX_VAR_NAME __Debug_Lib_Log_Mutex
	#endif

	#include <mutex>
	namespace DebugLib {
		extern std::mutex DEBUG_LIB_LOG_MUTEX_VAR_NAME;
	}
#endif

//New line
#ifndef DEBUG_LIB_NEXT_LINE
	#define DEBUG_LIB_NEXT_LINE "\n"
#endif

#ifdef DEBUG
	//Debug write macro
	//Allow to output one value to debug stream without new line afterwards.
	#define DEBUG_WRITE1(x) DEBUG_OUT << (x)
	//Allow to output two values to debug stream without new line afterwards.
	#define DEBUG_WRITE2(x,y) DEBUG_OUT << (x) << (y)
	//Allow to output three values to debug stream without new line afterwards.
	#define DEBUG_WRITE3(x,y,z) DEBUG_OUT << (x) << (y) << (z)
	//Allow to output four values to debug stream without new line afterwards.
	#define DEBUG_WRITE4(x,y,z,w) DEBUG_OUT << (x) << (y) << (z) << (w)
	//Allow to output one value to debug stream with new line afterwards.
	#define DEBUG_PRINT1(x) DEBUG_OUT << (x) << DEBUG_LIB_NEXT_LINE
	//Allow to output two values to debug stream with new line afterwards
	#define DEBUG_PRINT2(x,y) DEBUG_OUT << (x) << (y) << DEBUG_LIB_NEXT_LINE
	//Allow to output three values to debug stream with new line afterwards.
	#define DEBUG_PRINT3(x,y,z) DEBUG_OUT << (x) << (y) << (z) << DEBUG_LIB_NEXT_LINE
	//Allow to output four values to debug stream with new line afterwards.
	#define DEBUG_PRINT4(x,y,z,w) DEBUG_OUT << (x) << (y) << (z) << (w) << DEBUG_LIB_NEXT_LINE
#else
	#define DEBUG_WRITE1(x) {}
	#define DEBUG_WRITE2(x,y) {}
	#define DEBUG_WRITE3(x,y,z) {}
	#define DEBUG_WRITE4(x,y,z,w) {}
	#define DEBUG_PRINT1(x) {}
	#define DEBUG_PRINT2(x,y) {}
	#define DEBUG_PRINT3(x,y,z) {}
	#define DEBUG_PRINT4(x,y,z,w) {}
#endif

//First line of new massege
#ifndef DEBUG_NEW_MESSAGE
	#ifdef DEBUG
		#ifdef DEBUG_LIB_LOG_THREAD_SAFETY
			//Output lock guard for global mutex var name macro def : to avoid name conflict
			#ifndef DEBUG_LIB_LOG_LOCK_GUARG_VAR_NAME
				#define DEBUG_LIB_LOG_LOCK_GUARG_VAR_NAME __Debug_Lib_Msg_Scope_Lg;
			#endif
			#define DEBUG_NEW_MESSAGE(x) { \
				std::lock_guard<std::mutex> DEBUG_LIB_LOG_LOCK_GUARG_VAR_NAME(DebugLib::DEBUG_LIB_LOG_MUTEX_VAR_NAME);\
				DEBUG_OUT << (x) << DEBUG_LIB_NEXT_LINE;
		#else
			#define DEBUG_NEW_MESSAGE(x) { \
				DEBUG_OUT << (x) << DEBUG_LIB_NEXT_LINE;
		#endif	
	#else
		#define DEBUG_NEW_MESSAGE(x) { while(false) {
	#endif
#endif

//End line and flush
#ifndef DEBUG_END_MESSAGE
	#ifdef DEBUG
		#define DEBUG_END_MESSAGE DEBUG_OUT << std::flush; }
	#else
		#define DEBUG_END_MESSAGE }}
	#endif
#endif

//End message, flush and exit program with 'exitcode'
#ifndef DEBUG_END_MESSAGE_AND_EXIT
	#include <cstdlib>
	#ifdef DEBUG 
		#define DEBUG_END_MESSAGE_AND_EXIT(exitcode) \
				DEBUG_OUT << std::flush; \
				std::exit((exitcode)); }
	#else
		#define DEBUG_END_MESSAGE_AND_EXIT(exitcode) \
			} std::exit((exitcode)); }
	#endif
#endif

#endif