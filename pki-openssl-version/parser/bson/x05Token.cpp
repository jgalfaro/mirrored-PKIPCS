/*
2016, February
Pierre-Olivier Rocher
*/

#include "x05Token.h"

namespace Stonepine
{
	namespace Parsers
	{
		namespace Bson
		{
			x05Token::x05Token(void) noexcept : baseToken(tokenType::x05)
			{

			}

			x05Token::~x05Token(void)
			{

			}

			int x05Token::parse(const std::vector<char>& m_fileData, std::vector<char>::const_iterator& m_position)
			{
				auto iTB = m_position + 1;

				getName(m_fileData, iTB, m_name);

				int32_t size;

				getInt32(iTB, size);

				int inputChar = *iTB;

				std::advance(iTB, 1);

				auto iTE = iTB;
				std::advance(iTE, size);

				if (inputChar == static_cast<int>(tokenSubType::x00))
				{
					Stonepine::logger::debug("subtoken x00 encountered");

					std::copy(iTB, iTE, &m_data);
				}
				else if (inputChar == static_cast<int>(tokenSubType::x01))
				{
					Stonepine::logger::debug("subtoken x01 encountered");

					std::copy(iTB, iTE, &m_data);
				}
				else if (inputChar == static_cast<int>(tokenSubType::x02))
				{
					Stonepine::logger::debug("subtoken x02 encountered");

					std::copy(iTB, iTE, &m_data);
				}
				else if (inputChar == static_cast<int>(tokenSubType::x03))
				{
					Stonepine::logger::debug("subtoken x03 encountered");

					std::copy(iTB, iTE, &m_data);
				}
				else if (inputChar == static_cast<int>(tokenSubType::x04))
				{
					Stonepine::logger::debug("subtoken x04 encountered");

					std::copy(iTB, iTE, &m_data);
				}
				else if (inputChar == static_cast<int>(tokenSubType::x05))
				{
					Stonepine::logger::debug("subtoken x05 encountered");

					std::copy(iTB, iTE, &m_data);
				}
				else if (inputChar == static_cast<int>(tokenSubType::x80))
				{
					Stonepine::logger::debug("subtoken x80 encountered");

					std::copy(iTB, iTE, &m_data);
				}

				std::advance(m_position, std::distance(m_position + 1, iTE));

				return 0;
			}

			std::string x05Token::getBinaryValue(void) const noexcept
			{
				return m_data;
			}

			void x05Token::print(void) noexcept
			{
				std::cout << m_name << " => " << m_data << std::endl;
			}
		}
	}
}
 
