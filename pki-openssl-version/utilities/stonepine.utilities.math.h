/*
Télécom SudParis
2017, January
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

#include <random>

namespace Stonepine
{
	namespace Utilities
	{
		class math final
		{

		public:

			template<typename T>
			static const T generateRandomNumber(const T fm_minimum, const T fm_maximum) noexcept;

			template<typename T>
			static const T generateRandomNumber(void) noexcept;

		};

		template<typename T>
		inline const T math::generateRandomNumber(const T fm_minimum, const T fm_maximum) noexcept
		{
			std::random_device rD;
			std::mt19937 mT(rD());
			std::uniform_int_distribution<T> uID(fm_minimum, fm_maximum); // guaranteed unbiased

			return uID(mT);
		}

		template<>
		inline const uint64_t math::generateRandomNumber(void) noexcept
		{
			return generateRandomNumber<uint64_t>(0, UINT64_MAX);
		}
	}
}
