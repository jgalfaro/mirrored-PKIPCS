/*
Télécom SudParis
2017, February
Pierre-Olivier Rocher
*/

#pragma once

#include <stonepine/security/certificate/x509CertificateTools.h>
#include <stonepine/software/certificateStoreServer/certificateStoreServer.h>

#include <codeSynthesis/certificateStoreConfigBinding/certificateStoreConfigUtilities.h>

class certificateStoreServerFromXml final : public certificateStoreServer, protected certificateStoreConfigBinding::certificateStoreConfigUtilities
{

private:

	// Configuration
	std::shared_ptr<certificateStoreConfigBinding::certificateStoreConfigurationComplexType> m_certificateStoreConfiguration;

protected:

	bool m_dumpCreatedCertificates;

private:

	certificateStoreServerFromXml(void) noexcept;

	const void configureCertificateStoreServer(void) noexcept;

	template <typename T>
	const std::map<int, std::string> getCertificateExtensionsToAdd(const T& fm_defalutCertificateAuthorityOrCertificate) noexcept (false);
	
	template <typename T>
	const void checkIssuerAndSubjectCommonNamesAndSubjectCertificateType(const T& fm_defalutCertificateAuthorityOrCertificate) noexcept (false);

	/*template <typename T>
	const void generateDefaultCertificateAuthoritiesOrCertificates(const T& fm_defalutCertificateAuthorityOrCertificate, std::function<void(certificateStoreConfigBinding::defaultCertificateAuthorityComplexType&)> fm_selfSignedCertificateAuthorityOrCertificateFunction, std::function<void(certificateStoreConfigBinding::defaultCertificateAuthorityComplexType&)> fm_CertificateAuthorityOrCertificateSignedByIsserFunction) noexcept (false);*/

	const void generateDefaultCertificateAuthorities(const certificateStoreConfigBinding::defaultCertificateAuthorityComplexType& fm_defaultCertificateAuthority) noexcept (false);	
	const void generateDefaultSelfSignedCertificateAuthorities(const certificateStoreConfigBinding::defaultCertificateAuthorityComplexType& fm_defaultCertificateAuthority) noexcept (false);
	const void generateDefaultOtherCertificateAuthorities(const certificateStoreConfigBinding::defaultCertificateAuthorityComplexType& fm_defaultCertificateAuthority) noexcept (false);

	const void generateDefaultCertificates(const certificateStoreConfigBinding::defaultCertificateComplexType& fm_defaultCertificate) noexcept (false);
	const void generateDefaultOtherCertificates(const certificateStoreConfigBinding::defaultCertificateComplexType& fm_defaultCertificate) noexcept (false);

	const void loadCertificateAuthorities(const certificateStoreConfigBinding::defaultCertificateAuthorityComplexType& fm_defaultCertificateAuthority) noexcept (false);
	const void loadOCSPSigningCertificates(const certificateStoreConfigBinding::defaultCertificateComplexType& fm_defaultCertificate) noexcept (false);

	const std::shared_ptr<certificateStoreBinding::certificateStoreComplexType> postToMasterCertificateStoreCertificateRequestEndPoint(const std::string& fm_subjectCommonName, const std::string& fm_issuerCommonName, const certificateStoreBinding::certificateTypeSimpleType& fm_certificateType, const std::string& fm_certificateRequestPemData) noexcept;

	template <typename T>
	const void addCertificateFromMasterCertificateStore(const T& fm_defalutCertificateAuthorityOrCertificate) noexcept (false);

	// Proper exit
	const void exitProperly(const bool fm_exitStatus) const noexcept;

protected:

	CERTIFICATESTORESERVER_API void setCertificateStoreConfiguration(const std::shared_ptr<certificateStoreConfigBinding::certificateStoreConfigurationComplexType> fm_certificateStoreConfiguration) noexcept;

	CERTIFICATESTORESERVER_API const void generateOrLoadDefaultCertificateAuthoritiesAndCertificates(void)  noexcept (false);

public:

	CERTIFICATESTORESERVER_API certificateStoreServerFromXml(const std::string& fm_xmlConfigurationFileFilePath, const bool fm_initializeDatabase = { false }, const bool fm_dumpCreatedCertificates = { false }) noexcept;

	CERTIFICATESTORESERVER_API virtual ~certificateStoreServerFromXml(void);

};

