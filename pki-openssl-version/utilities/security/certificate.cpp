/*
Télécom SudParis
2016, October
Pierre-Olivier Rocher
*/

#include <stonepine/logger/logger.h>

#include "certificate.h"

namespace Stonepine
{
	namespace Utilities
	{
		const void certificate::setSelfSignedCertificateAuthorityExtensions(void) const noexcept
		{
			m_extensions.insert(std::make_pair(certificateExtension::CE_NID_SUBJECT_KEY_IDENTIFIER, "hash"));
			m_extensions.insert(std::make_pair(certificateExtension::CE_NID_AUTHORITY_KEY_IDENTIFIER, "keyid:always,issuer"));
			m_extensions.insert(std::make_pair(certificateExtension::CE_NID_BASIC_CONSTRAINTS, "critical,CA:true"));
			m_extensions.insert(std::make_pair(certificateExtension::CE_NID_KEY_USAGE, "critical,digitalSignature,cRLSign,keyCertSign"));
		}

		const void certificate::setCertificateAuthorityExtensions(void) const noexcept
		{
			m_extensions.insert(std::make_pair(certificateExtension::CE_NID_SUBJECT_KEY_IDENTIFIER, "hash"));
			m_extensions.insert(std::make_pair(certificateExtension::CE_NID_AUTHORITY_KEY_IDENTIFIER, "keyid:always,issuer"));
			m_extensions.insert(std::make_pair(certificateExtension::CE_NID_BASIC_CONSTRAINTS, "critical,CA:true"));
			m_extensions.insert(std::make_pair(certificateExtension::CE_NID_KEY_USAGE, "critical,digitalSignature,cRLSign,keyCertSign"));
		}

		const void certificate::setIntermediateCertificateAuthorityExtensions(void) const noexcept
		{
			m_extensions.insert(std::make_pair(certificateExtension::CE_NID_SUBJECT_KEY_IDENTIFIER, "hash"));
			m_extensions.insert(std::make_pair(certificateExtension::CE_NID_AUTHORITY_KEY_IDENTIFIER, "keyid:always,issuer"));
			m_extensions.insert(std::make_pair(certificateExtension::CE_NID_BASIC_CONSTRAINTS, "critical,CA:true"));
			m_extensions.insert(std::make_pair(certificateExtension::CE_NID_KEY_USAGE, "critical,digitalSignature,cRLSign,keyCertSign"));
		}

		const void certificate::setServerCertificateExtensions(void) const noexcept
		{
			m_extensions.insert(std::make_pair(certificateExtension::CE_NID_SUBJECT_KEY_IDENTIFIER, "hash"));
			m_extensions.insert(std::make_pair(certificateExtension::CE_NID_AUTHORITY_KEY_IDENTIFIER, "keyid,issuer"));
			m_extensions.insert(std::make_pair(certificateExtension::CE_NID_BASIC_CONSTRAINTS, "CA:FALSE"));
			m_extensions.insert(std::make_pair(certificateExtension::CE_NID_KEY_USAGE, "critical,digitalSignature,keyEncipherment"));
			m_extensions.insert(std::make_pair(certificateExtension::CE_NID_EXT_KEY_USAGE, "serverAuth"));
			m_extensions.insert(std::make_pair(certificateExtension::CE_NID_NETSCAPE_CERT_TYPE, "server"));
		}

		const void certificate::setClientCertificateExtensions(void) const noexcept
		{
			m_extensions.insert(std::make_pair(certificateExtension::CE_NID_SUBJECT_KEY_IDENTIFIER, "hash"));
			m_extensions.insert(std::make_pair(certificateExtension::CE_NID_AUTHORITY_KEY_IDENTIFIER, "keyid,issuer"));
			m_extensions.insert(std::make_pair(certificateExtension::CE_NID_BASIC_CONSTRAINTS, "CA:FALSE"));
			m_extensions.insert(std::make_pair(certificateExtension::CE_NID_KEY_USAGE, "critical,nonRepudiation,digitalSignature,keyEncipherment"));
			m_extensions.insert(std::make_pair(certificateExtension::CE_NID_EXT_KEY_USAGE, "clientAuth,emailProtection"));
			m_extensions.insert(std::make_pair(certificateExtension::CE_NID_NETSCAPE_CERT_TYPE, "client,email"));
		}

