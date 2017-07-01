/*
Télécom SudParis
2015, October
Pierre-Olivier Rocher
*/

#ifdef _WIN32
#include <experimental/filesystem>
#endif

#include <openssl/err.h>

#include <stonepine/logger/logger.h>

#include "stonepine.utilities.iO.h"

namespace Stonepine
{
	namespace Utilities
	{
		FILE* iO::openFile(const std::string& fm_filePath, const std::string& fm_openingMode) noexcept
		{
			FILE* f = nullptr;

			try
			{
				if (fm_openingMode == "rb" || fm_openingMode == "wb")
				{
#ifdef _WIN32
					const auto r = fopen_s(&f, fm_filePath.c_str(), fm_openingMode.c_str());
#else
					f = fopen(fm_filePath.c_str(), fm_openingMode.c_str());
#endif
					if (f == nullptr)
					{
						throw std::runtime_error("something got wrong");
					}
				}
				else
				{
					throw std::runtime_error("unauthorized opening mode");
				}
			}
			catch (const std::exception& e)
			{
				Stonepine::LOG_ERROR("[utilities] unable to open file ", fm_filePath, " -> ", + e.what());
			}

			return f;
		}

		const std::string iO::readDataFromFile(const std::string& fm_dataFileFilePath) noexcept
		{
			std::ifstream iS;

			iS.exceptions(std::ifstream::failbit | std::ifstream::badbit);

			try
			{
				iS.open(fm_dataFileFilePath, std::ios::in);

				return readStreamIntoString(iS);

				/*auto data = static_cast<std::ostringstream&>(std::ostringstream{} << iS.rdbuf()).str();

				iS.close();

				return data;*/
			}
			catch (const std::ifstream::failure& e)
			{
				Stonepine::LOG_ERROR("[utilities] something got wrong while reading file \"" + fm_dataFileFilePath + "\" -> " + e.what());
			}

			return std::string();
		}

		const std::string iO::readBinaryDataFromFile(const std::string& fm_dataFileFilePath) noexcept
		{
			std::ifstream iS;

			iS.exceptions(std::ifstream::failbit | std::ifstream::badbit);

			try
			{
				iS.open(fm_dataFileFilePath.c_str(), std::ios::in | std::ios::binary);

				auto data = readStreamIntoString<char>(iS);
				
				iS.close();

				return data;
			}
			catch (const std::ifstream::failure& e)
			{
				Stonepine::LOG_ERROR("[utilities] something got wrong while reading file \"" + fm_dataFileFilePath + "\" -> " + e.what());
			}

			return std::string();
		}

		const bool iO::writeBinaryDataToFile(const std::string& fm_dataToWrite, const std::string& fm_dataFileFilePath) noexcept
		{
			if (fm_dataToWrite.empty() == false)
			{
				std::ofstream oS;

				oS.exceptions(std::ifstream::failbit | std::ifstream::badbit);

				try
				{
					oS.open(fm_dataFileFilePath.c_str(), std::ios::out | std::ios::binary | std::ofstream::trunc);

					oS.write(const_cast<char*>(&fm_dataToWrite.front()), fm_dataToWrite.size());

					oS.close();
				}
				catch (const std::ofstream::failure& e)
				{
					Stonepine::LOG_ERROR("[utilities] something got wrong while writing file \"" + fm_dataFileFilePath + "\" -> " + e.what());

					return false;
				}
			}

			return true;
		}

		const bool iO::writeBinaryDataToFile(const std::vector<char>& fm_dataToWrite, const std::string& fm_dataFileFilePath) noexcept
		{
			return writeBinaryDataToFile(std::make_shared<std::vector<char>>(fm_dataToWrite), fm_dataFileFilePath);
		}

		const bool iO::writeBinaryDataToFile(const std::shared_ptr<std::vector<char>> fm_dataToWrite, const std::string& fm_dataFileFilePath) noexcept
		{
			if (fm_dataToWrite != nullptr)
			{
				std::ofstream oS;

				oS.exceptions(std::ifstream::failbit | std::ifstream::badbit);

				try
				{
					oS.open(fm_dataFileFilePath.c_str(), std::ios::out | std::ios::binary | std::ofstream::trunc);

					oS.write((char*)&fm_dataToWrite->front(), fm_dataToWrite->size());

					oS.close();
				}
				catch (const std::ofstream::failure& e)
				{
					Stonepine::LOG_ERROR("[utilities] something got wrong while writing file \"" + fm_dataFileFilePath + "\" -> " + e.what());

					return false;
				}
			}

			return true;
		}

		const int iO::getFileSize(const std::string& f_filePath) noexcept
		{
#ifdef _WIN32
			try
			{
				return static_cast<int>(std::experimental::filesystem::file_size(f_filePath));
			}
			catch (const std::experimental::filesystem::filesystem_error& e)
			{
				Stonepine::LOG_ERROR("[utilities] something got wrong while reading file \"" + f_filePath + "\" -> " + e.what());
			}

			return -1;

#else
			std::ifstream iS;

			iS.exceptions(std::ifstream::failbit | std::ifstream::badbit);

			try
			{
				iS.open(f_filePath.c_str(), std::ios::binary | std::ios::ate);

				auto s = static_cast<int>(iS.tellg());

				iS.close();

				return s;
			}
			catch (std::ifstream::failure& e)
			{
				Stonepine::LOG_ERROR("something got wrong while reading file ", f_filePath, e.what());
			}

			return -1;
#endif
		}

		const uint64_t iO::getFileSize2(const std::string& f_fileFilePath) noexcept (false)
		{
#ifdef _WIN32
			try
			{
				const auto fP = std::experimental::filesystem::path(f_fileFilePath);

				return static_cast<uint64_t>(std::experimental::filesystem::file_size(fP));
			}
			catch (const std::experimental::filesystem::filesystem_error& e)
			{
				throw std::runtime_error("[utilities] something got wrong while reading file \"" + f_fileFilePath + "\" -> " + e.what());
			}

			return 0;

#else
			std::ifstream iS;

			iS.exceptions(std::ifstream::failbit | std::ifstream::badbit);

			try
			{
				iS.open(f_filePath.c_str(), std::ios::binary | std::ios::ate);

				auto s = static_cast<int>(iS.tellg());

				iS.close();

				return s;
			}
			catch (std::ifstream::failure& e)
			{
				Stonepine::LOG_ERROR("something got wrong while reading file ", f_filePath, e.what());
			}

			return -1;
#endif
		}
	}
}
