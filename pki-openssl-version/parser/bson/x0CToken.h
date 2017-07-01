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
			class x0CToken final : public baseToken
			{

			private:

				// DBPointer — Deprecated
				std::string m_data;

			public:

				x0CToken(void) noexcept;
				virtual ~x0CToken(void);

				int parse(const std::vector<char>& m_fileData, std::vector<char>::const_iterator& m_position) override;
				void print(void) noexcept override;

				std::string getDbPointerValue(void) const noexcept;

			};
		}
	}
}
