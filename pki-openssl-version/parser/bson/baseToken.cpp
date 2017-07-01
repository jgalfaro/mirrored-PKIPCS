/*
2016, February
Pierre-Olivier Rocher
*/

#include "baseToken.h"

#include "bsonParser.h"

namespace Stonepine
{
	namespace Parsers
	{
		namespace Bson
		{
			void baseToken::getName(const std::vector<char>& m_fileData, std::vector<char>::const_iterator& m_position, std::string& fm_name)
			{
				auto iTB = m_position;

				auto p = [&](const char fa_data)
				{
					int inputChar = fa_data;

					if (inputChar == static_cast<int>(tokenType::x00))
					{
						return true;
					}
					else
					{
						return false;
					}
				};

				auto iTE = std::find_if(iTB, m_fileData.end(), p);

				fm_name = std::string(iTB, iTE);

				std::advance(m_position, std::distance(m_position, iTE) + 1);
			}

			void baseToken::getString(const std::vector<char>& m_fileData, std::vector<char>::const_iterator& m_position, std::string& fm_string)
			{
				int32_t size;

				getInt32(m_position, size);

				fm_string = std::string(m_position, m_position + size);

				std::advance(m_position, size);
			}

			void baseToken::getCstring(const std::vector<char>& m_fileData, std::vector<char>::const_iterator& m_position, std::string& fm_cString)
			{
				return getName(m_fileData, m_position, fm_cString);
			}

			void baseToken::getDocument(const std::vector<char>& m_fileData, std::vector<char>::const_iterator& m_position, std::vector<char>& fm_document)
			{
				int32_t size = 0;

				auto iTB = m_position;

				getInt32(iTB, size);

				fm_document.resize(size);

				std::copy(iTB, iTB + size, fm_document.begin());

				std::advance(m_position, std::distance(m_position, iTB) + size + 1);
			}

			void baseToken::getBinary(const std::vector<char>& m_fileData, std::vector<char>::const_iterator& m_position, std::string& fm_binary)
			{
				int32_t size = 0;

				auto iTB = m_position;

				getInt32(iTB, size);

				int inputChar = *iTB;

				std::advance(iTB, 1);

				if (inputChar == static_cast<int>(tokenSubType::x00))
				{
					std::copy(iTB, iTB + size, &fm_binary);
				}
				else if (inputChar == static_cast<int>(tokenSubType::x01))
				{
					std::copy(iTB, iTB + size, &fm_binary);
				}
				else if (inputChar == static_cast<int>(tokenSubType::x02))
				{
					std::copy(iTB, iTB + size, &fm_binary);
				}
				else if (inputChar == static_cast<int>(tokenSubType::x03))
				{
					std::copy(iTB, iTB + size, &fm_binary);
				}
				else if (inputChar == static_cast<int>(tokenSubType::x04))
				{
					std::copy(iTB, iTB + size, &fm_binary);
				}
				else if (inputChar == static_cast<int>(tokenSubType::x05))
				{
					std::copy(iTB, iTB + size, &fm_binary);
				}
				else if (inputChar == static_cast<int>(tokenSubType::x80))
				{
					std::copy(iTB, iTB + size, &fm_binary);
				}

				std::advance(m_position, std::distance(m_position + 1, iTB) + size);
			}

			void baseToken::getInt32(std::vector<char>::const_iterator& m_position, int32_t& fm_value)
			{
				getInt32(m_position, sizeof(int32_t), fm_value);
			}

			void baseToken::getInt64(std::vector<char>::const_iterator& m_position, int64_t& fm_value)
			{
				// Little endian
				fm_value = *reinterpret_cast<const int64_t*>(&m_position[0]);

				std::advance(m_position, sizeof(int64_t));
			}

			void baseToken::getInt32(std::vector<char>::const_iterator& m_position, const uint32_t fm_typeSize, int32_t& fm_value)
			{
				// Little endian
				fm_value = *reinterpret_cast<const int32_t*>(&m_position[0]);

				std::advance(m_position, fm_typeSize);
			}

			std::shared_ptr<std::vector<std::shared_ptr<baseToken>>> baseToken::parseOneDocument(const std::vector<char>& m_fileData)
			{
				// Recursive call
				Stonepine::logger::debug("Recursive call");

				auto bP = std::make_unique<bson>(m_fileData);

				bP->parseData();

				// Only one document can be present
				return bP->getBSONDocuments()[0];
			}

			std::string baseToken::string2Hex(const std::string& input) const noexcept
			{
				// From https://stackoverflow.com/questions/3381614/c-convert-string-to-hexadecimal-and-vice-versa

				static const char* const lut = "0123456789ABCDEF";
				size_t len = input.length();

				std::string output;
				output.reserve(2 * len);

				for (size_t i = 0; i < len; ++i)
				{
					const unsigned char c = input[i];

					output.push_back(lut[c >> 4]);
					output.push_back(lut[c & 15]);
				}

				return output;
			}

			baseToken::baseToken(void) noexcept
			{

			}

			baseToken::baseToken(const tokenType& fm_token) noexcept : baseToken()
			{
				setToken(fm_token);
			}

			baseToken::~baseToken(void)
			{

			}

			std::string baseToken::getName(void) const noexcept
			{
				return m_name;
			}

			void baseToken::setToken(const tokenType& fm_token) noexcept
			{
				m_token = fm_token;
			}

			tokenType baseToken::getToken(void) const noexcept
			{
				return m_token;
			}
		}
	}
}
