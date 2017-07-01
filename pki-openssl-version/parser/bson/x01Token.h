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
			class x01Token final : public baseToken
			{

			private:

				// 64-bit binary floating point
				double m_data;

			public:

				x01Token(void) noexcept;
				virtual ~x01Token(void);

				int parse(const std::vector<char>& m_fileData, std::vector<char>::const_iterator& m_position) override;
				void print(void) override;

				double getDoubleValue(void) const noexcept;

			};
		}
	}
}
