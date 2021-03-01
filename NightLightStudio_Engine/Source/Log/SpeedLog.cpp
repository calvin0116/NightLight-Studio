#include "SpeedLog.h"

// Default log
SpeedLog SPEEDLOG; //= spdlog::basic_logger_st("log", "logs/log.txt");

SpeedLog::SpeedLog()
{
}

SpeedLog::~SpeedLog()
{
	_log.reset();
}

SpeedLog::SpeedLog(const SpeedLog& other)
{
	_log = other._log;
}

SpeedLog::SpeedLog(const std::shared_ptr<spdlog::logger>& other)
{
	_log = other;
}

SpeedLog& SpeedLog::operator=(const SpeedLog& other)
{
	_log.reset();
	_log = other._log;
	return *this;
}

SpeedLog& SpeedLog::operator=(const std::shared_ptr<spdlog::logger>& other)
{
	_log.reset();
	_log = other;
	return *this;
}

bool SpeedLog::operator==(const SpeedLog& other)
{
	return _log == other._log;
}

bool SpeedLog::operator==(const std::shared_ptr<spdlog::logger>& other)
{
	return _log == other;
}

bool SpeedLog::operator!=(const SpeedLog& other)
{
	return _log != other._log;
}

bool SpeedLog::operator!=(const std::shared_ptr<spdlog::logger>& other)
{
	return _log != other;
}

std::shared_ptr<spdlog::logger> SpeedLog::operator->()
{
	if (!_log)
	{
		if (spdlog::get(DEFAULT_LOG_NAME))
		{
			*this = spdlog::get(DEFAULT_LOG_NAME);
		}
		else
		{
			*this = spdlog::basic_logger_st(DEFAULT_LOG_NAME, DEFAULT_LOG_PATH);
		}
	}
	return _log;
}

std::shared_ptr<spdlog::logger> SpeedLog::GetLogger()
{
	return _log;
}

std::shared_ptr<spdlog::logger> SpeedLog::ResetLogger()
{
	_log.reset();
	_log = spdlog::default_logger();
	return _log;
}

std::shared_ptr<spdlog::logger> SpeedLog::SetNewBasicLogger(const std::string& name, const std::string& path)
{
	_log.reset();
	_log = spdlog::basic_logger_st(name, path);
	return _log;
}

std::shared_ptr<spdlog::logger> SpeedLog::SetNewRotatingLogger(const std::string& name, const std::string& path, size_t maxSize, size_t maxFiles)
{
	_log.reset();
	_log = spdlog::rotating_logger_st(name, path, maxSize, maxFiles);
	return _log;
}