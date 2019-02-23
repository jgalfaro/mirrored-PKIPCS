/*
2016, February
Pierre-Olivier Rocher
*/

#include "x0AToken.h"

namespace Stonepine
{
	namespace Parsers
	{
		namespace Bson
		{
			x0AToken::x0AToken(void) noexcept : baseToken(tokenType::x0A)
			{
				
			}

			x0AToken::~x0AToken(void)
			{

			}

			int x0AToken::parse(const std::vector<char>& m_fileData, std::vector<char>::const_iterator& m_position)
			{
				auto iTB = m_position + 1;

				getName(m_fileData, iTB, m_name);

				std::advance(m_position, std::distance(m_position + 1, iTB));

				return 0;
			}

			void x0AToken::print(void) noexcept
			{
				std::cout << m_name << " => NULL" << std::endl;
			}
		}
	}
}
 
