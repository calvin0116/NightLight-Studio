#ifndef SPEEDLOG_WRAPPER
#define SPEEDLOG_WRAPPER

#define SPEEDLOG_ALLOW true

#pragma warning( disable : 4244 )  // utility: conversion warning

#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/daily_file_sink.h>

#define SPEEDLOG_LOG_TO_FILE true

class SpeedLog
{
#if SPEEDLOG_ALLOW
	static bool _first;
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
	std::shared_ptr<spdlog::logger> SetNewBasicLogger(const std::string& name, const std::string& path, bool truncate = false);
	std::shared_ptr<spdlog::logger> SetNewRotatingLogger(const std::string& name, const std::string& path, size_t maxSize, size_t maxFiles, bool rotate_on_open = false);
	std::shared_ptr<spdlog::logger> SetNewDailyLogger(const std::string& name, const std::string& path, int hour = 0, int minute = 0, bool truncate = false);
#else
public:
	SpeedLog() {}
	~SpeedLog() {}

	template<class... Args>
	SpeedLog(Args&&...) {}
	template<class... Args>
	SpeedLog& operator=(Args&&...) {}

	template<class... Args>
	bool operator == (Args&&...) {}

	template<class... Args>
	bool operator != (Args&&...) {}

	std::shared_ptr<spdlog::logger> operator->() { return nullptr; }

	std::shared_ptr<spdlog::logger> GetLogger() { return nullptr; }
	std::shared_ptr<spdlog::logger> ResetLogger() { return nullptr; }

	template<class... Args>
	std::shared_ptr<spdlog::logger> SetNewBasicLogger(Args&& ... args) { return nullptr; }

	template<class... Args>
	std::shared_ptr<spdlog::logger> SetNewRotatingLogger(Args&& ... args) { return nullptr; }

	template<class... Args>
	std::shared_ptr<spdlog::logger> SetNewDailyLogger(Args&& ... args) { return nullptr; }

	template<class... Args>
	void operator ()(Args&& ...) {}
#endif
};

extern SpeedLog SPEEDLOG;

#if SPEEDLOG_ALLOW

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

/*
// DO NOT USE IF YOU ARE NOT GOING TO SHUTDOWN
inline void SPEEDLOG_FLUSH_EVERY(const unsigned secs)
{
	spdlog::flush_every(std::chrono::seconds(secs));
}
*/

// DO NOT USE UNLESS FINISHING
/*
inline void SPEEDLOG_SHUTDOWN_ALL()
{
	spdlog::shutdown();
}
*/

template<class FormatStrings, class ...Args>
inline void SpeedLog::operator()(const FormatStrings& fmt, Args && ...args)
{
	(*this)->info(fmt, args...);
}


#else

template<class... Args>
inline void SPEEDLOG_INFO(Args&& ... ) {}

template<class... Args>
inline void SPEEDLOG_WARN(Args&& ... ) { }

template<class... Args>
inline void SPEEDLOG_ERROR(Args&& ... ) { }

template<class... Args>
inline void SPEEDLOG_DEBUG(Args&& ... ) { }

template<class... Args>
inline void SPEEDLOG_CRITICAL(Args&& ...) { }

template<class... Args>
inline void SPEEDLOG_TRACE(Args&& ...) { }

template<class... Args>
inline void SPEEDLOG_SETLEVEL(Args&& ...) { }

template<class... Args>
inline void SPEEDLOG_ENABLE_BACKTRACE(Args&& ...) { }

inline void SPEEDLOG_DISABLE_BACKTRACE(){}

inline void SPEEDLOG_DUMP_BACKTRACE(){}

inline void SPEEDLOG_FLUSH(){}

template<class... Args>
inline void SPEEDLOG_SET_NEW_BASIC(Args&& ...) { }

template<class... Args>
inline void SPEEDLOG_SET_NEW_ROTATING(Args&& ...) { }

/*
// DO NOT USE IF YOU ARE NOT GOING TO SHUTDOWN
template<class... Args>
inline void SPEEDLOG_FLUSH_EVERY(const unsigned secs)(Args&& ... args) { sink(args...); }
*/

// DO NOT USE UNLESS FINISHING
/*
inline void SPEEDLOG_SHUTDOWN_ALL() {}
*/


#endif

#endif