		const void certificate::setOcspSigningCertificateExtensions(void) const noexcept
		{
			m_extensions.insert(std::make_pair(certificateExtension::CE_NID_BASIC_CONSTRAINTS, "CA:FALSE"));
			m_extensions.insert(std::make_pair(certificateExtension::CE_NID_KEY_USAGE, "nonRepudiation,digitalSignature,keyEncipherment"));
			m_extensions.insert(std::make_pair(certificateExtension::CE_NID_EXT_KEY_USAGE, "OCSPSigning"));
		}

		const void certificate::seTimeStampingCertificateExtensions(void) const noexcept
		{
			m_extensions.insert(std::make_pair(certificateExtension::CE_NID_SUBJECT_KEY_IDENTIFIER, "hash"));
			m_extensions.insert(std::make_pair(certificateExtension::CE_NID_KEY_USAGE, "critical,digitalSignature"));
			m_extensions.insert(std::make_pair(certificateExtension::CE_NID_EXT_KEY_USAGE, "critical,timeStamping"));
		}

		certificate::certificate(void) noexcept
		{

		}

		certificate::~certificate(void)
		{

		}

		const certificate certificate::clone(void) const noexcept
		{
			return *this;
		}

		const void certificate::setIssuerCountryName(const std::string& fm_issuerCountryName) noexcept
		{
			m_issuerCountryName = fm_issuerCountryName;
		}

		const void certificate::setIssuerStateOrProvinceName(const std::string& fm_issuerStateOrProvinceName) noexcept
		{
			m_issuerStateOrProvinceName = fm_issuerStateOrProvinceName;
		}

		const void certificate::setIssuerLocalityName(const std::string& fm_issuerLocalityName) noexcept
		{
			m_issuerLocalityName = fm_issuerLocalityName;
		}

		const void certificate::setIssuerOrganizationName(const std::string& fm_issuerOrganizationName) noexcept
		{
			m_issuerOrganizationName = fm_issuerOrganizationName;
		}

		const void certificate::setIssuerOrganizationalUnitName(const std::string& fm_issuerOrganizationalUnitName) noexcept
		{
			m_issuerOrganizationalUnitName = fm_issuerOrganizationalUnitName;
		}

		const void certificate::setIssuerCommonName(const std::string& fm_issuerCommonName) noexcept
		{
			m_issuerCommonName = fm_issuerCommonName;
		}

		const void certificate::setIssuerEmailAddress(const std::string& fm_issuerEmailAddress) noexcept
		{
			m_issuerEmailAddress = fm_issuerEmailAddress;
		}

		const void certificate::setSerialNumber(const uint64_t setSerialNumber) noexcept
		{
			m_serialNumber = setSerialNumber;
		}

		const void certificate::setSubjectCountryName(const std::string& fm_subjectCountryName) noexcept
		{
			m_subjectCountryName = fm_subjectCountryName;
		}

		const void certificate::setSubjectStateOrProvinceName(const std::string& fm_subjectStateOrProvinceName) noexcept
		{
			m_subjectStateOrProvinceName = fm_subjectStateOrProvinceName;
		}

		const void certificate::setSubjectLocalityName(const std::string& fm_subjectLocalityName) noexcept
		{
			m_subjectLocalityName = fm_subjectLocalityName;
		}

		const void certificate::setSubjectOrganizationName(const std::string& fm_subjectOrganizationName) noexcept
		{
			m_subjectOrganizationName = fm_subjectOrganizationName;
		}

		const void certificate::setSubjectOrganizationalUnitName(const std::string& fm_subjectOrganizationalUnitName) noexcept
		{
			m_subjectOrganizationalUnitName = fm_subjectOrganizationalUnitName;
		}

		const void certificate::setSubjectCommonName(const std::string& fm_subjectCommonName) noexcept
		{
			m_subjectCommonName = fm_subjectCommonName;
		}

		const void certificate::setSubjectEmailAddress(const std::string& fm_subjectEmailAddress) noexcept
		{
			m_subjectEmailAddress = fm_subjectEmailAddress;
		}

		const void certificate::setBeginValidityDate(const std::time_t& fm_beginValidityDate) noexcept
		{
			m_beginValidityDate = fm_beginValidityDate;
		}

		const void certificate::setEndValidityDate(const std::time_t& fm_endValidityDate) noexcept
		{
			m_endValidityDate = fm_endValidityDate;
		}

