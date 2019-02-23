/*
2016, February
Pierre-Olivier Rocher
*/

#include "x10Token.h"

namespace Stonepine
{
	namespace Parsers
	{
		namespace Bson
		{
			x10Token::x10Token(void) noexcept : baseToken(tokenType::x10)
			{
				m_data = 0;
			}

			x10Token::~x10Token(void)
			{

			}

			int x10Token::parse(const std::vector<char>& m_fileData, std::vector<char>::const_iterator& m_position)
			{
				auto iTB = m_position + 1;

				getName(m_fileData, iTB, m_name);
				getInt32(iTB, m_data);

				std::advance(m_position, std::distance(m_position + 1, iTB));

				return 0;
			}

			int32_t x10Token::getInt32Value(void) const noexcept
			{
				return m_data;
			}

			void x10Token::print(void) noexcept
			{
				std::cout << m_name << " => " << m_data << std::endl;
			}
		}
	}
}
 
