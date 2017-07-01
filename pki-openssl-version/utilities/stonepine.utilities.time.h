/*
Télécom SudParis
2016, December
Pierre-Olivier Rocher
*/

#pragma once

#if _WIN32
#ifdef UTILITIES_EXPORTS
#define UTILITIES_API __declspec(dllexport)
#else
#define UTILITIES_API __declspec(dllimport)
#endif
#else
#define UTILITIES_API
#endif

#include <memory>
#include <iostream>
#include <iomanip>
#include <ctime>
#include <sstream>
#include <chrono>

#include <stonepine/architecture/architecture.h>

namespace Stonepine
{
	namespace Utilities
	{
		class time final
		{

			typedef std::chrono::duration<int, std::ratio<3600>> oneHour;
			typedef std::chrono::duration<int, std::ratio<3600 * 24>> oneDay;
			typedef std::chrono::duration<int, std::ratio<3600 * 24 * 31>> oneMonth;
			typedef std::chrono::duration<int, std::ratio<3600 * 24 * 365>> oneYear;

		public:

			// Get time
			UTILITIES_API static const time_t getLocalTime(void) noexcept;
			UTILITIES_API static const time_t getUniversalTime(void) noexcept;

			UTILITIES_API static const time_t getLocalTimeInXDays(const int fm_numberOfDays) noexcept;
			UTILITIES_API static const time_t getTimeInXDays(const time_t& fm_time, const int fm_numberOfDays) noexcept;
				
			UTILITIES_API static const time_t getLocalTimeInXMonths(const int fm_numberOfMonths) noexcept;
			UTILITIES_API static const time_t getTimeInXMonths(const time_t& fm_time, const int fm_numberOfMonths) noexcept;

			UTILITIES_API static const time_t getLocalTimeInXYears(const int f_numberOfYears) noexcept;
			UTILITIES_API static const time_t getTimeInXYears(const time_t& fm_time, const int f_numberOfYears) noexcept;

			// Conversion utilities
			UTILITIES_API static const std::string getLocalTimeAsString(const time_t& fm_localTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now())) noexcept;
			UTILITIES_API static const std::string getUniversalTimeAsString(const time_t& fm_localTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now())) noexcept;
			
			UTILITIES_API static const std::string getLocalTimeAsString(const time_t& fm_localTime, const std::string& fm_format) noexcept;
			UTILITIES_API static const std::string getUniversalTimeAsString(const time_t& fm_localTime, const std::string& fm_format) noexcept;

			template <class Function>
			static const int64_t getFunctionCallTime(Function&& f_function);

		};

		template<class Function>
		inline const int64_t time::getFunctionCallTime(Function&& f_function)
		{
#ifdef __WINDOWS__
			int64_t b = GetTickCount();

			f_function();

			return GetTickCount() - b;
#else
			return 0;
#endif
		}
	}
}
