/*
Télécom SudParis
2017, May
Pierre-Olivier Rocher
*/

#include "authentication.h"

namespace Stonepine
{
	namespace Utilities
	{
		const void authentication::setUserName(const std::string& fm_userName) noexcept
		{
			m_userName = fm_userName;
		}

		const void authentication::setPassword(const std::string& fm_password) noexcept
		{
			m_password = fm_password;
		}

		const void authentication::setCertificateAuthorityFileFilePath(const std::string& fm_certificateAuthorityFileFilePath) noexcept
		{
			m_certificateAuthorityFileFilePath = fm_certificateAuthorityFileFilePath;
		}

		const void authentication::setCertificateFileFilePath(const std::string& fm_certificateFileFilePath) noexcept
		{
			m_certificateFileFilePath = fm_certificateFileFilePath;
		}

		const void authentication::setPrivateKeyFileFilePath(const std::string& fm_privateKeyFileFilePath) noexcept
		{
			m_privateKeyFileFilePath = fm_privateKeyFileFilePath;
		}

		const void authentication::setPrivateKeyPassword(const std::string& fm_privateKeyPassword) noexcept
		{
			m_privateKeyPassword = fm_privateKeyPassword;
		}

		const void authentication::setPkcs11ModuleFileFilePath(const std::string& fm_pkcs11ModulePath) noexcept
		{
			m_pkcs11ModuleFileFilePath = fm_pkcs11ModulePath;
		}

		const void authentication::setSmartCardId(const uint16_t fm_smartCardId) noexcept
		{
			m_smartCardId = fm_smartCardId;
		}

		const void authentication::setSmartCardCertificateId(const uint16_t fm_smartCardCertificateId) noexcept
		{
			m_smartCardCertificateId = fm_smartCardCertificateId;
		}

		const void authentication::setSmartCardCertificateSubjectCommonName(const std::string& fm_smartCardCertificateSubjectCommonName) noexcept
		{
			m_smartCardCertificateSubjectCommonName = fm_smartCardCertificateSubjectCommonName;
		}

		const void authentication::setSmartCardPinPassword(const std::string& fm_smartCardPinPassword) noexcept
		{
			m_smartCardPinPassword = fm_smartCardPinPassword;
		}

		const std::string& authentication::getUserName(void) const noexcept
		{
			return m_userName;
		}

		const std::string& authentication::getPassword(void) const noexcept
		{
			return m_password;
		}

		const std::string& authentication::getCertificateAuthorityFileFilePath(void) const noexcept
		{
			return m_certificateAuthorityFileFilePath;
		}

		const std::string& authentication::getCertificateFileFilePath(void) const noexcept
		{
			return m_certificateFileFilePath;
		}

		const std::string& authentication::getPrivateKeyFileFilePath(void) const noexcept
		{
			return m_privateKeyFileFilePath;
		}

		const std::string& authentication::getPrivateKeyPassword(void) const noexcept
		{
			return m_privateKeyPassword;
		}

		const std::string& authentication::getPkcs11ModuleFileFilePath(void) const noexcept
		{
			return m_pkcs11ModuleFileFilePath;
		}

		const uint16_t authentication::getSmartCardId(void) const noexcept
		{
			return m_smartCardId;
		}

		const uint16_t authentication::getSmartCardCertificateId(void) const noexcept
		{
			return m_smartCardCertificateId;
		}

		const std::string& authentication::getSmartCardCertificateSubjectCommonName(void) const noexcept
		{
			return m_smartCardCertificateSubjectCommonName;
		}

		const std::string& authentication::getSmartCardPinPassword(void) const noexcept
		{
			return m_smartCardPinPassword;
		}

		authentication::authentication(void) noexcept
		{

		}

		authentication::~authentication(void)
		{

		}
	}
}
