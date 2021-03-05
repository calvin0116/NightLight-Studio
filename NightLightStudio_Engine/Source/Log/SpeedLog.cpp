#include "SpeedLog.h"
#include <ctime>

// Default log
SpeedLog SPEEDLOG; //= spdlog::basic_logger_st("log", "logs/log.txt");

#if SPEEDLOG_ALLOW
bool SpeedLog::_first = true;

SpeedLog::SpeedLog(){}

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
			ResetLogger();
		}
		else
		{
			_first = false;
			// Sets up Speed Log here to prevent weird issues
			if (SPEEDLOG_LOG_TO_FILE)
			{
				// Log by day
				time_t rawtime;
				time(&rawtime);
				struct tm now;
				localtime_s(&now, &rawtime);

				// Path name with date included
				std::string currentDate = ""; // = "[";
				if (now.tm_mday < 10)
				{
					currentDate.append("0");
				}
				currentDate.append(std::to_string(now.tm_mday));
				currentDate.append("_");

				if (now.tm_mon < 10)
				{
					currentDate.append("0");
				}
				currentDate.append(std::to_string(now.tm_mon + 1));
				currentDate.append("_");

				currentDate.append(std::to_string(now.tm_year + 1900));
				//currentDate.append("]");

				std::string path = "logs/";
				path.append(currentDate).append(".txt");

				SPEEDLOG.SetNewBasicLogger(currentDate, path);
				SPEEDLOG("||||||||||||||||||||||||||||||||||||||||||||||||||||||||||\n");
				SPEEDLOG("SpeedLog : Initializing Speed Log ========================\n");
			}
			else
			{
				SPEEDLOG.ResetLogger();
			}
			// If first used SpeedLog is NOT SPEEDLOG, then set to default
			if (this != &SPEEDLOG)
			{
				ResetLogger();
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

std::shared_ptr<spdlog::logger> SpeedLog::SetNewBasicLogger(const std::string& name, const std::string& path, bool truncate)
{
	_log.reset();
	_log = spdlog::basic_logger_st(name, path, truncate);
	return _log;
}

std::shared_ptr<spdlog::logger> SpeedLog::SetNewRotatingLogger(const std::string& name, const std::string& path, size_t maxSize, size_t maxFiles, bool rotate_on_open)
{
	_log.reset();
	_log = spdlog::rotating_logger_st(name, path, maxSize, maxFiles, rotate_on_open);
	return _log;
}

std::shared_ptr<spdlog::logger> SpeedLog::SetNewDailyLogger(const std::string& name, const std::string& path, int hour, int minute, bool truncate)
{
	_log.reset();
	_log = spdlog::daily_logger_st(name, path, hour, minute, truncate);
	return _log;
}

#else


#endif
