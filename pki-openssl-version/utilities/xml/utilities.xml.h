/*
Télécom SudParis
2017, May
Pierre-Olivier Rocher
*/

#pragma once

#include <stonepine/architecture/architecture.h>

#ifdef __WINDOWS__
#ifdef UTILITIESXML_EXPORTS
#define UTILITIESXML_API __declspec(dllexport)
#else
#define UTILITIESXML_API __declspec(dllimport)
#endif
#else
#define UTILITIESXML_API
#endif
