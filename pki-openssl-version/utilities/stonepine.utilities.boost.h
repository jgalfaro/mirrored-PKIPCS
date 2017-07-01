/*
Télécom SudParis, Télécom Saint-Étienne
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

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

#include <boost/date_time/posix_time/posix_time.hpp>

#include <boost/lexical_cast.hpp>

namespace Stonepine
{
	namespace Utilities
	{
		class boost final
		{

		public:

			UTILITIES_API static const ::boost::uuids::uuid generateUuid(void) noexcept;
			UTILITIES_API static const ::boost::uuids::uuid extractUuid(const std::string& fm_dataWithAUuid) noexcept;

			// Conversion utilities
			UTILITIES_API static const std::string generateUuidAsString(void) noexcept;
			UTILITIES_API static const std::string extractUuidAsString(const std::string& fm_dataWithAUuid) noexcept;

			UTILITIES_API static const ::boost::uuids::uuid stringToUuid(const std::string& fm_uuid) noexcept;
			UTILITIES_API static const std::string uuidToString(const ::boost::uuids::uuid& fm_uuid) noexcept;

			UTILITIES_API static const ::boost::posix_time::ptime timeToPtime(const time_t& fm_time) noexcept;
			UTILITIES_API static const time_t pTimeToTime(const ::boost::posix_time::ptime& fm_time) noexcept;

		};
	}
}
