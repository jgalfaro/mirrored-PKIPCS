/*
2016, February
Pierre-Olivier Rocher
*/

#include "x12Token.h"

namespace Stonepine
{
	namespace Parsers
	{
		namespace Bson
		{
			x12Token::x12Token(void) noexcept : baseToken(tokenType::x12)
			{
				m_data = 0;
			}

			x12Token::~x12Token(void)
			{

			}

			int x12Token::parse(const std::vector<char>& m_fileData, std::vector<char>::const_iterator& m_position)
			{
				auto iTB = m_position + 1;

				getInt64(iTB, m_data);

				std::advance(m_position, std::distance(m_position + 1, iTB));

				return 0;
			}

			int64_t x12Token::getInt64Value(void) const noexcept
			{
				return m_data;
			}

			void x12Token::print(void) noexcept
			{
				std::cout << m_name << " => " << m_data << std::endl;
			}
		}
	}
}
 
