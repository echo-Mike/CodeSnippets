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
*   Copyright (c) 2017-2018 Mikhail Demchenko dev.echo.mike@gmail.com
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
/**
*	Section: Macros that can be redefined
*	
*	In order of appearance:
*
*	DEBUG_LIB_MUTEX_VAR_NAME - defines the name to be used for the global output mutex. 
*		This name is encapsulated in DebugLib namespace. You can refer to this variable as:
*		::DebugLib::DEBUG_LIB_MUTEX_VAR_NAME
*		Default value: Debug_Lib_Main_Mutex__
*		Status: Implementation dependent
*
*	DEBUG_LIB_LOG_LOCK_GUARG_VAR_NAME - defines the name of ::std::lock_guard for inner scope.
*		This name is only accesble from Inner scope (See section: Scopes) as:
*		DEBUG_LIB_LOG_LOCK_GUARG_VAR_NAME
*		Default value: Debug_Lib_Msg_Scope_Lg__
*		Status: Implementation dependent
*
*	DEBUG_LIB_DEFAULT_LOG_LEVEL - defines the log level with which the application starts.
*		If redefined must be one of DebugLib::Level enum members.
*		Default value: ::DebugLib::Level::All
*		Status: Implementation independent
*
*	DEBUG_LIB_LOG_FILE_VAR_NAME - defines the name to be used for the output file stream if defined(DEBUG_LIB_FILE_LOG). 
*		This name is encapsulated in DebugLib namespace. You can refer to this variable as:
*		::DebugLib::DEBUG_LIB_LOG_FILE_VAR_NAME
*		Only accessible if defined(DEBUG_LIB_FILE_LOG), otherwise have no effect.
*		Default value: Debug_Lib_Log_File__
*		Status: Implementation dependent
*
*	DEBUG_LIB_LOG_FILE_NAME - defines the file name of general log file.
*		If redefined must be a C-string with valid path to file.
*		On start of application this file will be opend with std::fstream::app | std::fstream::out flags.
*		It is a user's responsibility to check state of this file stream.
*		Only accessible if defined(DEBUG_LIB_FILE_LOG)), otherwise have no effect.
*		Default value: "log.dat"
*		Status: Implementation independent
*
*	DEBUG_OUT - defines the object to which all output will be redirected.
*		Must have operator<< that accepts at least C-strings, char, any numbers(integers or floats),
*		::std::flush and returns reference to this object.
*		Default value:
*			If defined(DEBUG_LIB_FILE_LOG):
*				::DebugLib::DEBUG_LIB_LOG_FILE_VAR_NAME
*			else
*				::std::clog
*		Status: Implementation independent
*
*	DEBUG_LIB_NEXT_LINE - defines a line separator.
*		Default value: '\n'
*		Status: Implementation independent
*
*	DEBUG_INFO_MESSAGE, DEBUG_WARNING_MESSAGE, DEBUG_ERROR_MESSAGE, DEBUG_NEW_MESSAGE(...) 
*		- defines a new debug message start.
*		Any macro of start message macro set may be redefined.
*		This macros may be used directly after any of next statements:
*			for(), while(), if(), else, try, catch(), function_definition
		as if they defines the statement scope.
*		Status: Implementation independent
*
*	DEBUG_END_MESSAGE, DEBUG_END_MESSAGE_AND_EVAL(expression), DEBUG_END_MESSAGE_AND_EXIT(exitcode), DEBUG_END_MESSAGE_EVAL_AND_EXIT(exitcode, expression)
*		_ defines the end of debug message.
*		Any macro of end message macro set may be redefined.
*		Status: Implementation independent
*
*	Section: Library behaviour macros
*
*	DEBUG_LIB_THREAD_SAFETY - if defined output facility must be thread safe.
*		Including:
*			1) Global log level must be an atomic variable.
*			2) The Inner scope must be a critical section protected by mutual exclusion or other synchronization or concurrent execution safety means.
*		For default implementation the <mutex> and <atomic> headers must be available.
*		Status: Implementation independent
*
*	DEBUG_LIB_FILE_LOG - if defined the implementation must create a file output to be used in DEBUG_OUT.
*		For default implementation the <fstream> header must be available if defined(DEBUG_LIB_FILE_LOG)
*		and <iostream> if !defined(DEBUG_LIB_FILE_LOG).
*		Status: Implementation independent
*
*	DEBUG - if defined Inner scope content must be generated, otherwise it may be not generated.
*		Status: Implementation independent
*
*	Section: Compilation
*
*	This library must be compiled.
*	It supports two cases: compile with your project and compile as static library.
*	To compile with your project simply include this file to you build script and compile paired .cpp file as part of your project.
*	To compile as static library use provided build scripts and include builded library and header to build scripts of your project.
*	Generally if thread safety is not needed the library is compatible with C++98 (See section: Other information).
*	If thread safety is needed your compiler must inplement C++11 thread and atomic std libraries.
*	
*	Section: Usage guide
*
*	Use one of start message macro set to start new debug message:
*		DEBUG_INFO_MESSAGE, DEBUG_WARNING_MESSAGE, DEBUG_ERROR_MESSAGE, DEBUG_NEW_MESSAGE(...)
*
*	Then use output macro set to output your message:
*		DEBUG_PRINT[1-4](1-4 args), DEBUG_PRINT(... (1-4 args)) - to print a line (DEBUG_LIB_NEXT_LINE will be put in the end of every call)
*		DEBUG_WRITE[1-5](1-5 args), DEBUG_WRITE(... (1-5 args)) - to write a line (DEBUG_LIB_NEXT_LINE will not be put in the end of every call)
*
*	All output are directed to DEBUG_OUT macro using operator<<.
*
*	To finish your message use next macros (end message macro set):
*		DEBUG_END_MESSAGE - finish your message and send ::std::flush to output stream;
*		DEBUG_END_MESSAGE_AND_EVAL(expression) - finish your message, send ::std::flush to output stream and evaluate
*			provided expression in middle scope (See: scopes section). To evaluate a complex expression surround it with "{}". 
*			If it is a oneline expression it must be ended with ";" (See: code generation section).
*			expression is evaluated even when DEBUG macro is undefined.
*			If an expression throws no actions to catch the exception will be taken.
*		DEBUG_END_MESSAGE_AND_EXIT(exitcode) - finish your message, send ::std::flush to output stream and call
*			::std::exit((exitcode)) expression in middle scope.
*		DEBUG_END_MESSAGE_EVAL_AND_EXIT(exitcode, expression) - combination of DEBUG_END_MESSAGE_AND_EVAL and DEBUG_END_MESSAGE_AND_EXIT.
*			::std::exit((exitcode)) is called right after evaluation an expression.
*
*	Section: Example
*	
*	Information message
*	In code:
*	DEBUG_INFO_MESSAGE
*		DEBUG_PRINT1("This is an info message with one param");
*		DEBUG_PRINT2("This is an info message with two params: ", 100);
*		DEBUG_PRINT3("This is an info message with three params: ", 100, " yey!!");
*		DEBUG_PRINT( "This is an info message with one to four params: ", 100, " Yo!!", "Yey!!");
*	DEBUG_END_MESSAGE
*
*	Code generated: (comments are not generated)
*	If defined(DEBUG) && defined(DEBUG_LIB_THREAD_SAFETY):
*
*	// Outer scope
*	{	// Middle scope begin
*		if (::DebugLib::GetGlobalLogLevel() <= ::DebugLib::Level::Info )
*		{	// Inner scope begin
*			::std::lock_guard<::std::mutex> DEBUG_LIB_LOG_LOCK_GUARG_VAR_NAME(::DebugLib::DEBUG_LIB_MUTEX_VAR_NAME);
*			DEBUG_OUT << ("INFO::" __FILE__ ":" #__LINE__) << DEBUG_LIB_NEXT_LINE; // "INFO::" __FILE__ ":" #__LINE__ is merged in one c-string by compiler
*			DEBUG_OUT << ("This is an info message with one param") << DEBUG_LIB_NEXT_LINE;
*			DEBUG_OUT << ("This is an info message with two params: ") << (100) << DEBUG_LIB_NEXT_LINE;
*			DEBUG_OUT << ("This is an info message with three params: ") << (100) << (" yey!!") << DEBUG_LIB_NEXT_LINE;
*			DEBUG_OUT << ("This is an info message with one to four params: ") << (100) << (" Yo!!") << ("Yey!!") << DEBUG_LIB_NEXT_LINE;
*			DEBUG_OUT << DEBUG_LIB_FLUSH;
*		} // Inner scope end
*	} // Middle scope end
*	// Outer scope
*
*	If defined(DEBUG) && !defined(DEBUG_LIB_THREAD_SAFETY):
*
*	// Outer scope
*	{	// Middle scope begin
*		if (::DebugLib::GetGlobalLogLevel() <= ::DebugLib::Level::Info )
*		{	// Inner scope begin
*			DEBUG_OUT << ("INFO::" __FILE__ ":" #__LINE__) << DEBUG_LIB_NEXT_LINE; // "INFO::" __FILE__ ":" #__LINE__ is merged in one c-string by compiler
*			DEBUG_OUT << ("This is an info message with one param") << DEBUG_LIB_NEXT_LINE;
*			DEBUG_OUT << ("This is an info message with two params: ") << (100) << DEBUG_LIB_NEXT_LINE;
*			DEBUG_OUT << ("This is an info message with three params: ") << (100) << (" yey!!") << DEBUG_LIB_NEXT_LINE;
*			DEBUG_OUT << ("This is an info message with one to four params: ") << (100) << (" Yo!!") << ("Yey!!") << DEBUG_LIB_NEXT_LINE;
*			DEBUG_OUT << DEBUG_LIB_FLUSH;
*		} // Inner scope end
*	} // Middle scope end
*	// Outer scope
*
*	If !defined(DEBUG):
*
*	// Outer scope
*	{	// Middle scope begin
*		while (false)
*		{	// Inner scope begin
*			{}
*			{}
*			{}
*			{}
*		} // Inner scope end
*	} // Middle scope end
*	// Outer scope
*
*	Possible output:
*	INFO::File_name:Line_number
*	This is an info message with one param
*	This is an info message with two params: 100
*	This is an info message with three params: 100 yey!!
*	This is an info message with one to four params: 100 Yo!! Yey!!
*
*	Section: Scopes
*
*	Four scopes are defined: Global, Outer, Middle and Inner.
*
*	Global scope - the scope where name lookup is done. Every named entity are prefixed with operator:: for this reason (like ::std::flush).
*
*	Outer scope - the scope before any macro of start message macro set and after any macro of end message macro set:
*		{
*			// Outer scope
*			DEBUG_INFO_MESSAGE
*				// Smth
*				// ...
*			DEBUG_END_MESSAGE
*			// Outer scope
*		}
*		All variables of Outer scope is accessible if Middle and Inner scopes.
*
*	Middle scope - the scope that surrounds inner scope. 
*		expression from DEBUG_END_MESSAGE_AND_EVAL and DEBUG_END_MESSAGE_EVAL_AND_EXIT is evaluated here.
*		::std::exit is also called from this scope.
*		Even if the Inner scope may be skipped as a result of undefined DEBUG macro or higher log level
*		expressions evaluated in this scope is never skipped.
*
*	Inner scope - the scope inside which the output occurs. This scope may be skipped due to undefined DEBUG macro or higher log level.
*		If defined(DEBUG_LIB_THREAD_SAFETY) then all expressions inside this scope are protected by mutex.
*
*	Section: Code generation (comments are not generated)
*
*	DEBUG_END_MESSAGE:
*		if defined(DEBUG):
*
*			DEBUG_OUT << DEBUG_LIB_FLUSH;
*		} // Inner scope end
*	} // Middle scope end
*	// Outer scope
*
*		if !defined(DEBUG):
*
*		} // Inner scope end
*	} // Middle scope end
*	// Outer scope
*
*	DEBUG_END_MESSAGE_AND_EVAL:
*		if defined(DEBUG):
*
*			DEBUG_OUT << DEBUG_LIB_FLUSH;
*		} // Inner scope end
*		expression
*	} // Middle scope end
*	// Outer scope
*
*		if !defined(DEBUG):
*
*		} // Inner scope end
*		expression
*	} // Middle scope end
*	// Outer scope
*
*	DEBUG_END_MESSAGE_AND_EXIT:
*		if defined(DEBUG):
*
*			DEBUG_OUT << DEBUG_LIB_FLUSH;
*		} // Inner scope end
*		::std::exit((exitcode));
*	} // Middle scope end
*	// Outer scope
*
*		if !defined(DEBUG):
*
*		} // Inner scope end
*		::std::exit((exitcode));
*	} // Middle scope end
*	// Outer scope
*
*	DEBUG_END_MESSAGE_EVAL_AND_EXIT:
*		if defined(DEBUG):
*
*			DEBUG_OUT << DEBUG_LIB_FLUSH;
*		} // Inner scope end
*		expression
*		::std::exit((exitcode));
*	} // Middle scope end
*	// Outer scope
*
*		if !defined(DEBUG):
*
*		} // Inner scope end
*		expression
*		::std::exit((exitcode));
*	} // Middle scope end
*	// Outer scope
*
*	Section: Other information
*
*	Standard names used in implementation:
*		::std::mutex
*		::std::ofstream
*		::std::clog
*		::std::lock_guard
*		::std::flush
*		::std::exit
*		::std::fstream::app 
*		::std::fstream::out
*		::atomic<int>::load
*		::atomic<int>::compare_exchange_strong
*		::atomic<int>::compare_exchange_weak
*
*	Features that must be removed to compile with C++98:
*		Comment move constructor and assignment operator for DebugLib::LogLevel
*
*	NESTED MESSAGES ARE NOT ALLOWED!!!
*		Usage of nested messages is undefined behaviour.
*
**/
//Create one debug.hpp file in your project with macro presets for this lib
#include "debug.hpp"

