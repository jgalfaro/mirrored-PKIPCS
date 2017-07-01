/*
2016, February
Pierre-Olivier Rocher
*/

#include "bsonParser.h"

namespace Stonepine
{
	namespace Parsers
	{
		std::vector<char> bson::m_fileData;

		bson::bson(void) noexcept
		{

		}

		std::shared_ptr<std::vector<std::shared_ptr<Stonepine::Parsers::Bson::baseToken>>> bson::parseOneDocument(const std::vector<char>::const_iterator fm_begin, const std::vector<char>::const_iterator fm_end)
		{
			using namespace Stonepine::Parsers::Bson;

			auto fileDataIterator = fm_begin;

			int inputChar = 0;
			int size = 0;

			std::shared_ptr<baseToken> token = nullptr;

			std::shared_ptr<std::vector<std::shared_ptr<baseToken>>> tokens = std::make_shared<std::vector<std::shared_ptr<baseToken>>>();

			while (fileDataIterator != fm_end)
			{
				if (fileDataIterator == fm_begin)
				{
					// Little endian
					size = *reinterpret_cast<const int32_t*>(&fileDataIterator[0]);

					std::advance(fileDataIterator, sizeof(int32_t));
				}

				inputChar = *fileDataIterator;
				token = nullptr;

				if (inputChar == static_cast<int>(tokenType::x00))
				{
					Stonepine::logger::debug("token x00 encountered");

					token = std::make_shared<x00Token>();
				}
				else if (inputChar == static_cast<int>(tokenType::x01))
				{
					Stonepine::logger::debug("token x01 encountered");

					token = std::make_shared<x01Token>();
				}
				else if (inputChar == static_cast<int>(tokenType::x02))
				{
					Stonepine::logger::debug("token x02 encountered");

					token = std::make_shared<x02Token>();
				}
				else if (inputChar == static_cast<int>(tokenType::x03))
				{
					Stonepine::logger::debug("token x03 encountered");

					token = std::make_shared<x03Token>();
				}
				else if (inputChar == static_cast<int>(tokenType::x04))
				{
					Stonepine::logger::debug("token x04 encountered");

					token = std::make_shared<x04Token>();
				}
				else if (inputChar == static_cast<int>(tokenType::x05))
				{
					Stonepine::logger::debug("token x05 encountered");

					token = std::make_shared<x05Token>();
				}
				else if (inputChar == static_cast<int>(tokenType::x06))
				{
					Stonepine::logger::debug("token x06 encountered");
					Stonepine::logger::warning("token x06 deprecated");

					token = std::make_shared<x06Token>();
				}
				else if (inputChar == static_cast<int>(tokenType::x07))
				{
					Stonepine::logger::debug("token x07 encountered");

					token = std::make_shared<x07Token>();
				}
				else if (inputChar == static_cast<int>(tokenType::x08))
				{
					Stonepine::logger::debug("token x08 encountered");

					token = std::make_shared<x08Token>();
				}
				else if (inputChar == static_cast<int>(tokenType::x09))
				{
					Stonepine::logger::debug("token x09 encountered");

					token = std::make_shared<x09Token>();
				}
				else if (inputChar == static_cast<int>(tokenType::x0A))
				{
					Stonepine::logger::debug("token x0A encountered");

					token = std::make_shared<x0AToken>();
				}
				else if (inputChar == static_cast<int>(tokenType::x0B))
				{
					Stonepine::logger::debug("token x0B encountered");

					token = std::make_shared<x0BToken>();
				}
				else if (inputChar == static_cast<int>(tokenType::x0C))
				{
					Stonepine::logger::debug("token x0C encountered");
					Stonepine::logger::warning("token x0C deprecated");

					token = std::make_shared<x0CToken>();
				}
				else if (inputChar == static_cast<int>(tokenType::x0D))
				{
					Stonepine::logger::debug("token x0D encountered");

					token = std::make_shared<x0DToken>();
				}
				else if (inputChar == static_cast<int>(tokenType::x0E))
				{
					Stonepine::logger::debug("token x0E encountered");
					Stonepine::logger::warning("token x0E deprecated");

					token = std::make_shared<x0EToken>();
				}
				else if (inputChar == static_cast<int>(tokenType::x0F))
				{
					Stonepine::logger::debug("token x0F encountered");

					token = std::make_shared<x0FToken>();
				}
				else if (inputChar == static_cast<int>(tokenType::x10))
				{
					Stonepine::logger::debug("token x10 encountered");

					token = std::make_shared<x10Token>();
				}
				else if (inputChar == static_cast<int>(tokenType::x11))
				{
					Stonepine::logger::debug("token x11 encountered");

					token = std::make_shared<x11Token>();
				}
				else if (inputChar == static_cast<int>(tokenType::x12))
				{
					Stonepine::logger::debug("token x12 encountered");

					token = std::make_shared<x12Token>();
				}
				else if (inputChar == static_cast<int>(tokenType::xFF))
				{
					Stonepine::logger::debug("token xFF encountered");

					token = std::make_shared<xFFToken>();
				}
				else if (inputChar == static_cast<int>(tokenType::x7F))
				{
					Stonepine::logger::debug("token x7F encountered");

					token = std::make_shared<x7FToken>();
				}
				else
				{
					Stonepine::logger::error("Unknow token encountered");

					// Return already correctly read key value pairs
					return tokens;
				}

				if (token != nullptr)
				{
					auto step = token->parse(m_fileData, fileDataIterator);

					if (step == 0)
					{
						tokens->push_back(token);

						std::advance(fileDataIterator, 1);
					}
					else if (step == -1)
					{
						return tokens;
					}
				}
			}

			return nullptr;
		}

