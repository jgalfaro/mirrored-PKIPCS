/*
2016, February
Pierre-Olivier Rocher
*/

#include "x0CToken.h"

namespace Stonepine
{
	namespace Parsers
	{
		namespace Bson
		{
			x0CToken::x0CToken(void) noexcept : baseToken(tokenType::x0C)
			{
				
			}

			x0CToken::~x0CToken(void)
			{

			}

			int x0CToken::parse(const std::vector<char>& m_fileData, std::vector<char>::const_iterator& m_position)
			{
				auto iTB = m_position + 1;

				getName(m_fileData, iTB, m_name);
				getString(m_fileData, iTB, m_data);

				m_data = std::string(iTB, iTB + 12);

				std::advance(m_position, std::distance(m_position + 1, iTB) + 12);

				return 0;
			}

			std::string x0CToken::getDbPointerValue(void) const noexcept
			{
				return m_data;
			}

			void x0CToken::print(void) noexcept
			{
				std::cout << m_name << " => " << m_data << std::endl;
			}
		}
	}
}
 
