/*
Télécom SudParis
2016, October
Pierre-Olivier Rocher
*/

#pragma once

#if _WIN32
#ifdef UTILITIES_EXPORTS
#define UTILITIES_API __declspec(dllexport)
#else
#define UTILITIES_API __declspec(dllimport)
#endif
#else
#define UTILITIES_API
#endif

#define NOMINMAX

#include <memory>
#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <deque>
#include <limits>

namespace Stonepine
{
	namespace Utilities
	{
		class iO final
		{

		public:

			UTILITIES_API static FILE* openFile(const std::string& fm_filePath, const std::string& fm_openingMode) noexcept;

			// Read/write from disk
			UTILITIES_API static const std::string readDataFromFile(const std::string& fm_dataFileFilePath) noexcept;

			UTILITIES_API static const std::string readBinaryDataFromFile(const std::string& fm_dataFileFilePath) noexcept;

			UTILITIES_API static const bool writeBinaryDataToFile(const std::string& fm_dataToWrite, const std::string& fm_dataFileFilePath) noexcept;
			UTILITIES_API static const bool writeBinaryDataToFile(const std::vector<char>& fm_dataToWrite, const std::string& fm_dataFileFilePath) noexcept;
			UTILITIES_API static const bool writeBinaryDataToFile(const std::shared_ptr<std::vector<char>> fm_dataToWrite, const std::string& fm_dataFileFilePath) noexcept;

			UTILITIES_API static const int getFileSize(const std::string& f_filePath) noexcept;

			UTILITIES_API static const uint64_t getFileSize2(const std::string& f_fileFilePath) noexcept (false);

			template <typename T = char, typename U = std::char_traits<T>, typename A = std::allocator<T>>
			static std::string readStreamIntoString(std::basic_istream<T, U>& in, A alloc = {}) noexcept(false);

			template <typename C = std::string, typename T = char, typename U = std::char_traits<char>>
			static C readStreamIntoContainer(std::basic_istream<T, U>& in, typename C::allocator_type alloc = {}) noexcept(false);

			template <typename T, typename U = std::char_traits<T>, typename O = T, typename A = std::allocator<O> >
			static std::deque<O, A> readFileIntoDeque(std::basic_istream<T, U>& in, A alloc = {}) noexcept(false);

		};

		template<typename T, typename U, typename A>
		inline std::string iO::readStreamIntoString(std::basic_istream<T, U>& in, A alloc) noexcept(false)
		{
			std::basic_ostringstream<T, U, A> ss(std::basic_string<T, U, A>(std::move(alloc)));

			if (!(ss << in.rdbuf()))
			{
				throw std::ios_base::failure{ "error" };
			}

			return ss.str();
		}

		template<typename C, typename T, typename U>
		inline C iO::readStreamIntoContainer(std::basic_istream<T, U>& in, typename C::allocator_type alloc) noexcept(false)
		{
			static_assert(
				std::is_same<C, std::basic_string<T, U, typename C::allocator_type> >::value ||
				std::is_same<C, std::vector<T, typename C::allocator_type> >::value ||
				std::is_same<C, std::vector<std::make_unsigned<T>, typename C::allocator_type>>::value ||
				std::is_same<C, std::vector<std::make_signed<T>, typename C::allocator_type>>::value,
				"only std::string or std::vector of (unsigned) char allowed");

			auto const start_pos = in.tellg();

			if (std::streamsize(-1) == start_pos)
			{
				throw std::ios_base::failure{ "error" };
			}

			if (!in.ignore(std::numeric_limits<std::streamsize>::max()))
			{
				throw std::ios_base::failure{ "error" };
			}

			auto const char_count = in.gcount();

			if (!in.seekg(start_pos))
			{
				throw std::ios_base::failure{ "error" };
			}

			auto container = C(std::move(alloc));

			container.resize(char_count);

			if (0 != container.size())
			{
				if (!in.read(reinterpret_cast<T*>(&container[0]), container.size()))
				{
					throw std::ios_base::failure{ "error" };
				}
			}

			return container;
		}

		template<typename T, typename U, typename O, typename A>
		inline std::deque<O, A> iO::readFileIntoDeque(std::basic_istream<T, U>& in, A alloc) noexcept(false)
		{
			static_assert(std::is_same<T, O>::value ||
				std::is_same<std::make_unsigned<T>, O>::value ||
				std::is_same<std::make_signed<T>, O>::value,
				"char type of deque must be same as stream char type (possibly signed or unsigned)");

			auto const chunk_size = std::size_t{ BUFSIZ };

			auto container = std::deque<O, A>(std::move(alloc));

			auto chunk = std::array<O, chunk_size>{};

			while (in.read(reinterpret_cast<T*>(chunk.data()), chunk.size()) || in.gcount())
			{
				container.insert(end(container), begin(chunk), begin(chunk) + in.gcount());
			}

			return container;
		}
	}
}