/// STD
#include <cstdlib>

#ifdef DEBUG_LIB_THREAD_SAFETY
/// STD for threads
#	include <mutex>
#	include <atomic>
//	Concurrent access control
#	ifndef DEBUG_LIB_MUTEX_VAR_NAME
#		define DEBUG_LIB_MUTEX_VAR_NAME Debug_Lib_Main_Mutex__
#	endif
namespace DebugLib 
{
	extern ::std::mutex DEBUG_LIB_MUTEX_VAR_NAME;
}
//	Output lock guard for global mutex var name macro def : to avoid name conflict
#	ifndef DEBUG_LIB_LOG_LOCK_GUARG_VAR_NAME
#		define DEBUG_LIB_LOG_LOCK_GUARG_VAR_NAME Debug_Lib_Msg_Scope_Lg__
#	endif
#endif /* DEBUG_LIB_THREAD_SAFETY */

namespace DebugLib
{
	/**
	 *	Defines the possible log levels.
	 */
	enum Level : int 
	{
		All = 0, 	//!< Log any messages
		Info = 0,	//!< Log messages with INFO and higher priority (DEBUG_INFO_MESSAGE, DEBUG_WARNING_MESSAGE, DEBUG_ERROR_MESSAGE, DEBUG_NEW_MESSAGE)
		Warning,	//!< Log messages with WARNING and higher priority (DEBUG_WARNING_MESSAGE, DEBUG_ERROR_MESSAGE, DEBUG_NEW_MESSAGE)
		Error,		//!< Log messages with ERROR and higher priority (DEBUG_ERROR_MESSAGE, DEBUG_NEW_MESSAGE)
		User,		//!< Log only messages that starts with DEBUG_NEW_MESSAGE
		Nothing		//!< Suppress all logging
	};
	
