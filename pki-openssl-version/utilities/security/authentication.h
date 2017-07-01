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
		class authentication
		{

		private:

			// User name and password
			std::string m_userName;
			std::string m_password;

			// Common
			std::string m_certificateAuthorityFileFilePath;

			// File base
			std::string m_certificateFileFilePath;
			std::string m_privateKeyFileFilePath;
			std::string m_privateKeyPassword;

			// Smart card base
			std::string m_pkcs11ModuleFileFilePath;
			uint16_t m_smartCardId;
			uint16_t m_smartCardCertificateId;
			std::string m_smartCardCertificateSubjectCommonName;
			std::string m_smartCardPinPassword;

		public:

			UTILITIES_API authentication(void) noexcept;
			UTILITIES_API virtual ~authentication(void);

			UTILITIES_API const void setUserName(const std::string& fm_userName) noexcept;
			UTILITIES_API const void setPassword(const std::string& fm_password) noexcept;

			UTILITIES_API const void setCertificateAuthorityFileFilePath(const std::string& fm_certificateAuthorityFileFilePath) noexcept;

			UTILITIES_API const void setCertificateFileFilePath(const std::string& fm_certificateFileFilePath) noexcept;
			UTILITIES_API const void setPrivateKeyFileFilePath(const std::string& fm_privateKeyFileFilePath) noexcept;
			UTILITIES_API const void setPrivateKeyPassword(const std::string& fm_privateKeyPassword) noexcept;

			UTILITIES_API const void setPkcs11ModuleFileFilePath(const std::string& fm_pkcs11ModulePath) noexcept;
			UTILITIES_API const void setSmartCardId(const uint16_t fm_smartCardId) noexcept;
			UTILITIES_API const void setSmartCardCertificateId(const uint16_t fm_smartCardCertificateId) noexcept;
			UTILITIES_API const void setSmartCardCertificateSubjectCommonName(const std::string& fm_smartCardCertificateSubjectCommonName) noexcept;
			UTILITIES_API const void setSmartCardPinPassword(const std::string& fm_smartCardPinPassword) noexcept;

			UTILITIES_API const std::string& getUserName(void) const noexcept;
			UTILITIES_API const std::string& getPassword(void) const noexcept;

			UTILITIES_API const std::string& getCertificateAuthorityFileFilePath(void) const noexcept;

			UTILITIES_API const std::string& getCertificateFileFilePath(void) const noexcept;
			UTILITIES_API const std::string& getPrivateKeyFileFilePath(void) const noexcept;
			UTILITIES_API const std::string& getPrivateKeyPassword(void) const noexcept;

			UTILITIES_API const std::string& getPkcs11ModuleFileFilePath(void) const noexcept;
			UTILITIES_API const uint16_t getSmartCardId(void) const noexcept;
			UTILITIES_API const uint16_t getSmartCardCertificateId(void) const noexcept;
			UTILITIES_API const std::string& getSmartCardCertificateSubjectCommonName(void) const noexcept;
			UTILITIES_API const std::string& getSmartCardPinPassword(void) const noexcept;

		};
	}
}
