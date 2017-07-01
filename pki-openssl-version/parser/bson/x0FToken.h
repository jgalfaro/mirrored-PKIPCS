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
			class x0FToken final : public baseToken
			{

			private:

				// JavaScript code w/ scope
				int32_t m_size;

				std::string m_string;

				std::vector<char> m_subDocument;

				// Embedded document
				std::shared_ptr<std::vector<std::shared_ptr<baseToken>>> m_data;

			public:

				x0FToken(void) noexcept;
				virtual ~x0FToken(void);

				int parse(const std::vector<char>& m_fileData, std::vector<char>::const_iterator& m_position) override;
				void print(void) noexcept override;

				std::string getStringValue(void) const noexcept;
				std::shared_ptr<std::vector<std::shared_ptr<baseToken>>> getDocumentValue(void) const noexcept;

			};
		}
	}
}