template<typename T>
inline const std::map<int, std::string> certificateStoreServerFromXml::getCertificateExtensionsToAdd(const T& fm_defalutCertificateAuthorityOrCertificate) noexcept (false)
{
	auto xCCE = std::map<int, std::string>();

	xCCE.insert(std::make_pair(NID_crl_distribution_points, "URI:http://" + server::getServerIpAddress() + ":" + std::to_string(server::getServerPort()) + "/certificateRevocationList/" + fm_defalutCertificateAuthorityOrCertificate.subject().commonName().get() + ".crl.pem"));
	xCCE.insert(std::make_pair(NID_info_access, "OCSP,URI:http://" + server::getServerIpAddress() + ":" + std::to_string(server::getServerPort()) + "/onlineCertificateStatusProtocol/" + fm_defalutCertificateAuthorityOrCertificate.subject().commonName().get()));

	if (fm_defalutCertificateAuthorityOrCertificate.subject().certificateExtensions().present() == true)
	{
		for (auto &iCE : fm_defalutCertificateAuthorityOrCertificate.subject().certificateExtensions()->certificateExtension())
		{
			if (iCE.name() == "NID_crl_distribution_points")
			{
				xCCE.insert(std::make_pair(103, iCE.value()));
			}
			else if (iCE.name() == "NID_info_access")
			{
				xCCE.insert(std::make_pair(177, iCE.value()));
			}
			else if (iCE.name() == "NID_basic_constraints")
			{
				xCCE.insert(std::make_pair(87, iCE.value()));
			}
			else
			{
				std::runtime_error("unknow option");
			}
		}
	}

	return xCCE;
}

template<typename T>
inline const void certificateStoreServerFromXml::checkIssuerAndSubjectCommonNamesAndSubjectCertificateType(const T& fm_defalutCertificateAuthorityOrCertificate) noexcept (false)
{
	if (!(fm_defalutCertificateAuthorityOrCertificate.subject().commonName().present() == true && fm_defalutCertificateAuthorityOrCertificate.issuer().commonName().present() == true))
	{
		throw std::runtime_error("both issuer and subject common names are mandatory");
	}

	if (fm_defalutCertificateAuthorityOrCertificate.subject().certificateType().present() == false)
	{
		throw std::runtime_error("subject certificate type is mandatory");
	}
}

template<typename T>
inline const void certificateStoreServerFromXml::addCertificateFromMasterCertificateStore(const T& fm_defalutCertificateAuthorityOrCertificate) noexcept (false)
{
	auto sCN = fm_defalutCertificateAuthorityOrCertificate.subject().commonName().get();
	auto pKP = fm_defalutCertificateAuthorityOrCertificate.subject().privateKeyPassword().get();

	auto iCR = generateAndAddCertificateRequest(sCN, fm_defalutCertificateAuthorityOrCertificate.subject().privateKeySize().get(), pKP);

	if (iCR > 0)
	{
		auto iCN = fm_defalutCertificateAuthorityOrCertificate.issuer().commonName().get();
		auto cT = fm_defalutCertificateAuthorityOrCertificate.subject().certificateType().get();

		auto cR = getCertificateRequest(sCN, true, pKP);

		auto cPDU = postToMasterCertificateStoreCertificateRequestEndPoint(sCN, iCN, cT, cR->getX509CertificateRequestData());

		if (cPDU != nullptr && cPDU->certificates().present() == true)
		{
			for (auto &iTC : cPDU->certificates()->certificate())
			{
				if (cPDU->certificates()->certificate().size() > 1)
				{
					throw std::runtime_error("answer contains more than one certificate");
				}

				if (iTC.x509Certificate().present() == false)
				{
					throw std::runtime_error("no X509 certificate");
				}

				auto uCAD = std::make_shared<Stonepine::Security::Certificate::x509Certificate>(iTC.x509Certificate()->pemData().data());

				uCAD->setCertificateType(certificateStoreBinding::certificateStoreUtilities::getCertificateType(cT));

				auto iC = addCertificate(iCR, uCAD);

				if (iC > 0)
				{
					if (Stonepine::Security::Certificate::x509CertificateTools::isCertificateAnAuthority(certificateStoreBinding::certificateStoreUtilities::getCertificateType(cT)) == true)
					{

					}

					m_certificateAuthoritiesSubjectCommonNamesAndPrivateKeysPasswords.insert(std::make_pair(sCN, pKP));
					m_certificateAuthoritiesSubjectCommonNamesAndDatabaseIndex.insert(std::make_pair(sCN, iC));
				}
				else
				{
					throw std::runtime_error("");
				}
			}
		}
		else
		{
			throw std::runtime_error("empty answer");
		}
	}
}

