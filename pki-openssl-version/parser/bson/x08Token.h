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
			class x08Token final : public baseToken
			{

			private:

				// Boolean "false"
				bool m_data;

			public:

				x08Token(void) noexcept;
				virtual ~x08Token(void);

				int parse(const std::vector<char>& m_fileData, std::vector<char>::const_iterator& m_position) override;
				void print(void) noexcept override;

				bool getBoolValue(void) const noexcept;

			};
		}
	}
}
