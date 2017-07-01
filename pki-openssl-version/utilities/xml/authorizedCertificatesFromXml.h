/*
Télécom SudParis
2017, May
Pierre-Olivier Rocher
*/

#pragma once

#include <stonepine/utilities/xml/utilities.xml.h>

#include <stonepine/utilities/security/authorizedCertificates.h>

namespace Stonepine
{
	namespace Utilities
	{
		class authorizedCertificatesFromXml final : public authorizedCertificates
		{

		private:

			class authorizedCertificatesFromXmlBase;

			std::unique_ptr<authorizedCertificatesFromXmlBase> m_authorizedCertificatesFromXmlBase;

			const void configureAuthorizedCertificates(void) noexcept;

			// Proper exit
			const void exitProperly(const bool fm_exitStatus) const noexcept;

		public:

			UTILITIESXML_API authorizedCertificatesFromXml(void) noexcept;
			UTILITIESXML_API authorizedCertificatesFromXml(const std::string& fm_xmlConfigurationFileFilePath, const bool fm_isXmlValidationEnabled = { true }) noexcept (false);

			UTILITIESXML_API ~authorizedCertificatesFromXml(void);

			UTILITIESXML_API const std::string getXmlConfigurationFileFilePath(void) const noexcept;
		};
	}
}
