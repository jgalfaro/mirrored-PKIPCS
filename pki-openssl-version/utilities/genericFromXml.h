/*
Télécom SudParis
2016, December
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

#include <string>

namespace Stonepine
{
	namespace Utilities
	{
		class genericFromXml
		{

		private:

			// Configuration
			static bool m_isXmlValidationEnabled;

			std::string m_xmlFileFilePath;

		public:

			UTILITIES_API genericFromXml(void) noexcept;
			UTILITIES_API genericFromXml(const std::string& fm_xmlFileFilePath, const bool fm_isXmlValidationEnabled) noexcept;

			UTILITIES_API virtual ~genericFromXml(void);

			UTILITIES_API const std::string& getXmlFileFilePath(void) const noexcept;
			UTILITIES_API static const bool getXmlValidation(void) noexcept;

			UTILITIES_API const void setXmlFileFilePath(const std::string& fm_xmlFileFilePath) noexcept;
			UTILITIES_API const void setXmlValidation(const bool fm_isXmlValidationEnabled) noexcept;

		};
	}
}
