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
			class x07Token final : public baseToken
			{

			private:

				// ObjectId
				std::string m_data;

			public:

				x07Token(void) noexcept;
				virtual ~x07Token(void);

				int parse(const std::vector<char>& m_fileData, std::vector<char>::const_iterator& m_position) override;
				void print(void) override;

				std::string getObjectIdValue(void) const noexcept;

			};
		}
	}
}
