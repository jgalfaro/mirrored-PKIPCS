/*
Télécom SudParis
2016, December
Pierre-Olivier Rocher
*/

#include "stonepine.utilities.regex.h"

namespace Stonepine
{
	namespace Utilities
	{
		const bool regex::isStringConformToRegexExpression(const std::string& f_stringToCheck, const std::string& f_regularExpression) noexcept
		{
			const auto r = std::regex(f_regularExpression, std::regex_constants::ECMAScript);

			std::smatch base_match;

			return std::regex_match(f_stringToCheck, base_match, r);
		}

		const int64_t regex::extractRegexMatchAsInteger(const std::string& f_stringToMatch, const uint8_t fm_indexBegin, const uint8_t fm_indexEnd) noexcept(false)
		{
			std::string rE = {"("};

			for (int i = fm_indexBegin; i < fm_indexEnd + 1; i++)
			{
				rE.append(std::to_string(i) + "|");

				if (i == fm_indexEnd)
				{
					rE.append(std::to_string(i) + ")");
				}
			}

			return extractRegexMatchAsInteger(f_stringToMatch, rE);
		}

		const int64_t regex::extractRegexMatchAsInteger(const std::string& f_stringToMatch, const std::string& f_regularExpression) noexcept (false)
		{
			auto r = std::regex(f_regularExpression, std::regex_constants::ECMAScript);

			auto s = std::smatch();

			if (std::regex_match(f_stringToMatch, s, r))
			{
				auto mS = s[1];

				return static_cast<int64_t>(std::stoll(mS.str()));
			}

			throw std::runtime_error("no match found");
		}

		const std::string regex::extractRegexMatchAsString(const std::string& f_stringToMatch, const std::string& f_regularExpression) noexcept (false)
		{
			auto r = std::regex(f_regularExpression, std::regex_constants::ECMAScript);

			auto s = std::smatch();

			if (std::regex_match(f_stringToMatch, s, r))
			{
				auto mS = s[1];

				return mS.str();
			}

			throw std::runtime_error("no match found");
		}

		std::shared_ptr<std::vector<int64_t>> regex::extractRegexMatchesAsIntegers(const std::string& f_stringToMatch, const std::string& f_regularExpression) noexcept(false)
		{
			auto r = std::regex(f_regularExpression, std::regex_constants::ECMAScript);

			auto s = std::smatch();

			auto v = std::make_shared<std::vector<int64_t>>();

			if (std::regex_match(f_stringToMatch, s, r))
			{
				std::ssub_match sm = s[1];

				v->push_back(std::stoi(sm.str()));
			}

			throw std::runtime_error("no match found");
		}

		const void removeAllSubstrings(std::string& fm_inputString, const std::string& fm_substring) noexcept
		{
			std::regex_replace(fm_inputString, std::regex(fm_substring), "");
		}

		const std::string regex::removeAllSubstrings(const std::string& fm_inputString, const std::string& fm_substring) noexcept
		{
			std::string r = fm_inputString;

			std::regex_replace(r, std::regex(fm_substring), "");

			return r;
		}
	}
}
 
