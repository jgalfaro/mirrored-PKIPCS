/*
2016, February
Pierre-Olivier Rocher
*/

#include "x0BToken.h"

namespace Stonepine
{
	namespace Parsers
	{
		namespace Bson
		{
			x0BToken::x0BToken(void) noexcept : baseToken(tokenType::x0B)
			{
				
			}

			x0BToken::~x0BToken(void)
			{

			}

			int x0BToken::parse(const std::vector<char>& m_fileData, std::vector<char>::const_iterator& m_position)
			{
				auto iTB = m_position + 1;

				getName(m_fileData, iTB, m_name);
				getCstring(m_fileData, iTB, m_regexPattern);
				getCstring(m_fileData, iTB, m_regexOptions);

				std::advance(m_position, std::distance(m_position + 1, iTB));

				return 0;
			}

			std::string x0BToken::getRegexPatternValue(void) const noexcept
			{
				return m_regexPattern;
			}

			std::string x0BToken::getRegexOptionsValue(void) const noexcept
			{
				return m_regexOptions;
			}

			void x0BToken::print(void) noexcept
			{
				std::cout << m_name << " => " << m_regexPattern << ":" << m_regexOptions << std::endl;
			}
		}
	}
}