	/**
	 *	@brief Method to obtain read access current global log level.
	 *	If DEBUG_LIB_THREAD_SAFETY is defined then the read operation is atomic.
	 *	@return Current global log level.
	 */
	DebugLib::Level GetGlobalLogLevel();
	
	/**
	 *	@brief Try to change global log level.
	 *	If DEBUG_LIB_THREAD_SAFETY is defined then one call to compare_exchange_strong is made.
	 *	@return true if level was changed, false otherwise.
	 */
	bool try_SetGlobalLogLevel(DebugLib::Level l);

	/**
	 *	@brief Change global log level (may block).
	 *	If DEBUG_LIB_THREAD_SAFETY is defined then compare_exchange_weak is called repeatedly until
	 *	desired log level is set.
	 *	@return true if level was changed, false otherwise.
	 */
	void SetGlobalLogLevel(DebugLib::Level l);
}
#ifndef DEBUG_LIB_DEFAULT_LOG_LEVEL
#	define DEBUG_LIB_DEFAULT_LOG_LEVEL ::DebugLib::Level::All
#endif

#ifdef DEBUG_LIB_FILE_LOG
#	ifndef DEBUG_OUT
#		include <fstream>
//		Output variable name macro def : to avoid name conflict
#		ifndef DEBUG_LIB_LOG_FILE_VAR_NAME
#			define DEBUG_LIB_LOG_FILE_VAR_NAME Debug_Lib_Log_File__
#		endif
//		Std log file path + name
#		ifndef DEBUG_LIB_LOG_FILE_NAME
#			define DEBUG_LIB_LOG_FILE_NAME "log.dat"
#		endif
		namespace DebugLib 
		{
			extern ::std::ofstream DEBUG_LIB_LOG_FILE_VAR_NAME;
		}
