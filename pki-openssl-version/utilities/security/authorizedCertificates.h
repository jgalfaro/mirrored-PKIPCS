/*
Télécom SudParis
2017, May
Pierre-Olivier Rocher
*/

#pragma once

#include <string>
#include <memory>
#include <vector>

#include <stonepine/utilities/utilities.h>

namespace Stonepine
{
	namespace Utilities
	{
		class authorizedCertificates
		{

		protected:

			std::shared_ptr<std::vector<std::string>> m_subjectCommonNames;

		protected:

			UTILITIES_API const void setSubjectCommonNames(const std::shared_ptr<std::vector<std::string>> fm_subjectCommonNames) noexcept (false);

			UTILITIES_API const void setInstance(const authorizedCertificates& fm_authorizedCertificates) noexcept;

		public:

			UTILITIES_API authorizedCertificates(void) noexcept;
			UTILITIES_API authorizedCertificates(const std::shared_ptr<std::vector<std::string>> fm_subjectCommonNames) noexcept (false);

			UTILITIES_API ~authorizedCertificates(void);

			UTILITIES_API const void addASubjectCommonName(const std::string& fm_peerSubjectCommonName) noexcept;

			UTILITIES_API const std::shared_ptr<std::vector<std::string>> getSubjectCommonNames(void) noexcept (false);

			UTILITIES_API const bool isSubjectCommonNameAuthorized(const std::string& fm_peerSubjectCommonName) const noexcept;

		};
	}
}
