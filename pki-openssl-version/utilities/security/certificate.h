/*
Télécom SudParis
2016, October
Pierre-Olivier Rocher
*/

#pragma once

#include <stonepine/architecture/architecture.h>

#ifdef __WINDOWS__
#ifdef UTILITIES_EXPORTS
#define UTILITIES_API __declspec(dllexport)
#else
#define UTILITIES_API __declspec(dllimport)
#endif
#else
#define UTILITIES_API
#endif

#include <map>
#include <string>
#include <ctime>

namespace Stonepine
{
	namespace Utilities
	{
		enum class signatureMethod : uint8_t
		{
			SM_UNKNOW = 0,
			SM_SHA1 = 1,
			SM_SHA224 = 2,
			SM_SHA256 = 3,
			SM_SHA384 = 4,
			SM_SHA512 = 5
		};

		enum class privateKeyType : uint8_t
		{
			PKT_UNKNOW = 0,
			PKT_RSA_2048 = 1,
			PKT_RSA_4096 = 2
		};

		enum class certificateType : uint8_t
		{
			CT_X509_UNKNOW = 0,
			CT_X509_SELF_SIGNED_CA = 1,
			CT_X509_CA = 2,
			CT_X509_INTERMEDIATE_CA = 3,
			CT_X509_SERVER = 4,
			CT_X509_CLIENT = 5,
			CT_X509_REQUEST = 6,
			CT_X509_REVOCATION_LIST = 7,
			CT_X509_OCSP_SIGNING = 8,
			CT_X509_TIMESTAMPING = 9
		};

		enum class certificateExtension : uint8_t
		{
			CE_NID_NETSCAPE_CERT_TYPE = 71,
			CE_NID_SUBJECT_KEY_IDENTIFIER = 82,
			CE_NID_KEY_USAGE = 83,
			CE_NID_BASIC_CONSTRAINTS = 87,
			CE_NID_AUTHORITY_KEY_IDENTIFIER = 90,
			CE_NID_CRL_DISTRIBUTION_POINTS = 103,
			CE_NID_EXT_KEY_USAGE = 126,
			CE_NID_INFO_ACCESS = 177
		};

		class certificate
		{

		private:

			// Issuer 
			std::string m_issuerCountryName;
			std::string m_issuerStateOrProvinceName;
			std::string m_issuerLocalityName;
			std::string m_issuerOrganizationName;
			std::string m_issuerOrganizationalUnitName;
			std::string m_issuerCommonName;
			std::string m_issuerEmailAddress;

			// Subject
			uint64_t m_serialNumber;

			std::string m_subjectCountryName;
			std::string m_subjectStateOrProvinceName;
			std::string m_subjectLocalityName;
			std::string m_subjectOrganizationName;
			std::string m_subjectOrganizationalUnitName;
			std::string m_subjectCommonName;
			std::string m_subjectEmailAddress;

			std::time_t m_beginValidityDate;
			std::time_t m_endValidityDate;

			std::string m_privateKeyPassword;

			certificateType m_certificateType;
			privateKeyType m_privateKeyType;

			mutable std::map<certificateExtension, std::string> m_extensions;

		private:

			const void setSelfSignedCertificateAuthorityExtensions(void) const noexcept;
			const void setCertificateAuthorityExtensions(void) const noexcept;
			const void setIntermediateCertificateAuthorityExtensions(void) const noexcept;
			const void setServerCertificateExtensions(void) const noexcept;
			const void setClientCertificateExtensions(void) const noexcept;
			const void setOcspSigningCertificateExtensions(void) const noexcept;
			const void seTimeStampingCertificateExtensions(void) const noexcept;

		public:

			UTILITIES_API certificate(void) noexcept;
			UTILITIES_API ~certificate(void);

			UTILITIES_API const certificate clone(void) const noexcept;