#		define DEBUG_OUT ::DebugLib::DEBUG_LIB_LOG_FILE_VAR_NAME
#	endif
#else
#	include <iostream>
/*
//	Output stream for INFO messages
#	ifndef DEBUG_LIB_INFO_OUT
#		define DEBUG_LIB_INFO_OUT ::std::clog
#	endif
//	Output stream for WARNING messages
#	ifndef DEBUG_LIB_WARNING_OUT
#		define DEBUG_LIB_WARNING_OUT ::std::cout
#	endif
//	Output stream for ERROR messages
#	ifndef DEBUG_LIB_ERROR_OUT
#		define DEBUG_LIB_ERROR_OUT ::std::cerr
#	endif
//	Output stream for USER messages
#	ifndef DEBUG_LIB_USER_OUT
#		define DEBUG_LIB_USER_OUT ::std::clog
#	endif
//*/
#	ifndef DEBUG_OUT
#		define DEBUG_OUT ::std::clog
#	endif
#endif /* DEBUG_LIB_FILE_LOG */

// New line definition
#ifndef DEBUG_LIB_NEXT_LINE
#	define DEBUG_LIB_NEXT_LINE '\n'
#endif /* DEBUG_LIB_NEXT_LINE */

// Flush definition
#ifndef DEBUG_LIB_FLUSH
#	define DEBUG_LIB_FLUSH ::std::flush
#endif /* DEBUG_LIB_FLUSH */

