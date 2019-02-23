/*
2016, February
Pierre-Olivier Rocher
*/

#include "x07Token.h"

namespace Stonepine
{
	namespace Parsers
	{
		namespace Bson
		{
			x07Token::x07Token(void) noexcept : baseToken(tokenType::x07)
			{

			}

			x07Token::~x07Token(void)
			{

			}

			int x07Token::parse(const std::vector<char>& m_fileData, std::vector<char>::const_iterator& m_position)
			{
				auto iTB = m_position + 1;

				getName(m_fileData, iTB, m_name);

				m_data = std::string(iTB, iTB + 12);

				std::advance(iTB, 12);

				std::advance(m_position, std::distance(m_position + 1, iTB));

				return 0;
			}

			void x07Token::print(void)
			{
				std::cout << m_name << " => " << getObjectIdValue() << std::endl;
			}

			std::string x07Token::getObjectIdValue(void) const noexcept
			{
				return string2Hex(m_data);
			}
		}
	}
}
 