			UTILITIES_API const void setIssuerCountryName(const std::string& fm_issuerCountryName) noexcept;
			UTILITIES_API const void setIssuerStateOrProvinceName(const std::string& fm_issuerStateOrProvinceName) noexcept;
			UTILITIES_API const void setIssuerLocalityName(const std::string& fm_issuerLocalityName) noexcept;
			UTILITIES_API const void setIssuerOrganizationName(const std::string& fm_issuerOrganizationName) noexcept;
			UTILITIES_API const void setIssuerOrganizationalUnitName(const std::string& fm_issuerOrganizationalUnitName) noexcept;
			UTILITIES_API const void setIssuerCommonName(const std::string& fm_issuerCommonName) noexcept;
			UTILITIES_API const void setIssuerEmailAddress(const std::string& fm_issuerEmailAddress) noexcept;

			UTILITIES_API const void setSerialNumber(const uint64_t fm_serialNumber) noexcept;

			UTILITIES_API const void setSubjectCountryName(const std::string& fm_subjectCountryName) noexcept;
			UTILITIES_API const void setSubjectStateOrProvinceName(const std::string& fm_subjectStateOrProvinceName) noexcept;
			UTILITIES_API const void setSubjectLocalityName(const std::string& fm_subjectLocalityName) noexcept;
			UTILITIES_API const void setSubjectOrganizationName(const std::string& fm_subjectOrganizationName) noexcept;
			UTILITIES_API const void setSubjectOrganizationalUnitName(const std::string& fm_subjectOrganizationalUnitName) noexcept;
			UTILITIES_API const void setSubjectCommonName(const std::string& fm_subjectCommonName) noexcept;
			UTILITIES_API const void setSubjectEmailAddress(const std::string& fm_subjectEmailAddress) noexcept;

			UTILITIES_API const void setBeginValidityDate(const std::time_t& fm_BeginValidityDate) noexcept;
			UTILITIES_API const void setEndValidityDate(const std::time_t& fm_EndValidityDate) noexcept;

			UTILITIES_API const void setPrivateKeyPassword(const std::string& fm_subjectPrivateKeyPassword) noexcept;

			UTILITIES_API const void setCertificateType(const certificateType& fm_certificateType) noexcept;
			UTILITIES_API const void setPrivateKeyType(const privateKeyType& fm_privateKeyType) noexcept;

			UTILITIES_API const void setExtensions(const std::map<certificateExtension, std::string>& fm_extensions) noexcept;

			UTILITIES_API std::map<certificateExtension, std::string>& setExtensions(void) noexcept;

			UTILITIES_API const std::string& getIssuerCountryName(void) const noexcept;
			UTILITIES_API const std::string& getIssuerStateOrProvinceName(void) const noexcept;
			UTILITIES_API const std::string& getIssuerLocalityName(void) const noexcept;
			UTILITIES_API const std::string& getIssuerOrganizationName(void) const noexcept;
			UTILITIES_API const std::string& getIssuerOrganizationalUnitName(void) const noexcept;
			UTILITIES_API const std::string& getIssuerCommonName(void) const noexcept;
			UTILITIES_API const std::string& getIssuerEmailAddress(void) const noexcept;

			UTILITIES_API const uint64_t getSerialNumber(void) const noexcept;

			UTILITIES_API const std::string& getSubjectCountryName(void) const noexcept;
			UTILITIES_API const std::string& getSubjectStateOrProvinceName(void) const noexcept;
			UTILITIES_API const std::string& getSubjectLocalityName(void) const noexcept;
			UTILITIES_API const std::string& getSubjectOrganizationName(void) const noexcept;
			UTILITIES_API const std::string& getSubjectOrganizationalUnitName(void) const noexcept;
			UTILITIES_API const std::string& getSubjectCommonName(void) const noexcept;
			UTILITIES_API const std::string& getSubjectEmailAddress(void) const noexcept;

			UTILITIES_API const std::time_t& getBeginValidityDate(void) const noexcept;
			UTILITIES_API const std::time_t& getEndValidityDate(void) const noexcept;

			UTILITIES_API const std::string& getPrivateKeyPassword(void) const noexcept;

			UTILITIES_API const certificateType& getCertificateType(void) const noexcept;
			UTILITIES_API const privateKeyType& getPrivateKeyType(void) const noexcept;

			UTILITIES_API const std::map<certificateExtension, std::string>& getExtensions(void) const noexcept;

		};
	}
}
