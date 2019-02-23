/*
2016, February
Pierre-Olivier Rocher
*/

#include "x09Token.h"

namespace Stonepine
{
	namespace Parsers
	{
		namespace Bson
		{
			x09Token::x09Token(void) noexcept : baseToken(tokenType::x09)
			{

			}

			x09Token::~x09Token(void)
			{

			}

			int x09Token::parse(const std::vector<char>& m_fileData, std::vector<char>::const_iterator& m_position)
			{
				auto iTB = m_position + 1;

				getName(m_fileData, iTB, m_name);

				getInt64(iTB, m_data);

				std::advance(m_position, std::distance(m_position + 1, iTB));

				return 0;
			}

			int64_t x09Token::getInt64Value(void) const noexcept
			{
				return m_data;
			}

			void x09Token::print(void) noexcept
			{
				std::cout << m_name << " => " << m_data << std::endl;
			}
		}
	}
}
 
