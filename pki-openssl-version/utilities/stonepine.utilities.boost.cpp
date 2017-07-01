/*
Télécom SudParis, Télécom Saint-Étienne
2016, December
Pierre-Olivier Rocher
*/

#include <stonepine/logger/logger.h>

#include "stonepine.utilities.regex.h"

#include "stonepine.utilities.boost.h"

namespace Stonepine
{
	namespace Utilities
	{
		const ::boost::uuids::uuid boost::generateUuid(void) noexcept
		{
			return ::boost::uuids::random_generator()();
		}

		const::boost::uuids::uuid boost::extractUuid(const std::string& fm_dataWithAUuid) noexcept
		{
			return stringToUuid(extractUuidAsString(fm_dataWithAUuid));
		}

		const std::string boost::generateUuidAsString(void) noexcept
		{
			return ::boost::lexical_cast<std::string>(generateUuid());
		}

		const std::string boost::extractUuidAsString(const std::string& fm_dataWithAUuid) noexcept
		{
			try
			{
				return Stonepine::Utilities::regex::extractRegexMatchAsString(fm_dataWithAUuid, "^.*?([a-fA-F0-9]{8}-[a-fA-F0-9]{4}-[a-fA-F0-9]{4}-[a-fA-F0-9]{4}-[a-fA-F0-9]{12}).*?$");
			}
			catch (const std::exception& e)
			{
				Stonepine::LOG_ERROR("[utilities] unable to extract UUID -> ", e.what());
			}

			return std::string();
		}

		const ::boost::uuids::uuid boost::stringToUuid(const std::string& fm_uuid) noexcept
		{
			return ::boost::lexical_cast<::boost::uuids::uuid>(fm_uuid);
		}

		const std::string boost::uuidToString(const ::boost::uuids::uuid& fm_uuid) noexcept
		{
			return ::boost::lexical_cast<std::string>(fm_uuid);
		}

		const::boost::posix_time::ptime boost::timeToPtime(const time_t& fm_time) noexcept
		{
			return ::boost::posix_time::from_time_t(fm_time);
		}

		const time_t boost::pTimeToTime(const ::boost::posix_time::ptime& fm_time) noexcept
		{
			// From http://stackoverflow.com/questions/4461586/how-do-i-convert-boostposix-timeptime-to-time-t

			auto eT = ::boost::posix_time::ptime(::boost::gregorian::date(1970, ::boost::gregorian::Jan, 1));

			return (fm_time - eT).total_seconds();
		}
	}
}
