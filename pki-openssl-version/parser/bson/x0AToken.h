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
			class x0AToken final : public baseToken
			{

			private:

				// Null value

			public:

				x0AToken(void) noexcept;
				virtual ~x0AToken(void);

				int parse(const std::vector<char>& m_fileData, std::vector<char>::const_iterator& m_position) override;
				void print(void) noexcept override;

			};
		}
	}
}
