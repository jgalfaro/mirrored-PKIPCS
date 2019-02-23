/*
2016, February
Pierre-Olivier Rocher
*/

#include "x0FToken.h"

namespace Stonepine
{
	namespace Parsers
	{
		namespace Bson
		{
			x0FToken::x0FToken(void) noexcept : baseToken(tokenType::x0F)
			{
				m_size = 0;
			}

			x0FToken::~x0FToken(void)
			{

			}

			int x0FToken::parse(const std::vector<char>& m_fileData, std::vector<char>::const_iterator& m_position)
			{
				auto iTB = m_position + 1;

				getName(m_fileData, iTB, m_name);
				getInt32(iTB, m_size);
				getString(m_fileData, iTB, m_string);
				getDocument(m_fileData, iTB, m_subDocument);

				m_data = parseOneDocument(m_subDocument);

				std::advance(m_position, std::distance(m_position + 1, iTB));

				return 0;
			}

			std::string x0FToken::getStringValue(void) const noexcept
			{
				return m_string;
			}

			std::shared_ptr<std::vector<std::shared_ptr<baseToken>>> x0FToken::getDocumentValue(void) const noexcept
			{
				return m_data;
			}

			void x0FToken::print(void) noexcept
			{
				std::cout << m_name << " => " << m_string << std::endl;

				for (auto iTD : *m_data)
				{
					iTD->print();
				}
			}
		}
	}
}
 
