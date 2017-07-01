/*
Télécom SudParis, Télécom Saint-Étienne
2017, May
Pierre-Olivier Rocher
*/

#pragma once

#ifdef _WIN32
#ifdef DATABASE_EXPORTS
#define DATABASE_API __declspec(dllexport)
#else
#define DATABASE_API __declspec(dllimport)
#endif
#else
#define DATABASE_API
#endif

#include <stdint.h>

namespace Stonepine
{
	namespace Database
	{
		enum class databaseType : uint8_t
		{
			DT_UNKNOW = 0,
			DT_POSTGRESQL = 1
		};

		enum class databaseSecureConnectionType : uint8_t
		{
			DSCT_UNKNOW = 0,
			DSCT_DISABLE = 1,
			DSCT_ALLOW = 2,
			DSCT_PREFER = 3,
			DSCT_REQUIRE = 4,
			DSCT_VERIFY_CA = 5,
			DSCT_VERIFY_FULL = 6
		};
	}
}
