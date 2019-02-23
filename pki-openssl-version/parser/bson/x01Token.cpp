/*
2016, February
Pierre-Olivier Rocher
*/

#include "x01Token.h"

namespace Stonepine
{
	namespace Parsers
	{
		namespace Bson
		{
			x01Token::x01Token(void) noexcept : baseToken(tokenType::x01)
			{
				
			}

			x01Token::~x01Token(void)
			{

			}

			int x01Token::parse(const std::vector<char>& m_fileData, std::vector<char>::const_iterator& m_position)
			{
				auto iTB = m_position + 1;

				getName(m_fileData, iTB, m_name);

				// Little endian
				m_data = *reinterpret_cast<const double*>(&iTB[0]);

				std::advance(iTB, sizeof(double));

				std::advance(m_position, std::distance(m_position + 1, iTB));

				return 0;
			}

			void x01Token::print(void)
			{
				std::cout << m_name << " => " << m_data << std::endl;
			}

			double x01Token::getDoubleValue(void) const noexcept
			{
				return m_data;
			}
		}
	}
}
 
