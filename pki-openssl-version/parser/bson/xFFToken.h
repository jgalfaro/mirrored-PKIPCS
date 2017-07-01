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
			class xFFToken final : public baseToken
			{

			private:

				// Min key

			public:

				xFFToken(void) noexcept;
				virtual ~xFFToken(void);

				int parse(const std::vector<char>& m_fileData, std::vector<char>::const_iterator& m_position) override;
				void print(void) noexcept override;

			};
		}
	}
}
