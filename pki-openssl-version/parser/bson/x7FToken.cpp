/*
2016, February
Pierre-Olivier Rocher
*/

#include "x7FToken.h"

namespace Stonepine
{
	namespace Parsers
	{
		namespace Bson
		{
			x7FToken::x7FToken(void) noexcept : baseToken(tokenType::x7F)
			{

			}

			x7FToken::~x7FToken(void)
			{

			}

			int x7FToken::parse(const std::vector<char>& m_fileData, std::vector<char>::const_iterator& m_position)
			{
				auto iTB = m_position + 1;

				getName(m_fileData, iTB, m_name);

				std::advance(m_position, std::distance(m_position + 1, iTB));

				return 0;
			}

			void x7FToken::print(void) noexcept
			{

			}
		}
	}
}
 
