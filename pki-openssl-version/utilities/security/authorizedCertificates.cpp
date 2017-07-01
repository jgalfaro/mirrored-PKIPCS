/*
Télécom SudParis
2017, May
Pierre-Olivier Rocher
*/

#include <stonepine/logger/logger.h>

#include "authorizedCertificates.h"

namespace Stonepine
{
	namespace Utilities
	{
		const void authorizedCertificates::setSubjectCommonNames(const std::shared_ptr<std::vector<std::string>> fm_authorizedCertificateCommonNames) noexcept (false)
		{
			m_subjectCommonNames = fm_authorizedCertificateCommonNames;
		}

		const void authorizedCertificates::setInstance(const authorizedCertificates& fm_authorizedCertificates) noexcept
		{
			*this = fm_authorizedCertificates;
		}

		authorizedCertificates::authorizedCertificates(void) noexcept
		{
			m_subjectCommonNames = std::make_shared<std::vector<std::string>>();
		}

		authorizedCertificates::authorizedCertificates(const std::shared_ptr<std::vector<std::string>> fm_authorizedCertificateCommonNames) noexcept (false)
		{
			setSubjectCommonNames(fm_authorizedCertificateCommonNames);
		}

		const std::shared_ptr<std::vector<std::string>> authorizedCertificates::getSubjectCommonNames(void) noexcept (false)
		{
			return m_subjectCommonNames;
		}

		const bool authorizedCertificates::isSubjectCommonNameAuthorized(const std::string& fm_peerSubjectCommonName) const noexcept
		{
			auto r1 = std::find(m_subjectCommonNames->begin(), m_subjectCommonNames->end(), fm_peerSubjectCommonName);

			if (r1 != m_subjectCommonNames->end())
			{
				return true;
			}
			else
			{
				return false;
			}
		}

		authorizedCertificates::~authorizedCertificates(void)
		{

		}

		const void authorizedCertificates::addASubjectCommonName(const std::string& fm_peerSubjectCommonName) noexcept
		{
			m_subjectCommonNames->emplace_back(fm_peerSubjectCommonName);
		}
	}
}
