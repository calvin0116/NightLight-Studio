#include "SpeedLog.h"

// Default log
SpeedLog SPEEDLOG; //= spdlog::basic_logger_st("log", "logs/log.txt");

bool SpeedLog::_first = true;

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
		if (!_first)
		{
			_log = spdlog::default_logger();
		}
		else
		{
			_first = false;
			// Sets up Speed Log here to prevent weird issues
			SPEEDLOG = SetNewBasicLogger(DEFAULT_LOG_NAME, DEFAULT_LOG_PATH);

			// If first used SpeedLog is NOT SPEEDLOG, then set to default
			if (*this != SPEEDLOG)
			{
				_log = spdlog::default_logger();
			}
		}
	}
	return _log;
}

std::shared_ptr<spdlog::logger> SpeedLog::GetLogger()
{
	return this->operator->();
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