/*
2016, February
Pierre-Olivier Rocher
*/

#include "logger.h"

namespace Stonepine
{
	std::mutex logger::m_logMutex;

	logger::logger(void)
	{

	}

	logger::~logger(void)
	{

	}

	void logger::info(const std::string& fm_info)
	{
		std::lock_guard<std::mutex> lock(m_logMutex);
		std::cout << "INFO: " << fm_info << std::endl;
	}

	void logger::debug(const std::string& fm_debugInfo)
	{
#if __WITH_DEBUG_INFO__ == 1

		std::lock_guard<std::mutex> lock(m_logMutex);
		std::cout << "DEBUG: " << fm_debugInfo << std::endl;
#endif
	}

	void logger::warning(const std::string& fm_warningInfo)
	{
#if __WITH_WARNING_INFO__ == 1
		std::lock_guard<std::mutex> lock(m_logMutex);
		std::cout << "WARNING: " << fm_warningInfo << std::endl;
#endif
	}

	void logger::error(const std::string& fm_errorInfo)
	{
		std::lock_guard<std::mutex> lock(m_logMutex);
		std::cout << "ERROR: " << fm_errorInfo << std::endl;
	}
}
