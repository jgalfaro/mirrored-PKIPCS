/*
2016, February
Pierre-Olivier Rocher
*/

#include "x02Token.h"

namespace Stonepine
{
	namespace Parsers
	{
		namespace Bson
		{
			x02Token::x02Token(void) noexcept : baseToken(tokenType::x02)
			{
				
			}

			x02Token::~x02Token(void)
			{

			}

			int x02Token::parse(const std::vector<char>& m_fileData, std::vector<char>::const_iterator& m_position)
			{
				auto iTB = m_position + 1;

				getName(m_fileData, iTB, m_name);
				getString(m_fileData, iTB, m_data);

				std::advance(m_position, std::distance(m_position + 1, iTB));

				return 0;
			}

			std::string x02Token::getStringValue(void) const noexcept
			{
				return m_data;
			}

			void x02Token::print(void) noexcept
			{
				std::cout << m_name << " => " << m_data << std::endl;
			}
		}
	}
}
