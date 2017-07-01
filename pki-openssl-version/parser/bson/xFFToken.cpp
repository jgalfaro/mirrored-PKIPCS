/*
2016, February
Pierre-Olivier Rocher
*/

#include "xFFToken.h"

namespace Stonepine
{
	namespace Parsers
	{
		namespace Bson
		{
			xFFToken::xFFToken(void) noexcept : baseToken(tokenType::xFF)
			{

			}

			xFFToken::~xFFToken(void)
			{

			}

			int xFFToken::parse(const std::vector<char>& m_fileData, std::vector<char>::const_iterator& m_position)
			{
				auto iTB = m_position + 1;

				getName(m_fileData, iTB, m_name);

				std::advance(m_position, std::distance(m_position + 1, iTB));

				return 0;
			}

			void xFFToken::print(void) noexcept
			{
				std::cout << m_name << std::endl;
			}
		}
	}
}
