/*
2016, February
Pierre-Olivier Rocher
*/

#include "x08Token.h"

namespace Stonepine
{
	namespace Parsers
	{
		namespace Bson
		{
			x08Token::x08Token(void) noexcept : baseToken(tokenType::x08)
			{
				m_data = false;
			}

			x08Token::~x08Token(void)
			{

			}

			int x08Token::parse(const std::vector<char>& m_fileData, std::vector<char>::const_iterator& m_position)
			{
				auto iTB = m_position + 1;

				int32_t v = 0;

				getName(m_fileData, iTB, m_name);
				getInt32(iTB, 1, v);

				if (v == 1)
				{
					m_data = true;
				}

				std::advance(m_position, std::distance(m_position + 1, iTB));

				return 0;
			}

			bool x08Token::getBoolValue(void) const noexcept
			{
				return m_data;
			}

			void x08Token::print(void) noexcept
			{
				std::cout << m_name << " => " << m_data << std::endl;
			}
		}
	}
}
 
