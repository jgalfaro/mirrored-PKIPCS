/*
Télécom SudParis
2016, December
Pierre-Olivier Rocher
*/

#include <stonepine/architecture/architecture.h>
#include <stonepine/logger/logger.h>

#include "stonepine.utilities.time.h"

namespace Stonepine
{
	namespace Utilities
	{
		const time_t time::getLocalTime(void) noexcept
		{
			return std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
		}

		const time_t time::getUniversalTime(void) noexcept
		{
			auto uT = tm();

#ifdef __WINDOWS__
			gmtime_s(&uT, reinterpret_cast<const time_t*>(getLocalTime()));
#else
			gmtime_r(reinterpret_cast<const time_t*>(getLocalTime()), &uT);
#endif

			return std::mktime(&uT);
		}

		const time_t time::getLocalTimeInXDays(const int fm_numberOfDays) noexcept
		{
			auto xDaysFromNow = std::chrono::system_clock::now() + oneDay(fm_numberOfDays);

			return std::chrono::system_clock::to_time_t(xDaysFromNow);
		}

		const time_t time::getTimeInXDays(const time_t& fm_time, const int fm_numberOfDays) noexcept
		{
			auto xDaysFromNow = std::chrono::system_clock::from_time_t(fm_time) + oneDay(fm_numberOfDays);

			return std::chrono::system_clock::to_time_t(xDaysFromNow);
		}

		const time_t time::getLocalTimeInXMonths(const int fm_numberOfMonths) noexcept
		{
			auto xMonthsFromNow = std::chrono::system_clock::now() + oneMonth(fm_numberOfMonths);

			return std::chrono::system_clock::to_time_t(xMonthsFromNow);
		}

		const time_t time::getTimeInXMonths(const time_t& fm_time, const int fm_numberOfMonths) noexcept
		{
			auto xMonthsFromNow = std::chrono::system_clock::from_time_t(fm_time) + oneMonth(fm_numberOfMonths);

			return std::chrono::system_clock::to_time_t(xMonthsFromNow);
		}

		const time_t time::getLocalTimeInXYears(const int f_numberOfYears) noexcept
		{
			auto xYearsFromNow = std::chrono::system_clock::now() + oneYear(f_numberOfYears);

			return std::chrono::system_clock::to_time_t(xYearsFromNow);
		}

		const time_t time::getTimeInXYears(const time_t& fm_time, const int f_numberOfYears) noexcept
		{
			auto xYearsFromNow = std::chrono::system_clock::from_time_t(fm_time) + oneYear(f_numberOfYears);

			return std::chrono::system_clock::to_time_t(xYearsFromNow);
		}
		
		const std::string time::getLocalTimeAsString(const time_t& fm_time) noexcept
		{
			auto lT = tm();

#ifdef __WINDOWS__
			char buffer[32];

			localtime_s(&lT, &fm_time);

			auto e = asctime_s(buffer, 32, &lT);

			if (e != 0)
			{
				Stonepine::LOG_ERROR("unable to convert time to string -> ", e);
			}

			return std::string(buffer);

#else
			localtime_r(&fm_time, &lT);

			return std::string(asctime(&lT));
#endif
		}
		
		const std::string time::getUniversalTimeAsString(const time_t& fm_time) noexcept
		{
			auto lT = tm();

#ifdef __WINDOWS__
			char buffer[32];

			gmtime_s(&lT, &fm_time);

			auto e = asctime_s(buffer, 32, &lT);

			if (e != 0)
			{
				Stonepine::LOG_ERROR("unable to convert time to string -> ", e);
			}

			return std::string(buffer);

#else
			gmtime_r(&fm_time, &lT);

			return std::string(asctime(&lT));
#endif
		}
		
		const std::string time::getLocalTimeAsString(const time_t& fm_localTime, const std::string& fm_format) noexcept
		{
			auto lT = tm();
			std::ostringstream oSS;

#ifdef __WINDOWS__
			localtime_s(&lT, &fm_localTime);

			oSS << std::put_time(&lT, fm_format.c_str());
#else
			localtime_r(&fm_localTime, &lT);
#endif
			return oSS.str();
		}

		const std::string time::getUniversalTimeAsString(const time_t& fm_localTime, const std::string& fm_format) noexcept
		{
			auto lT = tm();
			std::ostringstream oSS;

#ifdef __WINDOWS__
			gmtime_s(&lT, &fm_localTime);

			oSS << std::put_time(&lT, fm_format.c_str());
#else
			gmtime_r(&fm_localTime, &lT);
#endif
			return oSS.str();
		}
	}
}
