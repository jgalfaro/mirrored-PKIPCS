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
			class x05Token final : public baseToken
			{

			private:

				// Binary data
				std::string m_data;

			public:

				x05Token(void) noexcept;
				virtual ~x05Token(void);

				int parse(const std::vector<char>& m_fileData, std::vector<char>::const_iterator& m_position) override;
				void print(void) noexcept override;

				std::string getBinaryValue(void) const noexcept;

			};
		}
	}
}
