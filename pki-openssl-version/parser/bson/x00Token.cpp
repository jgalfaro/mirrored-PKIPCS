/*
2016, February
Pierre-Olivier Rocher
*/

#include "x00Token.h"

namespace Stonepine
{
	namespace Parsers
	{
		namespace Bson
		{
			x00Token::x00Token(void) noexcept : baseToken(tokenType::x00)
			{
				
			}

			x00Token::~x00Token(void)
			{

			}

			int x00Token::parse(const std::vector<char>& m_fileData, std::vector<char>::const_iterator& m_position) noexcept
			{
				return -1;
			}

			void x00Token::print(void) noexcept
			{

			}
		}
	}
}
 
