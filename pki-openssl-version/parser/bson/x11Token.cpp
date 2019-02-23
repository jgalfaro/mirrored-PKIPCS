/*
2016, February
Pierre-Olivier Rocher
*/

#include "x11Token.h"

namespace Stonepine
{
	namespace Parsers
	{
		namespace Bson
		{
			x11Token::x11Token(void) noexcept : baseToken(tokenType::x11)
			{

			}

			x11Token::~x11Token(void)
			{

			}

			int x11Token::parse(const std::vector<char>& m_fileData, std::vector<char>::const_iterator& m_position)
			{
				auto iTB = m_position + 1;

				getName(m_fileData, iTB, m_name);

				getInt64(iTB, m_data);

				std::advance(m_position, std::distance(m_position + 1, iTB));

				return 0;
			}

			int64_t x11Token::getInt64Value(void) const noexcept
			{
				return m_data;
			}

			void x11Token::print(void) noexcept
			{
				std::cout << m_name << " => " << m_data << std::endl;
			}
		}
	}
}
 
