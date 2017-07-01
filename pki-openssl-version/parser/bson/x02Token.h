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
			class x02Token final : public baseToken
			{

			private:

				// UTF-8 string
				std::string m_data;

			public:

				x02Token(void) noexcept;
				virtual ~x02Token(void);

				int parse(const std::vector<char>& m_fileData, std::vector<char>::const_iterator& m_position) override;
				void print(void) noexcept override;

				std::string getStringValue(void) const noexcept;

			};
		}
	}
}
