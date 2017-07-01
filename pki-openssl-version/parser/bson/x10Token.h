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
			class x10Token final : public baseToken
			{

			private:

				// 32-bit integer
				int32_t m_data;

			public:

				x10Token(void) noexcept;
				virtual ~x10Token(void);

				int parse(const std::vector<char>& m_fileData, std::vector<char>::const_iterator& m_position) override;
				void print(void) noexcept override;

				int32_t getInt32Value(void) const noexcept;

			};
		}
	}
}
