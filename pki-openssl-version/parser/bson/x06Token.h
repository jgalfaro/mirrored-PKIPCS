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
			class x06Token final : public baseToken
			{

			private:

				// Undefined (value) — Deprecated

			public:

				x06Token(void) noexcept;
				virtual ~x06Token(void);

				int parse(const std::vector<char>& m_fileData, std::vector<char>::const_iterator& m_position) override;
				void print(void) noexcept override;

			};
		}
	}
}
