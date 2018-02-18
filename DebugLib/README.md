# DebugLib

Defines and implements logging interface intended to be used in debug proposes.  
## Table of contents
<!-- TOC -->

- [DebugLib](#debuglib)
	- [Table of contents](#table-of-contents)
	- [Logging abstract machine](#logging-abstract-machine)
	- [Compiling](#compiling)
	- [Usage guide](#usage-guide)
	- [Example](#example)
		- [In code](#in-code)
		- [Code generated](#code-generated)
		- [Possible output](#possible-output)
	- [Scopes](#scopes)
		- [Global](#global)
		- [Outer](#outer)
		- [Middle](#middle)
		- [Inner](#inner)
	- [Code generation](#code-generation)
		- [`DEBUG_END_MESSAGE`](#debug_end_message)
		- [`DEBUG_END_MESSAGE_AND_EVAL`](#debug_end_message_and_eval)
		- [`DEBUG_END_MESSAGE_AND_EXIT`](#debug_end_message_and_exit)
		- [`DEBUG_END_MESSAGE_EVAL_AND_EXIT`](#debug_end_message_eval_and_exit)
	- [Other information](#other-information)
		- [Standard names used in the implementation](#standard-names-used-in-the-implementation)
		- [Features that must be removed to compile with C++98](#features-that-must-be-removed-to-compile-with-c98)
		- [**NESTED MESSAGES ARE NOT ALLOWED!!!**](#nested-messages-are-not-allowed)
	- [Macros that can be redefined](#macros-that-can-be-redefined)
		- [`DEBUG_LIB_MUTEX_VAR_NAME`](#debug_lib_mutex_var_name)
		- [`DEBUG_LIB_LOG_LOCK_GUARG_VAR_NAME`](#debug_lib_log_lock_guarg_var_name)
		- [`DEBUG_LIB_DEFAULT_LOG_LEVEL`](#debug_lib_default_log_level)
		- [`DEBUG_LIB_LOG_FILE_VAR_NAME`](#debug_lib_log_file_var_name)
		- [`DEBUG_LIB_LOG_FILE_NAME`](#debug_lib_log_file_name)
		- [`DEBUG_OUT`](#debug_out)
		- [`DEBUG_LIB_FLUSH`](#debug_lib_flush)
		- [`DEBUG_LIB_NEXT_LINE`](#debug_lib_next_line)
		- [Start message macros set](#start-message-macros-set)
		- [End message macros set](#end-message-macros-set)
	- [Library behaviour macros](#library-behaviour-macros)
		- [`DEBUG_LIB_THREAD_SAFETY`](#debug_lib_thread_safety)
		- [`DEBUG_LIB_FILE_LOG`](#debug_lib_file_log)
		- [`DEBUG`](#debug)

<!-- /TOC -->
## Logging abstract machine
Currently consists of next parts:
* Output stream
* Debug level
* Message  

The **Output stream** is a stream-like output facility to which all formated output will be redirected.  
The **Debug level** is a program-wide indicator based on value of which the decision is made in runtime: 
* If current **Debug level** is less or equal to debug level of the **Message** than it is transmitted to **Output stream**;
* Otherwise it is suppressed.  

The **Message** is a set of output operations that are seen together as a single output transaction. In order to obtain the desired behaviour it must be explicitly started and ended. Every **Message** have the associated quantity - the debug level depending on which it may be suppressed on runtime.  

## Compiling
This library must be compiled.  
Currently two cases are supported: 
* compile with your project;
* compile as static library.  

For the first case simply include *mDebugLib.hpp* file to you build script and compile paired *DebugLib.cpp* file as part of your project.  
For the second case use provided build scripts and include builded library and header to build scripts of your project.  
Generally if thread safety is not needed the library is compatible with C++98 (See: [Other information](#other-information)).
If thread safety is needed your compiler must implement C++11 thread and atomic std libraries.  

## Usage guide

Use one macro from **start message macros set** to start new debug message:  
* `DEBUG_INFO_MESSAGE` - starts message with info debug level; 
* `DEBUG_WARNING_MESSAGE` - starts message with warning debug level; 
* `DEBUG_ERROR_MESSAGE` - starts message with error debug level; 
* `DEBUG_NEW_MESSAGE(...)` - starts message with user debug level (highest).

Then use any count of macros from **output macros set** to output your message:  
* `DEBUG_PRINT[1-4](1-4 args), DEBUG_PRINT(... (1-4 args))` - to print a line (`DEBUG_LIB_NEXT_LINE` will be put in the end of every macro call);  
* `DEBUG_WRITE[1-5](1-5 args), DEBUG_WRITE(... (1-5 args))` - to write a line (`DEBUG_LIB_NEXT_LINE` will not be put in the end of any macro call).  

All output is redirected to `DEBUG_OUT` macro using `operator<<`.  

To finish your message use one macro from **end message macros set**:  
* `DEBUG_END_MESSAGE` - finish your message and send `DEBUG_LIB_FLUSH` to output stream;
* `DEBUG_END_MESSAGE_AND_EVAL(expression)` - finish your message, send `DEBUG_LIB_FLUSH` to output stream and evaluates provided `expression` in middle scope (See: [Scopes](#scopes)). To evaluate a complex expression surround it with `{}`. If it is a one-line expression it must be ended with `;` (See: [Code generation](#code-generation)). `expression` is evaluated even when `DEBUG` macro is undefined. If an `expression` throws no actions to catch the exception will be taken;
* `DEBUG_END_MESSAGE_AND_EXIT(exitcode)` - finish your message, send `DEBUG_LIB_FLUSH` to output stream and call `::std::exit((exitcode))` expression in middle scope;
* `DEBUG_END_MESSAGE_EVAL_AND_EXIT(exitcode, expression)` - combination of `DEBUG_END_MESSAGE_AND_EVAL` and `DEBUG_END_MESSAGE_AND_EXIT`. `::std::exit((exitcode))` is called right after evaluation an `expression`.  

## Example
Information message
### In code:
```C++
DEBUG_INFO_MESSAGE
	DEBUG_PRINT1("This is an info message with one param");
	DEBUG_PRINT2("This is an info message with two params: ", 100);
	DEBUG_PRINT3("This is an info message with three params: ", 100, " yey!!");
	DEBUG_PRINT( "This is an info message with one to four params: ", 100, " Yo!!", "Yey!!");
DEBUG_END_MESSAGE
```
### Code generated:
Comments are not generated.  
If `defined(DEBUG) && defined(DEBUG_LIB_THREAD_SAFETY)`:
```C++
// Outer scope
{	// Middle scope begin
	if (::DebugLib::GetGlobalLogLevel() <= ::DebugLib::Level::Info )
	{	// Inner scope begin
		::std::lock_guard<::std::mutex> DEBUG_LIB_LOG_LOCK_GUARG_VAR_NAME(::DebugLib::DEBUG_LIB_MUTEX_VAR_NAME);
		DEBUG_OUT << ("INFO::" __FILE__ ":" #__LINE__) << DEBUG_LIB_NEXT_LINE; // "INFO::" __FILE__ ":" #__LINE__ is merged in one c-string by compiler
		DEBUG_OUT << ("This is an info message with one param") << DEBUG_LIB_NEXT_LINE;
		DEBUG_OUT << ("This is an info message with two params: ") << (100) << DEBUG_LIB_NEXT_LINE;
		DEBUG_OUT << ("This is an info message with three params: ") << (100) << (" yey!!") << DEBUG_LIB_NEXT_LINE;
		DEBUG_OUT << ("This is an info message with one to four params: ") << (100) << (" Yo!!") << ("Yey!!") << DEBUG_LIB_NEXT_LINE;
		DEBUG_OUT << DEBUG_LIB_FLUSH;
	} // Inner scope end
} // Middle scope end
// Outer scope
```
If `defined(DEBUG) && !defined(DEBUG_LIB_THREAD_SAFETY)`:
```C++
// Outer scope
{	// Middle scope begin
	if (::DebugLib::GetGlobalLogLevel() <= ::DebugLib::Level::Info )
	{	// Inner scope begin
		DEBUG_OUT << ("INFO::" __FILE__ ":" #__LINE__) << DEBUG_LIB_NEXT_LINE; // "INFO::" __FILE__ ":" #__LINE__ is merged in one c-string by compiler
		DEBUG_OUT << ("This is an info message with one param") << DEBUG_LIB_NEXT_LINE;
		DEBUG_OUT << ("This is an info message with two params: ") << (100) << DEBUG_LIB_NEXT_LINE;
		DEBUG_OUT << ("This is an info message with three params: ") << (100) << (" yey!!") << DEBUG_LIB_NEXT_LINE;
		DEBUG_OUT << ("This is an info message with one to four params: ") << (100) << (" Yo!!") << ("Yey!!") << DEBUG_LIB_NEXT_LINE;
		DEBUG_OUT << DEBUG_LIB_FLUSH;
	} // Inner scope end
} // Middle scope end
// Outer scope
```
If `!defined(DEBUG)`:
```C++
// Outer scope
{	// Middle scope begin
	while (false)
	{	// Inner scope begin
		{}
		{}
		{}
		{}
	} // Inner scope end
} // Middle scope end
// Outer scope
```
### Possible output:
>INFO::File_name:Line_number  
>This is an info message with one param  
>This is an info message with two params: 100  
>This is an info message with three params: 100 yey!!  
>This is an info message with one to four params: 100 Yo!! Yey!!

## Scopes
Four scopes are defined: Global, Outer, Middle and Inner.  
### Global
Scope where name lookup is done. Every named entity are prefixed with `operator::` for this reason (like `::std::flush`).  
### Outer
Scope before any macro of start message macro set and after any macro of end message macro set:
```C++
{
	// Outer scope
	DEBUG_INFO_MESSAGE
		// Smth
		// ...
	DEBUG_END_MESSAGE
	// Outer scope
}
```
All variables of **Outer** scope is accessible if **Middle** and **Inner** scopes.  
### Middle
Scope that surrounds **Inner** scope. `expression` from `DEBUG_END_MESSAGE_AND_EVAL` and `DEBUG_END_MESSAGE_EVAL_AND_EXIT` is evaluated here. `::std::exit` is also called from this scope.  
Even if the **Inner** scope may be skipped as a result of undefined `DEBUG` macro or higher log level expressions evaluated in this scope is never skipped.  
### Inner
Scope inside which the output occurs. This scope may be skipped due to undefined `DEBUG` macro or higher log level.  
If `defined(DEBUG_LIB_THREAD_SAFETY)` then this scope is a critical section and it must be protected.  

## Code generation
 Comments are not generated.

### `DEBUG_END_MESSAGE`
if `defined(DEBUG)`:
```C++
		DEBUG_OUT << DEBUG_LIB_FLUSH;
	} // Inner scope end
} // Middle scope end
// Outer scope
```
if `!defined(DEBUG)`:
```C++
	} // Inner scope end
} // Middle scope end
// Outer scope
```
### `DEBUG_END_MESSAGE_AND_EVAL`
if `defined(DEBUG)`:
```C++
		DEBUG_OUT << DEBUG_LIB_FLUSH;
	} // Inner scope end
	expression
} // Middle scope end
// Outer scope
```
if `!defined(DEBUG)`:
```C++
	} // Inner scope end
	expression
} // Middle scope end
// Outer scope
```
### `DEBUG_END_MESSAGE_AND_EXIT`
if `defined(DEBUG)`:
```C++
		DEBUG_OUT << DEBUG_LIB_FLUSH;
	} // Inner scope end
	::std::exit((exitcode));
} // Middle scope end
// Outer scope
```
if `!defined(DEBUG)`:
```C++
	} // Inner scope end
	::std::exit((exitcode));
} // Middle scope end
// Outer scope
```
### `DEBUG_END_MESSAGE_EVAL_AND_EXIT`
if `defined(DEBUG)`:
```C++
		DEBUG_OUT << DEBUG_LIB_FLUSH;
	} // Inner scope end
	expression
	::std::exit((exitcode));
} // Middle scope end
// Outer scope
```
if `!defined(DEBUG)`:
```C++
	} // Inner scope end
	expression
	::std::exit((exitcode));
} // Middle scope end
// Outer scope
```
## Other information
### Standard names used in the implementation
```C++
	::std::mutex
	::std::ofstream
	::std::clog
	::std::lock_guard
	::std::flush
	::std::exit
	::std::fstream::app 
	::std::fstream::out
	::atomic<int>::load
	::atomic<int>::compare_exchange_strong
	::atomic<int>::compare_exchange_weak
```
### Features that must be removed to compile with C++98
Comment move constructor and assignment operator for `DebugLib::LogLevel`

### **NESTED MESSAGES ARE NOT ALLOWED!!!**
Current implementation does not allow nested messages. Usage of nested messages is undefined behaviour.

## Macros that can be redefined
Listed in order of appearance in library header.
### `DEBUG_LIB_MUTEX_VAR_NAME`
**Description**: defines the name to be used for the global output mutex.  
This name is encapsulated in DebugLib namespace. You can refer to this variable as: `::DebugLib::DEBUG_LIB_MUTEX_VAR_NAME`  
**Default value**: `Debug_Lib_Main_Mutex__`  
**Status**: Implementation dependent

### `DEBUG_LIB_LOG_LOCK_GUARG_VAR_NAME`
**Description**: defines the name of `::std::lock_guard` for **Inner** scope.  This name is only accessible from **Inner** scope (See: [Scopes](#scopes)) as: `DEBUG_LIB_LOG_LOCK_GUARG_VAR_NAME`  
**Default value**: `Debug_Lib_Msg_Scope_Lg__`
**Status**: Implementation dependent

### `DEBUG_LIB_DEFAULT_LOG_LEVEL`
**Description**: defines the log level with which the application starts.  
If redefined must be one of `DebugLib::Level` enum members.  
**Default value**: `::DebugLib::Level::All`  
**Status**: Implementation independent

### `DEBUG_LIB_LOG_FILE_VAR_NAME`
**Description**: defines the name to be used for the output file stream if `defined(DEBUG_LIB_FILE_LOG)`.   
This name is encapsulated in `DebugLib` namespace. You can refer to this variable as: `::DebugLib::DEBUG_LIB_LOG_FILE_VAR_NAME`  
Only accessible if `defined(DEBUG_LIB_FILE_LOG)`, otherwise have no effect.  
**Default value**: `Debug_Lib_Log_File__`  
**Status**: Implementation dependent

### `DEBUG_LIB_LOG_FILE_NAME`
**Description**: defines the file name of general log file.  
If redefined must be a C-string with valid path to file. On start of application this file will be opened with `std::fstream::app | std::fstream::out` flags.  
It is a user's responsibility to check state of this file stream.  
Only accessible if `defined(DEBUG_LIB_FILE_LOG)`, otherwise have no effect.  
**Default value**: `"log.dat"`  
**Status**: Implementation independent

### `DEBUG_OUT`
**Description**: defines the object to which all output will be redirected.  
Must have `operator<<` that accepts at least C-strings, char, any numbers(integers or floats), `DEBUG_LIB_FLUSH` (must have same effect as `::std::flush`) and returns reference to stream object.  
**Default value**:
```C++
#if defined(DEBUG_LIB_FILE_LOG)
#   define DEBUG_OUT ::DebugLib::DEBUG_LIB_LOG_FILE_VAR_NAME
#else
#   define DEBUG_OUT ::std::clog
#endif
```
**Status**: Implementation independent

### `DEBUG_LIB_FLUSH`
**Description**: defines the flush operation.  
`DEBUG_OUT` must process this input as if it was a standard stream and `::std::flush` was given as input.  
**Default value**: `::std::flush`  
**Status**: Implementation independent

### `DEBUG_LIB_NEXT_LINE`
**Description**: defines the next line operation.  
**Default value**: `'\n'`  
**Status**: Implementation independent

### Start message macros set
Includes:
* `DEBUG_INFO_MESSAGE`
* `DEBUG_WARNING_MESSAGE`
* `DEBUG_ERROR_MESSAGE`
* `DEBUG_NEW_MESSAGE(...)`  
 
**Description**: defines a new debug message start.  
Any macro of start message macro set may be redefined.  
This macros may be used directly after any of next statements:  
* for( *for_expr* )
* while( *while_expr* )
* do (must be ended with while( *while_expr* ))
* if( *if_expr* )
* else
* try
* catch( *catch_expr* )
* function_definition  

as if they defines the statement scope.  
**Default value**: See mDebugLib.hpp code   
**Status**: Implementation independent

### End message macros set
Includes:
* `DEBUG_END_MESSAGE`
* `DEBUG_END_MESSAGE_AND_EVAL(expression)`
* `DEBUG_END_MESSAGE_AND_EXIT(exitcode)`
* `DEBUG_END_MESSAGE_EVAL_AND_EXIT(exitcode, expression)`  

**Description**: defines the end of debug message.  
Any macro of end message macro set may be redefined.  
**Default value**: See mDebugLib.hpp code   
**Status**: Implementation independent

## Library behaviour macros

### `DEBUG_LIB_THREAD_SAFETY`
**Description**: if defined output facility must be thread safe.  
Includes:  
  1. Global log level must be an atomic variable;  
  2. The Inner scope must be a critical section protected by mutual exclusion or other synchronization or concurrent execution safety measures.  

For default implementation the `<mutex>` and `<atomic>` headers must be available.  
**Status**: Implementation independent

### `DEBUG_LIB_FILE_LOG`
**Description**: if defined the implementation must create a file output to be used as `DEBUG_OUT`.  
For default implementation the `<fstream>` header must be available if `defined(DEBUG_LIB_FILE_LOG)` and `<iostream>` if `!defined(DEBUG_LIB_FILE_LOG)`.  
**Status**: Implementation independent

### `DEBUG`
**Description**: if defined **Inner** scope content must be generated, otherwise it may be not generated.  
**Status**: Implementation independent