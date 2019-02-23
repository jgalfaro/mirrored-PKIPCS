/*
Télécom SudParis
2017, June
Pierre-Olivier Rocher
*/

#include "certificates.h"

namespace Stonepine
{
	namespace Utilities
	{
		certificates::certificates(void) noexcept
		{
			m_certificates = std::make_shared<std::vector<std::shared_ptr<certificate>>>();
		}

		certificates::~certificates(void)
		{

		}

		const void certificates::addAcertificate(const std::shared_ptr<certificate> fm_certificate) noexcept
		{
			m_certificates->emplace_back(fm_certificate);
		}

		const std::shared_ptr<std::vector<std::shared_ptr<certificate>>> certificates::getCertificates(void) const noexcept
		{
			return m_certificates;
		}
	}
}
 
