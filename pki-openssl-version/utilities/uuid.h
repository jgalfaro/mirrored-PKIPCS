/*
Télécom SudParis
2017, May
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

#include <string>
#include <memory>

namespace Stonepine
{
	namespace Utilities
	{
		class uuid final
		{

		private:

			class uuidBase;

			std::unique_ptr<uuidBase> m_uuidBase;

		public:

			UTILITIES_API uuid(void) noexcept;
			UTILITIES_API uuid(const std::string& fm_uuid) noexcept (false);

			UTILITIES_API ~uuid(void);

			// Move
			/*UTILITIES_API uuid(uuid&& fm_uuid) noexcept;
			UTILITIES_API uuid& operator=(uuid&& fm_uuid) noexcept;*/

			// Copy
			UTILITIES_API uuid(const uuid& fm_uuid);
			UTILITIES_API uuid& operator=(const uuid& fm_uuid);

			UTILITIES_API const void generate(void) const noexcept;

			UTILITIES_API const std::string getUuidAsString(void) const noexcept;

		};
	}
}
