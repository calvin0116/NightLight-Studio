#ifndef SPEEDLOG_WRAPPER
#define SPEEDLOG_WRAPPER

#include "../../framework.h"
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/rotating_file_sink.h>

#define DEFAULT_LOG_NAME "log"
#define DEFAULT_LOG_PATH "logs/log.txt"
#define FLUSH_EVERY_SECONDS 3

class SpeedLog
{
	std::shared_ptr<spdlog::logger> _log;
public:

	SpeedLog();
	~SpeedLog();

	SpeedLog(const SpeedLog& other);
	SpeedLog(const std::shared_ptr<spdlog::logger>& other);
	SpeedLog& operator=(const SpeedLog& other);
	SpeedLog& operator=(const std::shared_ptr<spdlog::logger>& other);

	bool operator == (const SpeedLog& other);
	bool operator == (const std::shared_ptr<spdlog::logger>& other);

	bool operator != (const SpeedLog& other);
	bool operator != (const std::shared_ptr<spdlog::logger>& other);

	template<class FormatStrings, class... Args>
	void operator ()(const FormatStrings& fmt, Args&& ... args);

	std::shared_ptr<spdlog::logger> operator->();

	std::shared_ptr<spdlog::logger> GetLogger();
	std::shared_ptr<spdlog::logger> ResetLogger();
	std::shared_ptr<spdlog::logger> SetNewBasicLogger(const std::string& name, const std::string& path);
	std::shared_ptr<spdlog::logger> SetNewRotatingLogger(const std::string& name, const std::string& path, size_t maxSize, size_t maxFiles);
};

extern SpeedLog SPEEDLOG;

template<class FormatStrings, class... Args>
inline void SPEEDLOG_INFO(const FormatStrings& fmt, Args&& ... args)
{
	SPEEDLOG->info(fmt, args...);
}

template<class FormatStrings, class... Args>
inline void SPEEDLOG_WARN(const FormatStrings& fmt, Args&& ... args)
{
	SPEEDLOG->warn(fmt, args...);
}

template<class FormatStrings, class... Args>
inline void SPEEDLOG_ERROR(const FormatStrings& fmt, Args&& ... args)
{
	SPEEDLOG->error(fmt, args...);
}

template<class FormatStrings, class... Args>
inline void SPEEDLOG_DEBUG(const FormatStrings& fmt, Args&& ... args)
{
	SPEEDLOG->debug(fmt, args...);
}

template<class FormatStrings, class... Args>
inline void SPEEDLOG_CRITICAL(const FormatStrings& fmt, Args&& ... args)
{
	SPEEDLOG->critical(fmt, args...);
}

template<class FormatStrings, class... Args>
inline void SPEEDLOG_TRACE(const FormatStrings& fmt, Args&& ... args)
{
	SPEEDLOG->trace(fmt, args...);
}

inline void SPEEDLOG_SETLEVEL(spdlog::level::level_enum lvl)
{
	SPEEDLOG->set_level(lvl);
}

inline void SPEEDLOG_ENABLE_BACKTRACE(int num)
{
	SPEEDLOG->enable_backtrace(num);
}

inline void SPEEDLOG_DISABLE_BACKTRACE()
{
	SPEEDLOG->disable_backtrace();
}

inline void SPEEDLOG_DUMP_BACKTRACE()
{
	SPEEDLOG->dump_backtrace();
}

inline void SPEEDLOG_FLUSH()
{
	SPEEDLOG->flush();
}

inline void SPEEDLOG_SET_NEW_BASIC(const std::string& name, const std::string path)
{
	SPEEDLOG.SetNewBasicLogger(name, path);
}

inline void SPEEDLOG_SET_NEW_ROTATING(const std::string& name, const std::string& path, size_t maxSize, size_t maxFiles)
{
	SPEEDLOG.SetNewRotatingLogger(name, path, maxSize, maxFiles);
}

inline void SPEEDLOG_ACTIVATE_FLUSH()
{
	SPEEDLOG->flush();
}


template<class FormatStrings, class ...Args>
inline void SpeedLog::operator()(const FormatStrings& fmt, Args && ...args)
{
	(*this)->info(fmt, args...);
}


#endif
