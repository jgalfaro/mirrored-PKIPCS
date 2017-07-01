/*
Télécom SudParis
2017, June
Pierre-Olivier Rocher
*/

#pragma once

#include <string>
#include <memory>
#include <vector>

#include <stonepine/utilities/xml/utilities.xml.h>

#include <stonepine/utilities/security/certificate.h>

namespace Stonepine
{
	namespace Utilities
	{
		class certificates final
		{

		private:

			std::shared_ptr<std::vector<std::shared_ptr<certificate>>> m_certificates;

		public:

			UTILITIESXML_API certificates(void) noexcept;
			UTILITIESXML_API ~certificates(void);

			UTILITIESXML_API const void addAcertificate(const std::shared_ptr<certificate> fm_certificate) noexcept;

			UTILITIESXML_API const std::shared_ptr<std::vector<std::shared_ptr<certificate>>> getCertificates(void) const noexcept;

		};
	}
}