//template<typename T>
//inline const void certificateStoreServerFromXml::generateDefaultCertificateAuthoritiesOrCertificates(const T& fm_defalutCertificateAuthorityOrCertificate, std::function<void(certificateStoreConfigBinding::defaultCertificateAuthorityComplexType&)> fm_selfSignedCertificateAuthorityOrCertificateFunction, std::function<void(certificateStoreConfigBinding::defaultCertificateAuthorityComplexType&)> fm_CertificateAuthorityOrCertificateSignedByIsserFunction) noexcept (false)
//{
//	if (fm_defaultCertificateAuthority.subject().commonName().get() == fm_defaultCertificateAuthority.issuer().commonName().get())
//	{
//		fm_selfSignedCertificateAuthorityOrCertificateFunction(fm_defaultCertificateAuthority);
//	}
//	else
//	{
//		if (m_httpsClientConfiguration == nullptr)
//		{
//			if (m_certificateAuthoritiesSubjectCommonNamesAndPrivateKeysPasswords.find(fm_defaultCertificateAuthority.issuer().commonName().get()) != m_certificateAuthoritiesSubjectCommonNamesAndPrivateKeysPasswords.end())
//			{
//				fm_CertificateAuthorityOrCertificateSignedByIsserFunction(fm_defaultCertificateAuthority);
//			}
//
//			throw std::runtime_error("");
//		}
//		else
//		{
//			if (m_certificateAuthoritiesSubjectCommonNamesAndPrivateKeysPasswords.find(fm_defaultCertificateAuthority.issuer().commonName().get()) != m_certificateAuthoritiesSubjectCommonNamesAndPrivateKeysPasswords.end())
//			{
//				fm_CertificateAuthorityOrCertificateSignedByIsserFunction(fm_defaultCertificateAuthority);
//			}
//			else
//			{
//				auto sCN = fm_defaultCertificateAuthority.subject().commonName().get();
//				auto pKP = fm_defaultCertificateAuthority.subject().privateKeyPassword().get();
//
//				auto iCR = generateAndAddCertificateRequest(sCN, fm_defaultCertificateAuthority.subject().privateKeySize().get(), pKP);
//
//				if (iCR > 0)
//				{
//					auto iCN = fm_defaultCertificateAuthority.issuer().commonName().get();
//					auto cT = fm_defaultCertificateAuthority.subject().certificateType().get();
//
//					auto cR = getCertificateRequest(sCN, true, pKP);
//
//					auto cPDU = postToMasterCertificateStoreCertificateRequestEndPoint(sCN, iCN, cT, cR->getX509CertificateRequestData());
//
//					if (cPDU != nullptr && cPDU->certificates().present() == true)
//					{
//						for (auto &iTC : cPDU->certificates()->certificate())
//						{
//							if (cPDU->certificates()->certificate().size() > 1)
//							{
//								throw std::runtime_error("answer contains more than one certificate");
//							}
//
//							if (iTC.x509Certificate().present() == false)
//							{
//								throw std::runtime_error("no X509 certificate");
//							}
//
//							auto uCAD = std::make_shared<Stonepine::Security::Certificate::x509Certificate>(iTC.x509Certificate()->pemData().data());
//
//							uCAD->setCertificateType(certificateStoreBinding::certificateStoreUtilities::getCertificateType(cT));
//
//							auto iC = addCertificate(iCR, uCAD);
//
//							if (iC > 0)
//							{
//								m_certificateAuthoritiesSubjectCommonNamesAndPrivateKeysPasswords.insert(std::make_pair(sCN, pKP));
//								m_certificateAuthoritiesSubjectCommonNamesAndDatabaseIndex.insert(std::make_pair(sCN, iC));
//							}
//							else
//							{
//								throw std::runtime_error("");
//							}
//						}
//					}
//					else
//					{
//						throw std::runtime_error("empty answer");
//					}
//				}
//			}
//		}
//	}
//}
