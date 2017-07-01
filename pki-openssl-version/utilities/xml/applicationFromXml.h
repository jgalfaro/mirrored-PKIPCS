/*
Télécom SudParis
2017, June
Pierre-Olivier Rocher
*/

#pragma once

#include <stonepine/utilities/xml/utilities.xml.h>

#include <stonepine/utilities/application/application.h>

namespace Stonepine
{
	namespace Utilities
	{
		class applicationFromXml final : public application
		{

		private:

			class applicationFromXmlBase;

			std::unique_ptr<applicationFromXmlBase> m_applicationFromXmlBase;

		private:

			// Proper exit
			const void exitProperly(const bool fm_exitStatus) const noexcept;

		protected:

			UTILITIES_API applicationFromXml(void) noexcept;

			UTILITIES_API const void configureApplication(void) noexcept;

		public:

			UTILITIES_API applicationFromXml(const std::string& fm_xmlConfigurationFileFilePath, const bool fm_isXmlValidationEnabled = { true }) noexcept;
			UTILITIES_API virtual ~applicationFromXml(void);

		};
	}
}