// Selector macro set
#define DEBUG_LIB_GET_MACRO_4(_1, _2, _3, _4, NAME, ...) NAME
#define DEBUG_LIB_GET_MACRO_5(_1, _2, _3, _4, _5, NAME, ...) NAME
#ifdef _MSC_VER
// MSVC PP is not same as other PP
#	define DEBUG_LIB_EXPAND( x ) x
#endif

/* Output macro set */

#ifdef DEBUG
//	Debug write macro
//	Allow to output one value to debug stream without new line afterwards.
#	define DEBUG_WRITE1(x) DEBUG_OUT << (x)
//	Allow to output two values to debug stream without new line afterwards.
#	define DEBUG_WRITE2(x, y) DEBUG_OUT << (x) << (y)
//	Allow to output three values to debug stream without new line afterwards.
#	define DEBUG_WRITE3(x, y, z) DEBUG_OUT << (x) << (y) << (z)
//	Allow to output four values to debug stream without new line afterwards.
#	define DEBUG_WRITE4(x, y, z, w) DEBUG_OUT << (x) << (y) << (z) << (w)
//	Allow to output four values to debug stream without new line afterwards.
#	define DEBUG_WRITE5(x, y, z, w, h) DEBUG_OUT << (x) << (y) << (z) << (w) << (h)
//	Auto select between DEBUG_WRITEN macros set
#	ifdef _MSC_VER
#		define DEBUG_WRITE(...) DEBUG_LIB_EXPAND(DEBUG_LIB_GET_MACRO_5(__VA_ARGS__, DEBUG_WRITE5, DEBUG_WRITE4, DEBUG_WRITE3, DEBUG_WRITE2, DEBUG_WRITE1)(__VA_ARGS__))
#	else
#		define DEBUG_WRITE(...) DEBUG_LIB_GET_MACRO_5(__VA_ARGS__, DEBUG_WRITE5, DEBUG_WRITE4, DEBUG_WRITE3, DEBUG_WRITE2, DEBUG_WRITE1)(__VA_ARGS__)
#endif
//	Allow to output one value to debug stream with new line afterwards.
#	define DEBUG_PRINT1(x) DEBUG_OUT << (x) << DEBUG_LIB_NEXT_LINE
//	Allow to output two values to debug stream with new line afterwards
#	define DEBUG_PRINT2(x, y) DEBUG_OUT << (x) << (y) << DEBUG_LIB_NEXT_LINE
//	Allow to output three values to debug stream with new line afterwards.
#	define DEBUG_PRINT3(x, y, z) DEBUG_OUT << (x) << (y) << (z) << DEBUG_LIB_NEXT_LINE
//	Allow to output four values to debug stream with new line afterwards.
#	define DEBUG_PRINT4(x, y, z, w) DEBUG_OUT << (x) << (y) << (z) << (w) << DEBUG_LIB_NEXT_LINE
//	Auto select between DEBUG_PRINTN macros set
#	ifdef _MSC_VER
#		define DEBUG_PRINT(...) DEBUG_LIB_EXPAND(DEBUG_LIB_GET_MACRO_4(__VA_ARGS__, DEBUG_PRINT4, DEBUG_PRINT3, DEBUG_PRINT2, DEBUG_PRINT1)(__VA_ARGS__))
#	else
#		define DEBUG_PRINT(...) DEBUG_LIB_GET_MACRO_4(__VA_ARGS__, DEBUG_PRINT4, DEBUG_PRINT3, DEBUG_PRINT2, DEBUG_PRINT1)(__VA_ARGS__)
#endif
#else
#	define DEBUG_WRITE1(x) {}
#	define DEBUG_WRITE2(x,y) {}
#	define DEBUG_WRITE3(x,y,z) {}
#	define DEBUG_WRITE4(x,y,z,w) {}
#	define DEBUG_WRITE5(x,y,z,w,h) {}
#	define DEBUG_WRITE(...) {}
#	define DEBUG_PRINT1(x) {}
#	define DEBUG_PRINT2(x,y) {}
#	define DEBUG_PRINT3(x,y,z) {}
#	define DEBUG_PRINT4(x,y,z,w) {}
#	define DEBUG_PRINT(...) {}
#endif /* DEBUG */

