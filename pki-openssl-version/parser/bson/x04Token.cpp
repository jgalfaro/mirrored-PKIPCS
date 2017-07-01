/*
2016, February
Pierre-Olivier Rocher
*/

#include "x04Token.h"

namespace Stonepine
{
	namespace Parsers
	{
		namespace Bson
		{
			x04Token::x04Token(void) noexcept : baseToken(tokenType::x04)
			{

			}

			x04Token::~x04Token(void)
			{

			}

			int x04Token::parse(const std::vector<char>& m_fileData, std::vector<char>::const_iterator& m_position)
			{
				auto iTB = m_position + 1;

				getName(m_fileData, iTB, m_name);
				getDocument(m_fileData, m_position, m_subDocument);

				m_data = parseOneDocument(m_subDocument);

				std::advance(m_position, std::distance(m_position + 1, iTB));

				return 0;
			}

			std::shared_ptr<std::vector<std::shared_ptr<baseToken>>> x04Token::getDocumentValue(void) const noexcept
			{
				return m_data;
			}

			void x04Token::print(void) noexcept
			{
				std::cout << m_name << std::endl;

				for (auto iTD : *m_data)
				{
					iTD->print();
				}
			}
		}
	}
}
