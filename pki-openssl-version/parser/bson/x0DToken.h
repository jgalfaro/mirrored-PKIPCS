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
			class x0DToken final : public baseToken
			{

			private:

				// JavaScript code
				std::string m_data;

			public:

				x0DToken(void) noexcept;
				virtual ~x0DToken(void);

				int parse(const std::vector<char>& m_fileData, std::vector<char>::const_iterator& m_position) override;
				void print(void) noexcept override;

				std::string getStringValue(void) const noexcept;

			};
		}
	}
}
