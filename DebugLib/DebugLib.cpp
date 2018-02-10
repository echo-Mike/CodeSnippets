#include "mDebuglib.hpp"

namespace DebugLib
{

#ifdef DEBUG_LIB_FILE_LOG
    std::ofstream DEBUG_LIB_LOG_FILE_VAR_NAME(DEBUG_LIB_LOG_FILE_NAME, std::fstream::app | std::fstream::out);
#endif

#ifdef DEBUG_LIB_THREAD_SAFETY
    std::mutex DEBUG_LIB_MUTEX_VAR_NAME;
#endif

    class LogLevel
    {
	public:

		LogLevel(::DebugLib::Level level) : logLevel(level) {}

		LogLevel(const LogLevel&) = delete;
		LogLevel& operator=(const LogLevel&) = delete;
		LogLevel(LogLevel&&) = delete;
		LogLevel& operator=(LogLevel&&) = delete;

    protected:
		friend Level DebugLib::GetGlobalLogLevel();
		friend bool DebugLib::try_SetGlobalLogLevel(Level l);
		friend bool DebugLib::SetGlobalLogLevel(Level l);
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

		bool setLogLevel(Level l)
		{
			int dummy = logLevel.load();
			while (!logLevel.compare_exchange_weak(dummy, static_cast<int>(l)));
			return true;
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

		bool setLogLevel(Level l)
		{
			logLevel = static_cast<int>(l);
			return true;
		}
	private:
		int logLevel;	
#endif
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

bool DebugLib::SetGlobalLogLevel(DebugLib::Level l)
{
	return Debug_Lib_Log_State__.setLogLevel(l);
}