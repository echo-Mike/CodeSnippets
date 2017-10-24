#include "mDebuglib.hpp"

#ifdef DEBUG_LIB_FILE_LOG
    std::ofstream DebugLib::DEBUG_LIB_LOG_FILE_VAR_NAME(DEBUG_LIB_LOG_FILE_NAME, std::fstream::app | std::fstream::out);
#endif

#ifdef DEBUG_LIB_LOG_THREAD_SAFETY
    std::mutex DebugLib::DEBUG_LIB_LOG_MUTEX_VAR_NAME;
#endif