		bson::bson(const std::vector<char>& fm_fileData) noexcept : bson()
		{
			setFileData(fm_fileData);
		}

		bson::~bson(void)
		{

		}

		void bson::parseData(void)
		{
			auto fileDataIterator = m_fileData.begin();

			do
			{
				// Little endian
				int size = *reinterpret_cast<const int32_t*>(&fileDataIterator[0]);

				auto i = std::distance(m_fileData.begin(), fileDataIterator);

				// Final begin and end iterators
				auto iTB = fileDataIterator;
				auto iTE = fileDataIterator;

				std::advance(iTE, size);

				std::advance(fileDataIterator, size - 1);

				int inputChar = *fileDataIterator;

				// Check if the BSON document is finishing by 0x00
				if (inputChar != static_cast<int>(Stonepine::Parsers::Bson::tokenType::x00))
				{
					Stonepine::logger::debug("BSON document at index " + std::to_string(i) + " is malformed");
				}
				else
				{
					Stonepine::logger::debug("Adding BSON document at index " + std::to_string(i) + ":" + std::to_string(i + size));

					m_documentsSizes.push_back(std::make_pair(iTB, iTE));
				}

				std::advance(fileDataIterator, 1);

#if __WITH_MULTI_DOCUMENTS_PER_FILE_SUPPORT__ == 0
				if (fileDataIterator != m_fileData.end())
				{
					Stonepine::logger::warning("the whole file wasn't read");
				}

				fileDataIterator = m_fileData.end();
#endif
			} while (fileDataIterator != m_fileData.end());


			for (auto iTDS = m_documentsSizes.begin(); iTDS != m_documentsSizes.end(); ++iTDS)
			{
				if (std::distance(iTDS->first, iTDS->second) == 5)
				{
					Stonepine::logger::warning("BSON document at index " + std::to_string(std::distance(m_documentsSizes.begin(), iTDS)) + " is empty");
				}

#if __WITH_MULTI_THREADING_SUPPORT__ == 1
				m_futureTokens.insert(std::make_pair(static_cast<int>(std::distance(m_documentsSizes.begin(), iTDS)), std::async(bson::parseOneDocument, iTDS->first, iTDS->second)));
#else
				m_tokens.insert(std::make_pair(static_cast<int>(std::distance(m_documentsSizes.begin(), iTDS)), bson::parseOneDocument(iTDS->first, iTDS->second)));
#endif
			}

#if __WITH_MULTI_THREADING_SUPPORT__ == 1
			for (auto &iTFT : m_futureTokens)
			{
				m_tokens.insert(std::make_pair(iTFT.first, iTFT.second.get()));
			}
#endif
		}

		void bson::print(void)
		{
			for (auto &iTT : m_tokens)
			{
				for (auto &jTBT : *iTT.second)
				{
					jTBT->print();
				}
			}
		}

		void bson::setFileData(const std::vector<char>& fm_fileData) noexcept
		{
			m_fileData = fm_fileData;
		}

		std::vector<char> bson::getFileData(void) noexcept
		{
			return m_fileData;
		}

		int bson::getBSONDocumentNumber(void)
		{
			return static_cast<int>(m_documentsSizes.size());
		}

		std::map<int, std::shared_ptr<std::vector<std::shared_ptr<Stonepine::Parsers::Bson::baseToken>>>> bson::getBSONDocuments(void)
		{
			return m_tokens;
		}
	}
}
