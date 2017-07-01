/*
2016, February
Pierre-Olivier Rocher
*/

#pragma once

#include "baseToken.h"

#include "bsonParser.h"

namespace Stonepine
{
	namespace Parsers
	{
		namespace Bson
		{
			class x03Token final : public baseToken
			{

			private:

				std::vector<char> m_subDocument;

				// Embedded document
				std::shared_ptr<std::vector<std::shared_ptr<baseToken>>> m_data;

			public:

				x03Token(void) noexcept;
				virtual ~x03Token(void);

				int parse(const std::vector<char>& m_fileData, std::vector<char>::const_iterator& m_position) override;
				void print(void) noexcept override;

				std::shared_ptr<std::vector<std::shared_ptr<baseToken>>> getDocumentValue(void) const noexcept;

			};
		}
	}
}
