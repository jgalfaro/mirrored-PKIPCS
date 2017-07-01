/*
Télécom SudParis
2016, December
Pierre-Olivier Rocher
*/

#pragma once

#include <stonepine/architecture/architecture.h>

#ifdef __WINDOWS__
#ifdef UTILITIES_EXPORTS
#define UTILITIES_API __declspec(dllexport)
#else
#define UTILITIES_API __declspec(dllimport)
#endif
#else
#define UTILITIES_API
#endif

#include <memory>

namespace Stonepine
{
	namespace Utilities
	{
		class dumpCore final
		{

		private:

			class dumpCoreBase;

			std::unique_ptr<dumpCoreBase> m_dumpCoreBase;

		public:

			UTILITIES_API dumpCore(void) noexcept;
			UTILITIES_API virtual ~dumpCore(void);

		};
	}
}
