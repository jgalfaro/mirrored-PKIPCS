/*
2016, February
Pierre-Olivier Rocher
*/

#include "x0EToken.h"

namespace Stonepine
{
	namespace Parsers
	{
		namespace Bson
		{
			x0EToken::x0EToken(void) noexcept : baseToken(tokenType::x0E)
			{
				
			}

			x0EToken::~x0EToken(void)
			{

			}

			int x0EToken::parse(const std::vector<char>& m_fileData, std::vector<char>::const_iterator& m_position)
			{
				auto iTB = m_position + 1;

				getName(m_fileData, iTB, m_name);
				getString(m_fileData, iTB, m_data);

				std::advance(m_position, std::distance(m_position + 1, iTB));

				return 0;
			}

			std::string x0EToken::getStringValue(void) const noexcept
			{
				return m_data;
			}

			void x0EToken::print(void) noexcept
			{
				std::cout << m_name << " => " << m_data << std::endl;
			}
		}
	}
}