/* Debug message start macro set */

// Allows to use preprocessor operator# in non function-like macros
#define DEBUG_LIB_AS_C_STRING(val) #val

// Messages that starts with this macro have INFO level of importance
// First line of message will be generated automatically:
// "INFO::File_name:Line_number" 
#ifndef DEBUG_INFO_MESSAGE

#	if defined(DEBUG) && defined(DEBUG_LIB_THREAD_SAFETY)

#		define DEBUG_INFO_MESSAGE \
{ \
	if (::DebugLib::GetGlobalLogLevel() <= ::DebugLib::Level::Info ) \
	{ \
		::std::lock_guard<::std::mutex> DEBUG_LIB_LOG_LOCK_GUARG_VAR_NAME(::DebugLib::DEBUG_LIB_MUTEX_VAR_NAME); \
		DEBUG_PRINT1("INFO::" __FILE__ ":" DEBUG_LIB_AS_C_STRING(__LINE__));

#	elif defined(DEBUG) && !defined(DEBUG_LIB_THREAD_SAFETY)

#		define DEBUG_INFO_MESSAGE \
{ \
	if (::DebugLib::GetGlobalLogLevel() <= ::DebugLib::Level::Info ) \
	{ \
		DEBUG_PRINT1("INFO::" __FILE__ ":" DEBUG_LIB_AS_C_STRING(__LINE__));
		
#	else

#		define DEBUG_INFO_MESSAGE \
{ \
	while(false) \
	{

#	endif

#endif /* DEBUG_INFO_MESSAGE */

// Messages that starts with this macro have WARNING level of importance
// First line of message will be generated automatically:
// "WARNING::File_name:Line_number" 
#ifndef DEBUG_WARNING_MESSAGE

#	if defined(DEBUG) && defined(DEBUG_LIB_THREAD_SAFETY)

#		define DEBUG_WARNING_MESSAGE \
{ \
	if (::DebugLib::GetGlobalLogLevel() <= ::DebugLib::Level::Warning ) \
	{ \
		::std::lock_guard<::std::mutex> DEBUG_LIB_LOG_LOCK_GUARG_VAR_NAME(::DebugLib::DEBUG_LIB_MUTEX_VAR_NAME); \
		DEBUG_PRINT1("WARNING::" __FILE__ ":" DEBUG_LIB_AS_C_STRING(__LINE__));

#	elif defined(DEBUG) && !defined(DEBUG_LIB_THREAD_SAFETY)

#		define DEBUG_WARNING_MESSAGE \
{ \
	if (::DebugLib::GetGlobalLogLevel() <= ::DebugLib::Level::Warning ) \
	{ \
		DEBUG_PRINT1("WARNING::" __FILE__ ":" DEBUG_LIB_AS_C_STRING(__LINE__));

#	else

#		define DEBUG_WARNING_MESSAGE \
{ \
	while(false) \
	{

#	endif

#endif /* DEBUG_WARNING_MESSAGE */

// Messages that starts with this macro have ERROR level of importance
// First line of message will be generated automatically:
// "ERROR::File_name:Line_number" 
#ifndef DEBUG_ERROR_MESSAGE

#	if defined(DEBUG) && defined(DEBUG_LIB_THREAD_SAFETY)

#		define DEBUG_ERROR_MESSAGE \
{ \
	if (::DebugLib::GetGlobalLogLevel() <= ::DebugLib::Level::Error ) \
	{ \
		::std::lock_guard<::std::mutex> DEBUG_LIB_LOG_LOCK_GUARG_VAR_NAME(::DebugLib::DEBUG_LIB_MUTEX_VAR_NAME); \
		DEBUG_PRINT1("ERROR::" __FILE__ ":" DEBUG_LIB_AS_C_STRING(__LINE__));

#	elif defined(DEBUG) && !defined(DEBUG_LIB_THREAD_SAFETY)

#		define DEBUG_ERROR_MESSAGE \
{ \
	if (::DebugLib::GetGlobalLogLevel() <= ::DebugLib::Level::Error ) \
	{ \
		DEBUG_PRINT1("ERROR::" __FILE__ ":" DEBUG_LIB_AS_C_STRING(__LINE__));

#	else

#		define DEBUG_ERROR_MESSAGE \
{ \
	while(false) \
	{

#	endif

#endif /* DEBUG_ERROR_MESSAGE */

// Messages that starts with this macro have USER level of importance
// First line of message must be provided by user
// All provided arguments are forwarded to DEBUG_PRINT(...)
#ifndef DEBUG_NEW_MESSAGE

#	if defined(DEBUG) && defined(DEBUG_LIB_THREAD_SAFETY)

#		define DEBUG_NEW_MESSAGE(...) \
{ \
	if (::DebugLib::GetGlobalLogLevel() <= ::DebugLib::Level::User ) \
	{ \
		::std::lock_guard<::std::mutex> DEBUG_LIB_LOG_LOCK_GUARG_VAR_NAME(::DebugLib::DEBUG_LIB_MUTEX_VAR_NAME); \
		DEBUG_PRINT(__VA_ARGS__);

#	elif defined(DEBUG) && !defined(DEBUG_LIB_THREAD_SAFETY)

#		define DEBUG_NEW_MESSAGE(...) \
{ \
	if (::DebugLib::GetGlobalLogLevel() <= ::DebugLib::Level::User ) \
	{ \
		DEBUG_PRINT(__VA_ARGS__);	
		
#	else

#		define DEBUG_NEW_MESSAGE(...) \
{ \
	while(false) \
	{

#	endif

#endif /* DEBUG_NEW_MESSAGE */

/* Debug message end macro set */

// End message and flush
#ifndef DEBUG_END_MESSAGE
#	ifdef DEBUG
#		define DEBUG_END_MESSAGE DEBUG_OUT << DEBUG_LIB_FLUSH; } }
#	else
#		define DEBUG_END_MESSAGE } }
#	endif
#endif /* DEBUG_END_MESSAGE */

// End message, flush and evaluate an expression
#ifndef DEBUG_END_MESSAGE_AND_EVAL
#	ifdef DEBUG
#		define DEBUG_END_MESSAGE_AND_EVAL(expression) \
			DEBUG_OUT << DEBUG_LIB_FLUSH; \
		}\
	expression \
 }
#	else
#		define DEBUG_END_MESSAGE_AND_EVAL(expression) \
		}\
	expression \
 }
#	endif
#endif /* DEBUG_END_MESSAGE_AND_EVAL */

// End message, flush and exit program with 'exitcode'
#ifndef DEBUG_END_MESSAGE_AND_EXIT
#	ifdef DEBUG
#		define DEBUG_END_MESSAGE_AND_EXIT(exitcode) \
		DEBUG_OUT << DEBUG_LIB_FLUSH; } ::std::exit((exitcode)); }
#	else
#		define DEBUG_END_MESSAGE_AND_EXIT(exitcode) \
		} ::std::exit((exitcode)); }
#	endif
#endif /* DEBUG_END_MESSAGE_AND_EXIT */

// End message, flush, evaluate an expression and exit program with 'exitcode'
#ifndef DEBUG_END_MESSAGE_EVAL_AND_EXIT
#	ifdef DEBUG
#		define DEBUG_END_MESSAGE_EVAL_AND_EXIT(exitcode, expression) \
			DEBUG_OUT << DEBUG_LIB_FLUSH; \
		} \
	expression \
	::std::exit((exitcode));\
 }
#	else
#		define DEBUG_END_MESSAGE_EVAL_AND_EXIT(exitcode, expression) \
		} \
	expression \
	::std::exit((exitcode));\
 }
#	endif
#endif /* DEBUG_END_MESSAGE_EVAL_AND_EXIT */

/* Tests */

#ifdef DEBUG_LIB_TEST
#if defined(_WIN32) && defined(_MSC_VER)
	//*
	// For test purposes: float/double to int conversion
	#pragma warning( disable : 4244 )
	// For test purposes: std::size_t to int conversion
	#pragma warning( disable : 4267 )
	// For test purposes: DEBUG_OUT macro multiple redefinition
	#pragma warning( disable : 4005 )
	// For test purposes: std::size_t to int conversion for fprintf
	#pragma warning( disable : 4477 )
	// We use std::fopen instead of std::fopen_s
	#pragma warning( disable : 4996 )
	//*/
#endif
#	include <cstdio>
	namespace DebugLib 
	{
		struct DebugLibLoggerSingleton
		{
			static std::FILE* getInstance(const char* path)
			{
				static DebugLibLoggerSingleton s(path);
				return s.h;
			}

		private:
			std::FILE* h;
		
			DebugLibLoggerSingleton(const char* path) : h(std::fopen(path, "w+")) {}

			~DebugLibLoggerSingleton()
			{
				std::fflush(h);
				std::fclose(h);
			}
		};

#	ifdef DEBUG_LIB_THREAD_SAFETY
		extern ::std::mutex Debug_Lib_Logger_Singletone_Mutex__;
#	endif

	} // namespace DebugLib

#	ifndef	LOG
#		ifdef DEBUG_LIB_THREAD_SAFETY
#			define LOG(format, ...) \
do { \
	::std::lock_guard<::std::mutex> Debug_Lib_Logger_LG__(::DebugLib::Debug_Lib_Logger_Singletone_Mutex__); \
	::std::fprintf(::DebugLib::DebugLibLoggerSingleton::getInstance(".\\DebugLibTestLogMT.txt"), "[%-35s: %-25s: line:%-4d] " format "\n", __FILE__, __FUNCTION__, __LINE__, ## __VA_ARGS__); \
} while(0);
#		else
#			define LOG(format, ...) \
do { \
	::std::fprintf(::DebugLib::DebugLibLoggerSingleton::getInstance(".\\DebugLibTestLogST.txt"), "[%-35s: %-25s: line:%-4d] " format "\n", __FILE__, __FUNCTION__, __LINE__, ## __VA_ARGS__); \
} while(0);
#		endif
#	endif 
#endif /* DEBUG_LIB_TEST */

#endif /* DEBUG_LIB_HPP__ */