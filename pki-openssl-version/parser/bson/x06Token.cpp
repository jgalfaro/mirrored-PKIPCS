/*
2016, February
Pierre-Olivier Rocher
*/

#include "x06Token.h"

namespace Stonepine
{
	namespace Parsers
	{
		namespace Bson
		{
			x06Token::x06Token(void) noexcept : baseToken(tokenType::x06)
			{

			}

			x06Token::~x06Token(void)
			{

			}

			int x06Token::parse(const std::vector<char>& m_fileData, std::vector<char>::const_iterator& m_position)
			{
				auto iTB = m_position + 1;

				getName(m_fileData, iTB, m_name);

				std::advance(m_position, std::distance(m_position + 1, iTB));

				return 0;
			}

			void x06Token::print(void) noexcept
			{
				std::cout << m_name << " => Deprecated !" << std::endl;
			}
		}
	}
}