		const void certificate::setPrivateKeyPassword(const std::string& fm_privateKeyPassword) noexcept
		{
			m_privateKeyPassword = fm_privateKeyPassword;
		}

		const void certificate::setCertificateType(const certificateType& fm_certificateType) noexcept
		{
			m_certificateType = fm_certificateType;

			if (m_certificateType == certificateType::CT_X509_CA)
			{
				setCertificateAuthorityExtensions();
			}
			else if (m_certificateType == certificateType::CT_X509_CLIENT)
			{
				setClientCertificateExtensions();
			}
			else if (m_certificateType == certificateType::CT_X509_INTERMEDIATE_CA)
			{
				setIntermediateCertificateAuthorityExtensions();
			}
			else if (m_certificateType == certificateType::CT_X509_OCSP_SIGNING)
			{
				setOcspSigningCertificateExtensions();
			}
			else if (m_certificateType == certificateType::CT_X509_SELF_SIGNED_CA)
			{
				setSelfSignedCertificateAuthorityExtensions();
			}
			else if (m_certificateType == certificateType::CT_X509_SERVER)
			{
				setServerCertificateExtensions();
			}
			else if (m_certificateType == certificateType::CT_X509_UNKNOW)
			{
				LOG_WARNING("[utilities] unknow enumeration value");
			}
		}

		const void certificate::setPrivateKeyType(const privateKeyType& fm_privateKeyType) noexcept
		{
			m_privateKeyType = fm_privateKeyType;
		}

		const void certificate::setExtensions(const std::map<certificateExtension, std::string>& fm_extensions) noexcept
		{
			m_extensions = fm_extensions;
		}

		std::map<certificateExtension, std::string>& certificate::setExtensions(void) noexcept
		{
			return m_extensions;
		}

		const std::string& certificate::getIssuerCountryName(void) const noexcept
		{
			return m_issuerCountryName;
		}

		const std::string& certificate::getIssuerStateOrProvinceName(void) const noexcept
		{
			return m_issuerStateOrProvinceName;
		}

		const std::string& certificate::getIssuerLocalityName(void) const noexcept
		{
			return m_issuerLocalityName;
		}

		const std::string& certificate::getIssuerOrganizationName(void) const noexcept
		{
			return m_issuerOrganizationName;
		}

		const std::string& certificate::getIssuerOrganizationalUnitName(void) const noexcept
		{
			return m_issuerOrganizationalUnitName;
		}

		const std::string& certificate::getIssuerCommonName(void) const noexcept
		{
			return m_issuerCommonName;
		}

		const std::string& certificate::getIssuerEmailAddress(void) const noexcept
		{
			return m_issuerEmailAddress;
		}

		const uint64_t certificate::getSerialNumber(void) const noexcept
		{
			return m_serialNumber;
		}

		const std::string& certificate::getSubjectCountryName(void) const noexcept
		{
			return m_subjectCountryName;
		}

		const std::string& certificate::getSubjectStateOrProvinceName(void) const noexcept
		{
			return m_subjectStateOrProvinceName;
		}

		const std::string& certificate::getSubjectLocalityName(void) const noexcept
		{
			return m_subjectLocalityName;
		}

		const std::string& certificate::getSubjectOrganizationName(void) const noexcept
		{
			return m_subjectOrganizationName;
		}

		const std::string& certificate::getSubjectOrganizationalUnitName(void) const noexcept
		{
			return m_subjectOrganizationalUnitName;
		}

		const std::string& certificate::getSubjectCommonName(void) const noexcept
		{
			return m_subjectCommonName;
		}

		const std::string& certificate::getSubjectEmailAddress(void) const noexcept
		{
			return m_subjectEmailAddress;
		}

		const std::time_t& certificate::getBeginValidityDate(void) const noexcept
		{
			return m_beginValidityDate;
		}

		const std::time_t& certificate::getEndValidityDate(void) const noexcept
		{
			return m_endValidityDate;
		}

		const std::string& certificate::getPrivateKeyPassword(void) const noexcept
		{
			return m_privateKeyPassword;
		}

		const certificateType& certificate::getCertificateType(void) const noexcept
		{
			return m_certificateType;
		}

		const privateKeyType& certificate::getPrivateKeyType(void) const noexcept
		{
			return m_privateKeyType;
		}

		const std::map<certificateExtension, std::string>& certificate::getExtensions(void) const noexcept
		{
			return m_extensions;
		}
	}
}
