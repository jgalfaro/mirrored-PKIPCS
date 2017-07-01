/*
2016, February
Pierre-Olivier Rocher
*/

#pragma once

#include <vector>
#include <algorithm>
#include <iostream>
#include <string>
#include <memory>
#include <ctime>
#include <sstream>
#include <bitset>
#include <mutex>

#include "config.h"
#include "logger.h"

namespace Stonepine
{
	namespace Parsers
	{
		namespace Bson
		{
			enum class tokenType : int { x00 = 0, x01 = 1, x02 = 2, x03 = 3, x04 = 4, x05 = 5, x06 = 6, x07 = 7, x08 = 8, x09 = 9, x0A = 10, x0B = 11, x0C = 12, x0D = 13, x0E = 14, x0F = 15, x10 = 16, x11 = 17, x12 = 18, xFF = 255, x7F = 127 };
			enum class tokenSubType : int { x00 = 0, x01 = 1, x02 = 2, x03 = 3, x04 = 4, x05 = 5, x80 = 80 };

			class baseToken
			{

			private:

				tokenType m_token;

			protected:

				std::string m_name;

			protected:

				// Parsing utilities
				void getName(const std::vector<char>& m_fileData, std::vector<char>::const_iterator& m_position, std::string& fm_name);
				void getString(const std::vector<char>& m_fileData, std::vector<char>::const_iterator& m_position, std::string& fm_string);
				void getCstring(const std::vector<char>& m_fileData, std::vector<char>::const_iterator& m_position, std::string& fm_cString);
				void getDocument(const std::vector<char>& m_fileData, std::vector<char>::const_iterator& m_position, std::vector<char>& fm_document);
				void getBinary(const std::vector<char>& m_fileData, std::vector<char>::const_iterator& m_position, std::string& fm_binary);

				void getInt32(std::vector<char>::const_iterator& m_position, int32_t& fm_value);
				void getInt64(std::vector<char>::const_iterator& m_position, int64_t& fm_value);

				void getInt32(std::vector<char>::const_iterator& m_position, const uint32_t fm_typeSize, int32_t& fm_value);

				// Recursive parsing
				std::shared_ptr<std::vector<std::shared_ptr<baseToken>>> parseOneDocument(const std::vector<char>& m_fileData);

				// Other utilities
				std::string string2Hex(const std::string& input) const noexcept;

			private:

				baseToken(void) noexcept;

			public:

				baseToken(const tokenType& fm_token) noexcept;
				virtual ~baseToken(void);

				virtual int parse(const std::vector<char>& m_fileData, std::vector<char>::const_iterator& m_position) = 0;
				virtual void print(void) = 0;

				virtual std::string getName(void) const noexcept;

				void setToken(const tokenType& fm_token) noexcept;
				tokenType getToken(void) const noexcept;

			};
		}
	}
}
