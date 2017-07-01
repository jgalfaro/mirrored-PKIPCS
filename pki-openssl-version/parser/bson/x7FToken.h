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
			class x7FToken final : public baseToken
			{

			private:

				// Max key

			public:

				x7FToken(void) noexcept;
				virtual ~x7FToken(void);

				int parse(const std::vector<char>& m_fileData, std::vector<char>::const_iterator& m_position) override;
				void print(void) noexcept override;

			};
		}
	}
}
