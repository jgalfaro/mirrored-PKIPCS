/*
2016, February
Pierre-Olivier Rocher
*/

#pragma once

#include <vector>
#include <iostream>
#include <string>
#include <memory>
#include <map>
#include <future>

#include "config.h"

#include "baseToken.h"

#include "x00Token.h"
#include "x01Token.h"
#include "x02Token.h"
#include "x03Token.h"
#include "x04Token.h"
#include "x05Token.h"
#include "x06Token.h"
#include "x07Token.h"
#include "x08Token.h"
#include "x09Token.h"
#include "x0AToken.h"
#include "x0BToken.h"
#include "x0CToken.h"
#include "x0DToken.h"
#include "x0EToken.h"
#include "x0FToken.h"
#include "x10Token.h"
#include "x11Token.h"
#include "x12Token.h"
#include "xFFToken.h"
#include "x7FToken.h"

namespace Stonepine
{
	namespace Parsers
	{
		class bson
		{

		private:

			// Read Data
			static std::vector<char> m_fileData;

			// Document(s) indexes in the read file
			std::vector<std::pair<std::vector<char>::const_iterator, std::vector<char>::const_iterator>> m_documentsSizes;

#if __WITH_MULTI_THREADING_SUPPORT__ == 1
			// Threading mamanegement
			std::map<int, std::future<std::shared_ptr<std::vector<std::shared_ptr<Stonepine::Parsers::Bson::baseToken>>>>> m_futureTokens;
#endif

			// Output data structure
			std::map<int, std::shared_ptr<std::vector<std::shared_ptr<Stonepine::Parsers::Bson::baseToken>>>> m_tokens;

		private:

			bson(void) noexcept;

			static std::shared_ptr<std::vector<std::shared_ptr<Stonepine::Parsers::Bson::baseToken>>> parseOneDocument(const std::vector<char>::const_iterator fm_begin, const std::vector<char>::const_iterator fm_end);

		public:

			bson(const std::vector<char>& fm_fileData) noexcept;
			~bson(void);

			void parseData(void);

			void print(void);

			void setFileData(const std::vector<char>& fm_fileData) noexcept;
			std::vector<char> getFileData(void) noexcept;

			int getBSONDocumentNumber(void);

			std::map<int, std::shared_ptr<std::vector<std::shared_ptr<Stonepine::Parsers::Bson::baseToken>>>> getBSONDocuments(void);

		};
	}
}
