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
			class x09Token final : public baseToken
			{

			private:

				// UTC datetime
				int64_t m_data;

			public:

				x09Token(void) noexcept;
				virtual ~x09Token(void);

				int parse(const std::vector<char>& m_fileData, std::vector<char>::const_iterator& m_position) override;
				void print(void) noexcept override;

				int64_t getInt64Value(void) const noexcept;

			};
		}
	}
}
