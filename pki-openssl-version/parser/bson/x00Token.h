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
			class x00Token final : public baseToken
			{

			private:

				// End of BSON Document

			public:

				x00Token(void) noexcept;
				virtual ~x00Token(void);

				int parse(const std::vector<char>& m_fileData, std::vector<char>::const_iterator& m_position) noexcept override;
				void print(void) noexcept override;

			};
		}
	}
}
