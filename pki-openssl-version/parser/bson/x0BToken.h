/*
2016, February
Pierre-Olivier Rocher
*/

#pragma once

#include "baseToken.h"

namespace Stonepine
{
	namespace Parsers
	{
		namespace Bson
		{
			class x0BToken final : public baseToken
			{

			private:

				// Regular expression
				std::string m_regexPattern;
				std::string m_regexOptions;

			public:

				x0BToken(void) noexcept;
				virtual ~x0BToken(void);

				int parse(const std::vector<char>& m_fileData, std::vector<char>::const_iterator& m_position) override;
				void print(void) noexcept override;

				std::string getRegexPatternValue(void) const noexcept;
				std::string getRegexOptionsValue(void) const noexcept;

			};
		}
	}
}
