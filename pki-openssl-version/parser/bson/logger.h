/*
2016, February
Pierre-Olivier Rocher
*/

#pragma once

#include <mutex>
#include <string>
#include <iostream>

#include "config.h"

namespace Stonepine
{
	class logger
	{

	private:

		static std::mutex m_logMutex;

	public:

		logger(void);
		~logger(void);

		static void info(const std::string& fm_info);
		static void debug(const std::string& fm_debugInfo);
		static void warning(const std::string& fm_warningInfo);
		static void error(const std::string& fm_errorInfo);

	};
}
