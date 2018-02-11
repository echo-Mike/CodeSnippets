#include "mDebuglib.hpp"

namespace DebugLib
{

#ifdef DEBUG_LIB_FILE_LOG
    ::std::ofstream DEBUG_LIB_LOG_FILE_VAR_NAME(DEBUG_LIB_LOG_FILE_NAME, ::std::fstream::app | ::std::fstream::out);
#endif

#ifdef DEBUG_LIB_THREAD_SAFETY
    ::std::mutex DEBUG_LIB_MUTEX_VAR_NAME;
#endif

    class LogLevel
    {
	public:

		LogLevel(::DebugLib::Level level) : logLevel(level) {}
		
    protected:
		friend Level DebugLib::GetGlobalLogLevel();
		friend bool DebugLib::try_SetGlobalLogLevel(Level l);
		friend void DebugLib::SetGlobalLogLevel(Level l);
#ifdef DEBUG_LIB_THREAD_SAFETY
		Level getLogLevel() 
		{
			return static_cast<Level>(logLevel.load());
		}

		bool try_SetLogLevel(Level l) 
		{
			int dummy = logLevel.load();
			return logLevel.compare_exchange_strong(dummy, static_cast<int>(l));
		}

		void setLogLevel(Level l)
		{
			int dummy = logLevel.load();
			while (!logLevel.compare_exchange_weak(dummy, static_cast<int>(l)));
		}
	private:
		std::atomic<int>  logLevel;	
#else
		Level getLogLevel()
		{
			return static_cast<Level>(logLevel);
		}

		bool try_SetLogLevel(Level l) 
		{
			logLevel = static_cast<int>(l);
			return true;
		}

		void setLogLevel(Level l)
		{
			logLevel = static_cast<int>(l);
		}
	private:
		int logLevel;	
#endif
		LogLevel(const LogLevel&) {}
		LogLevel& operator=(const LogLevel&) { return *this; }
		//* (//* -> /* (remove first slash) if C++98 is used)
		LogLevel(LogLevel&&) {}
		LogLevel& operator=(LogLevel&&) { return *this; }
		//*/
	};
}

static DebugLib::LogLevel Debug_Lib_Log_State__(DEBUG_LIB_DEFAULT_LOG_LEVEL);

DebugLib::Level DebugLib::GetGlobalLogLevel()
{
    return Debug_Lib_Log_State__.getLogLevel();
}

bool DebugLib::try_SetGlobalLogLevel(DebugLib::Level l)
{
    return Debug_Lib_Log_State__.try_SetLogLevel(l);
}

void DebugLib::SetGlobalLogLevel(DebugLib::Level l)
{
	Debug_Lib_Log_State__.setLogLevel(l);
}