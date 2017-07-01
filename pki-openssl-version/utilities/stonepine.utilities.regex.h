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
#include <vector>
#include <regex>

namespace Stonepine
{
	namespace Utilities
	{
		class regex final
		{

		public:

			UTILITIES_API static const bool isStringConformToRegexExpression(const std::string& f_stringToCheck, const std::string& f_regularExpression) noexcept;

			UTILITIES_API static const int64_t extractRegexMatchAsInteger(const std::string& f_stringToMatch, const uint8_t fm_indexBegin, const uint8_t fm_indexEnd) noexcept (false);

			UTILITIES_API static const int64_t extractRegexMatchAsInteger(const std::string& f_stringToMatch, const std::string& f_regularExpression) noexcept (false);
			UTILITIES_API static const std::string extractRegexMatchAsString(const std::string& f_stringToMatch, const std::string& f_regularExpression) noexcept (false);

			UTILITIES_API static std::shared_ptr<std::vector<int64_t>> extractRegexMatchesAsIntegers(const std::string& f_stringToMatch, const std::string& f_regularExpression) noexcept (false);

			// Regex base functions
			UTILITIES_API static const void removeAllSubstrings(std::string& fm_inputString, const std::string& fm_substring) noexcept;
			UTILITIES_API static const std::string removeAllSubstrings(const std::string& fm_inputString, const std::string& fm_substring) noexcept;

		};
	}
}
