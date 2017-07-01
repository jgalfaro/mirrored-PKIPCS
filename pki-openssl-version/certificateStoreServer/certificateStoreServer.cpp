/*
Télécom SudParis
2017, February
Pierre-Olivier Rocher
*/

#include <functional>

#include "certificateStoreServer.h"

#include <stonepine/security/certificate/x509CertificateTools.h>

#include <codeSynthesis/x509CertificateBinding/x509CertificateUtilities.h>

#include <stonepine/utilities/xml/server/codeSynthesis/httpsServerFromCodeSynthesis.h>

#include <stonepine/libraries/openssl.ocsp.h>

#include <stonepine/utilities/utilities.h>
#include <stonepine/utilities/stonepine.utilities.regex.h>
#include <stonepine/utilities/stonepine.utilities.iO.h>

#include <stonepine/network/client/httpsClientFromXml.h>

namespace Stonepine
{
	const std::shared_ptr<certificateStoreBinding::certificatesComplexType> certificateStoreServer::getCertificateChainFromMasterCertificateStore(void) noexcept
	{
		auto cC = getXFromMasterCertificateStore("/certificateChain");

		if (cC->certificates().present() == true)
		{
			return std::make_shared<certificateStoreBinding::certificatesComplexType>(cC->certificates().get());
		}

		return nullptr;
	}

	const std::map<std::string, std::string> certificateStoreServer::processHeaderFields(const std::map<std::string, std::string>& fm_inputHeaderFields) const noexcept
	{
		std::map<std::string, std::string> hF = {};

		const auto c = fm_inputHeaderFields.find("Connection");

		if (c != fm_inputHeaderFields.end())
		{
#ifdef __WINDOWS__
			hF.try_emplace("Connection", c->second);
#else
			hF.insert(std::make_pair("Connection", c->second));
#endif	
		}
#ifdef __WINDOWS__
		hF.try_emplace("Content-Type", "text/xml; charset=UTF-8");
#else
		hF.insert(std::make_pair("Content-Type", "text/xml; charset=UTF-8"));
#endif
		return hF;
	}

	const void certificateStoreServer::generateOrLoadDefaultCertificateAuthoritiesAndCertificates(void) noexcept (false)
	{
		for (auto &iTDCA : *m_defaultCertificateAuthorities.getCertificates().get())
		{
			if (m_isDatabaseInitilizationEnabled == true)
			{
				generateDefaultCertificateAuthorities(iTDCA);
			}
			else
			{
				loadCertificateAuthorities(iTDCA);
			}
		}

		if (m_isDefalutCreatedCertificatesDumpEnabled == true)
		{
			m_certificateStoreDatabase->writeCertificateAuthorityChain({}, Stonepine::utilities::extractFirstElements(m_certificateAuthoritiesSubjectCommonNamesAndDatabaseIndex));
		}

		for (auto &iTDC : *m_defaultCertificates.getCertificates().get())
		{
			if (m_isDatabaseInitilizationEnabled == true)
			{
				generateDefaultCertificates(iTDC);
			}
			else
			{
				loadOCSPSigningCertificates(iTDC);
			}
		}

		if (m_ocspSubjectCommonNamesAndPrivateKeysPasswords.empty() == true || m_ocspSubjectCommonNamesAndDatabaseIndex.empty() == true || m_certificateAuthoritiesSubjectCommonNamesAndOcspSubjectCommonNames.empty() == true)
		{
			Stonepine::LOG_INFO("[certificateStoreServer] no OCSP informations in the XML configuration file, only certificateRevocationList endpoint will be available");
		}
		else
		{
			if (m_ocspSubjectCommonNamesAndPrivateKeysPasswords.size() == m_certificateAuthoritiesSubjectCommonNamesAndPrivateKeysPasswords.size())
			{
				m_isOnlineCertificateStatusProtocol = { true };
			}
			else
			{
				throw std::runtime_error("incoherent OCSP configuration");
			}
		}

		m_certificateStoreDatabase->updateCertificateRevocationLists(m_certificateAuthoritiesSubjectCommonNamesAndPrivateKeysPasswords);
	}

	const std::shared_ptr<certificateStoreBinding::certificateRevocationListsComplexType> certificateStoreServer::getCertificateRevocationListFromMasterCertificateStore(void) noexcept
	{
		auto cRLS = getXFromMasterCertificateStore("/certificateRevocationList");

		if (cRLS->certificateRevocationLists().present() == true)
		{
			return std::make_shared<certificateStoreBinding::certificateRevocationListsComplexType>(cRLS->certificateRevocationLists().get());
		}

		return nullptr;
	}

	const void certificateStoreServer::updateCertificateExtensions(const std::shared_ptr<Stonepine::Utilities::certificate> fm_defaultCertificateAuthorityOrCertificate) noexcept (false)
	{
		auto cES = fm_defaultCertificateAuthorityOrCertificate->getExtensions();

		if (m_httpsServer->getExternalServerIpAddress().empty() == false)
		{
			cES.insert(std::make_pair(Stonepine::Utilities::certificateExtension::CE_NID_CRL_DISTRIBUTION_POINTS, "URI:http://" + m_httpsServer->getExternalServerIpAddress() + ":" + std::to_string(m_httpsServer->getServerPort()) + "/certificateRevocationList/" + fm_defaultCertificateAuthorityOrCertificate->getSubjectCommonName() + ".crl.pem"));
			cES.insert(std::make_pair(Stonepine::Utilities::certificateExtension::CE_NID_INFO_ACCESS, "OCSP,URI:http://" + m_httpsServer->getExternalServerIpAddress() + ":" + std::to_string(m_httpsServer->getServerPort()) + "/onlineCertificateStatusProtocol/" + fm_defaultCertificateAuthorityOrCertificate->getSubjectCommonName()));
		}
		else
		{
			cES.insert(std::make_pair(Stonepine::Utilities::certificateExtension::CE_NID_CRL_DISTRIBUTION_POINTS, "URI:http://" + m_httpsServer->getServerIpAddress() + ":" + std::to_string(m_httpsServer->getServerPort()) + "/certificateRevocationList/" + fm_defaultCertificateAuthorityOrCertificate->getSubjectCommonName() + ".crl.pem"));
			cES.insert(std::make_pair(Stonepine::Utilities::certificateExtension::CE_NID_INFO_ACCESS, "OCSP,URI:http://" + m_httpsServer->getServerIpAddress() + ":" + std::to_string(m_httpsServer->getServerPort()) + "/onlineCertificateStatusProtocol/" + fm_defaultCertificateAuthorityOrCertificate->getSubjectCommonName()));
		}

		fm_defaultCertificateAuthorityOrCertificate->setExtensions(cES);
	}

	const void certificateStoreServer::checkIssuerAndSubjectCommonNamesAndSubjectCertificateType(const std::shared_ptr<Stonepine::Utilities::certificate> fm_defaultCertificateAuthorityOrCertificate) noexcept (false)
	{
		if (fm_defaultCertificateAuthorityOrCertificate->getSubjectCommonName().empty() == false && fm_defaultCertificateAuthorityOrCertificate->getIssuerCommonName().empty() == true)
		{
			throw std::runtime_error("both issuer and subject common names are mandatory");
		}

		if (fm_defaultCertificateAuthorityOrCertificate->getCertificateType() == Stonepine::Utilities::certificateType::CT_X509_UNKNOW)
		{
			throw std::runtime_error("subject certificate type is mandatory");
		}
	}

	const void certificateStoreServer::generateDefaultCertificateAuthorities(const std::shared_ptr<Stonepine::Utilities::certificate> fm_defaultCertificateAuthority) noexcept (false)
	{
		checkIssuerAndSubjectCommonNamesAndSubjectCertificateType(fm_defaultCertificateAuthority);

		if (fm_defaultCertificateAuthority->getPrivateKeyPassword().empty() == true)
		{
			throw std::runtime_error("private key password is mandatory for certificate authorities");
		}

		const auto iCN = fm_defaultCertificateAuthority->getIssuerCommonName();

		if (fm_defaultCertificateAuthority->getSubjectCommonName() == iCN)
		{
			generateDefaultSelfSignedCertificateAuthorities(fm_defaultCertificateAuthority);
		}
		else
		{
			if (m_httpsClient == nullptr)
			{
				if (m_certificateAuthoritiesSubjectCommonNamesAndPrivateKeysPasswords.find(iCN) != m_certificateAuthoritiesSubjectCommonNamesAndPrivateKeysPasswords.end())
				{
					generateDefaultOtherCertificateAuthorities(fm_defaultCertificateAuthority);
				}
				else
				{
					throw std::runtime_error("unable to find \"" + iCN + "\" certificate authority");
				}
			}
			else
			{
				if (m_certificateAuthoritiesSubjectCommonNamesAndPrivateKeysPasswords.find(iCN) != m_certificateAuthoritiesSubjectCommonNamesAndPrivateKeysPasswords.end())
				{
					generateDefaultOtherCertificateAuthorities(fm_defaultCertificateAuthority);
				}
				else
				{
					addCertificateFromMasterCertificateStore(fm_defaultCertificateAuthority);
				}
			}
		}
	}

	const void certificateStoreServer::generateDefaultSelfSignedCertificateAuthorities(const std::shared_ptr<Stonepine::Utilities::certificate> fm_defaultCertificateAuthority) noexcept (false)
	{
		// Sanity check
		if (fm_defaultCertificateAuthority->getCertificateType() != Stonepine::Utilities::certificateType::CT_X509_SELF_SIGNED_CA)
		{
			throw std::runtime_error("incoherent configuration");
		}

		updateCertificateExtensions(fm_defaultCertificateAuthority);

		const auto sCN = fm_defaultCertificateAuthority->getSubjectCommonName();
		const auto pKP = fm_defaultCertificateAuthority->getPrivateKeyPassword();

		const auto iSSCA = m_certificateStoreDatabase->generateAndAddSelfSignedCertificateAuthorityCertificate(sCN, /*fm_defaultCertificateAuthority.subject().privateKeySize().get()*/4096, /*fm_defaultCertificateAuthority.subject().numberOfValidDaysFromNow().get()*/3650, pKP);

		if (iSSCA > 0)
		{
			m_certificateAuthoritiesSubjectCommonNamesAndPrivateKeysPasswords.insert(std::make_pair(sCN, pKP));
			m_certificateAuthoritiesSubjectCommonNamesAndDatabaseIndex.insert(std::make_pair(sCN, iSSCA));
		}
		else
		{
			throw std::runtime_error("");
		}

		if (m_isDefalutCreatedCertificatesDumpEnabled == true)
		{
			m_certificateStoreDatabase->writeCertificateAuthority({}, sCN);
		}
	}

	const void certificateStoreServer::generateDefaultOtherCertificateAuthorities(const std::shared_ptr<Stonepine::Utilities::certificate> fm_defaultCertificateAuthority) noexcept (false)
	{
		const auto cT = fm_defaultCertificateAuthority->getCertificateType();

		if (cT == Stonepine::Utilities::certificateType::CT_X509_CA || cT == Stonepine::Utilities::certificateType::CT_X509_INTERMEDIATE_CA)
		{
			const auto iCN = fm_defaultCertificateAuthority->getIssuerCommonName();
			const auto sCN = fm_defaultCertificateAuthority->getSubjectCommonName();
			const auto pKP = fm_defaultCertificateAuthority->getPrivateKeyPassword();

			// Create and add the CA certificate request in the database
			const auto iTICAR = m_certificateStoreDatabase->generateAndAddCertificateRequest(fm_defaultCertificateAuthority);

			if (iTICAR > 0)
			{
				updateCertificateExtensions(fm_defaultCertificateAuthority);

				const auto iICA = m_certificateStoreDatabase->generateAndAddCertificate(m_certificateAuthoritiesSubjectCommonNamesAndDatabaseIndex[iCN], iTICAR, cT, /*fm_defaultCertificateAuthority.subject().numberOfValidDaysFromNow().get()*/365, m_certificateAuthoritiesSubjectCommonNamesAndPrivateKeysPasswords[iCN]);

				if (iICA > 0)
				{
					m_certificateAuthoritiesSubjectCommonNamesAndPrivateKeysPasswords.insert(std::make_pair(sCN, pKP));
					m_certificateAuthoritiesSubjectCommonNamesAndDatabaseIndex.insert(std::make_pair(sCN, iICA));
				}
				else
				{
					throw std::runtime_error("");
				}
			}
			else
			{
				throw std::runtime_error("");
			}

			if (m_isDefalutCreatedCertificatesDumpEnabled == true)
			{
				m_certificateStoreDatabase->writeCertificateAuthority({}, sCN);
			}
		}
	}

	const void certificateStoreServer::generateDefaultCertificates(const std::shared_ptr<Stonepine::Utilities::certificate> fm_defaultCertificate) noexcept (false)
	{
		checkIssuerAndSubjectCommonNamesAndSubjectCertificateType(fm_defaultCertificate);

		const auto iCN = fm_defaultCertificate->getIssuerCommonName();

		if (fm_defaultCertificate->getSubjectCommonName() == iCN)
		{
			throw std::runtime_error("unable to generate self signed certificate");
		}
		else
		{
			if (m_httpsClient == nullptr)
			{
				if (m_certificateAuthoritiesSubjectCommonNamesAndPrivateKeysPasswords.find(iCN) != m_certificateAuthoritiesSubjectCommonNamesAndPrivateKeysPasswords.end())
				{
					generateDefaultOtherCertificates(fm_defaultCertificate);
				}
				else
				{
					throw std::runtime_error("unable to find \"" + iCN + "\" certificate authority");
				}
			}
			else
			{
				if (m_certificateAuthoritiesSubjectCommonNamesAndPrivateKeysPasswords.find(iCN) != m_certificateAuthoritiesSubjectCommonNamesAndPrivateKeysPasswords.end())
				{
					generateDefaultOtherCertificates(fm_defaultCertificate);
				}
				else
				{
					addCertificateFromMasterCertificateStore(fm_defaultCertificate);
				}
			}
		}
	}

	const void certificateStoreServer::generateDefaultOtherCertificates(const std::shared_ptr<Stonepine::Utilities::certificate> fm_defaultCertificate) noexcept (false)
	{
		const auto cT = fm_defaultCertificate->getCertificateType();

		if (cT == Stonepine::Utilities::certificateType::CT_X509_CLIENT || cT == Stonepine::Utilities::certificateType::CT_X509_SERVER || cT == Stonepine::Utilities::certificateType::CT_X509_OCSP_SIGNING)
		{
			updateCertificateExtensions(fm_defaultCertificate);

			const auto iCN = fm_defaultCertificate->getIssuerCommonName();
			const auto sCN = fm_defaultCertificate->getSubjectCommonName();

			const auto pKP = fm_defaultCertificate->getPrivateKeyPassword();

			const auto iCRAC = m_certificateStoreDatabase->generateAndAddCertificateRequestAndGenerateAndAddCertificate(iCN, sCN, cT, /*fm_defaultCertificate.subject().privateKeySize().get()*/2048, /*fm_defaultCertificate.subject().numberOfValidDaysFromNow().get()*/365, m_certificateAuthoritiesSubjectCommonNamesAndPrivateKeysPasswords[iCN], pKP);

			if (!(iCRAC.first > 0 && iCRAC.second > 0))
			{
				throw std::runtime_error("");
			}

			if (cT == Stonepine::Utilities::certificateType::CT_X509_OCSP_SIGNING)
			{
				m_ocspSubjectCommonNamesAndPrivateKeysPasswords.insert(std::make_pair(sCN, pKP));
				m_ocspSubjectCommonNamesAndDatabaseIndex.insert(std::make_pair(sCN, iCRAC.second));

				m_certificateAuthoritiesSubjectCommonNamesAndOcspSubjectCommonNames.insert(std::make_pair(iCN, sCN));
			}

			if (cT != Stonepine::Utilities::certificateType::CT_X509_OCSP_SIGNING && m_isDefalutCreatedCertificatesDumpEnabled == true)
			{
				m_certificateStoreDatabase->writeCertificateAndCertificateRequestAndPrivateKey({}, sCN);
			}
		}
	}

	const void certificateStoreServer::loadCertificateAuthorities(const std::shared_ptr<Stonepine::Utilities::certificate> fm_defaultCertificateAuthority) noexcept (false)
	{
		checkIssuerAndSubjectCommonNamesAndSubjectCertificateType(fm_defaultCertificateAuthority);

		if (Stonepine::Security::Certificate::x509CertificateTools::isCertificateAnAuthority(fm_defaultCertificateAuthority->getCertificateType()) == false)
		{
			throw std::runtime_error("incoherent configuration");
		}

		auto sCN = fm_defaultCertificateAuthority->getSubjectCommonName();

		auto iSSCA = m_certificateStoreDatabase->getCertificateAuthorityId(sCN);

		if (iSSCA > 0)
		{
			m_certificateAuthoritiesSubjectCommonNamesAndPrivateKeysPasswords.insert(std::make_pair(sCN, fm_defaultCertificateAuthority->getPrivateKeyPassword()));
			m_certificateAuthoritiesSubjectCommonNamesAndDatabaseIndex.insert(std::make_pair(sCN, iSSCA));

			Stonepine::LOG_INFO("\"" + sCN + "\" certificate auhtority has been successfully loaded...");
		}
		else
		{
			throw std::runtime_error("");
		}
	}

	const void certificateStoreServer::loadOCSPSigningCertificates(const std::shared_ptr<Stonepine::Utilities::certificate> fm_defaultCertificate) noexcept (false)
	{
		checkIssuerAndSubjectCommonNamesAndSubjectCertificateType(fm_defaultCertificate);

		if (fm_defaultCertificate->getCertificateType() == Stonepine::Utilities::certificateType::CT_X509_OCSP_SIGNING)
		{
			auto sCN = fm_defaultCertificate->getSubjectCommonName();

			auto iIC = m_certificateStoreDatabase->getCertificateId(sCN);
			auto c = m_certificateStoreDatabase->getCertificate(sCN);

			if (iIC > 0 && c != nullptr)
			{
				m_ocspSubjectCommonNamesAndPrivateKeysPasswords.insert(std::make_pair(sCN, fm_defaultCertificate->getPrivateKeyPassword()));
				m_ocspSubjectCommonNamesAndDatabaseIndex.insert(std::make_pair(sCN, iIC));

				m_certificateAuthoritiesSubjectCommonNamesAndOcspSubjectCommonNames.insert(std::make_pair(c->getIssuerCommonName(), sCN));

				Stonepine::LOG_INFO("[certificateStoreServer] \"" + sCN + "\" OCSP signing certificate as been successfully loaded...");
			}
			else
			{
				throw std::runtime_error("");
			}
		}
	}

	const std::shared_ptr<certificateStoreBinding::certificateStoreComplexType> certificateStoreServer::postToMasterCertificateStoreCertificateRequestEndPoint(const std::string& fm_subjectCommonName, const std::string& fm_issuerCommonName, const Stonepine::Utilities::certificateType& fm_certificateType, const std::string& fm_certificateRequestPemData) noexcept
	{
		return std::shared_ptr<certificateStoreBinding::certificateStoreComplexType>();
	}

	const void certificateStoreServer::addCertificateFromMasterCertificateStore(const std::shared_ptr<Stonepine::Utilities::certificate> fm_defaultCertificateAuthorityOrCertificate) noexcept (false)
	{
		const auto sCN = fm_defaultCertificateAuthorityOrCertificate->getSubjectCommonName();
		const auto pKP = fm_defaultCertificateAuthorityOrCertificate->getPrivateKeyPassword();

		const auto iCR = m_certificateStoreDatabase->generateAndAddCertificateRequest(fm_defaultCertificateAuthorityOrCertificate);
		
		if (iCR > 0)
		{
			const auto iCN = fm_defaultCertificateAuthorityOrCertificate->getIssuerCommonName();
			const auto cT = fm_defaultCertificateAuthorityOrCertificate->getCertificateType();

			const auto cR = m_certificateStoreDatabase->getCertificateRequest(sCN, true, pKP);

			const auto cPDU = postToMasterCertificateStoreCertificateRequestEndPoint(sCN, iCN, cT, cR->getX509CertificateRequestAsPemData());

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

					const auto uCAD = std::make_shared<Stonepine::Security::Certificate::x509Certificate>(iTC.x509Certificate()->pemData().data());

					uCAD->setCertificateType(cT);

					const auto iC = m_certificateStoreDatabase->addCertificate(iCR, uCAD);

					if (iC > 0)
					{
						if (Stonepine::Security::Certificate::x509CertificateTools::isCertificateAnAuthority(cT) == true)
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

	const void certificateStoreServer::exitProperly(const bool fm_exitStatus) const noexcept
	{
		certificateStoreServer::~certificateStoreServer();

		exit(fm_exitStatus);
	}

	const std::shared_ptr<certificateStoreBinding::certificateStoreComplexType> certificateStoreServer::getXFromMasterCertificateStore(const std::string& fm_endPoint) noexcept
	{
		std::shared_ptr<certificateStoreBinding::certificateStoreComplexType> r = { nullptr };

		if (m_httpsClient != nullptr)
		{
			auto hC = m_httpsClient;

			auto hF = std::map<std::string, std::string>();

			hF.insert(std::make_pair("Content-Type", "text/xml; charset=UTF-8"));
			hF.insert(std::make_pair("Accept", "text/xml; charset=UTF-8"));
			hF.insert(std::make_pair("Connection", "close"));

			hC->sendPacket({}, fm_endPoint, Stonepine::Network::Protocol::httpMethod::HM_GET, hF);

			r = certificateStoreBinding::certificateStoreUtilities::readXmlData(hC->getResponseData());
		}

		return r;
	}

	const void certificateStoreServer::configureEndPoints(void) noexcept
	{
		m_httpsServer->addACallback("/certificateAuthority", std::bind(&certificateStoreServer::answerCertificateAuthority, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5));
		m_httpsServer->addACallback("/certificateRequest", std::bind(&certificateStoreServer::answerCertificateRequest, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5));
		m_httpsServer->addACallback("/certificate", std::bind(&certificateStoreServer::answerCertificate, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5));

		m_httpsServer->addACallback("/certificateChain", std::bind(&certificateStoreServer::answerCertificateAuthorityChain, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5));

		if (m_isCertificateRevocationListEnabled == true)
		{
			m_httpsServer->addACallback("/certificateRevocationList", std::bind(&certificateStoreServer::answerCertificateRevocationList, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5));
		}

		m_httpsServer->addACallback("/verifyCertificate", std::bind(&certificateStoreServer::answerVerifyCertificate, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5));

		m_httpsServer->setDefaultCallback(std::bind(&certificateStoreServer::answerNotFoundPage, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5));

		m_httpsServer->setAllowedMethods(Stonepine::Network::Protocol::httpMethod::HM_GET | Stonepine::Network::Protocol::httpMethod::HM_POST);
	}

	const std::tuple<std::string, Stonepine::Network::Protocol::httpStatusCode, std::string, std::map<std::string, std::string>> certificateStoreServer::answerCertificateAuthority(const std::string& fm_peerCertificateSubjectCommonName, const std::string& fm_requestContent, const Network::Protocol::httpMethod& fm_requestType, const std::map<std::string, std::string>& fm_headerFields, const std::string& fm_requestPath) noexcept
	{
		if (m_httpsServer->isClientAuthorized(fm_peerCertificateSubjectCommonName) == true)
		{
			return std::make_tuple(std::string(), Stonepine::Network::Protocol::httpStatusCode::HSC_UNAUTHORIZED, std::string(), processHeaderFields(fm_headerFields));
		}

		if (fm_requestType == Network::Protocol::httpMethod::HM_GET)
		{
			std::string aC = {}, eD = {};

			Stonepine::Network::Protocol::httpStatusCode eC = Stonepine::Network::Protocol::httpStatusCode::HSC_NOT_IMPLEMENTED;

			std::map<std::string, std::string> hF = {};

			auto cAS = m_certificateStoreDatabase->getCertificateAuthoritySubjectCommonNamesAndSubjectSerialNumbers();

			if (cAS->size() > 0)
			{
				auto cSCT = certificateStoreBinding::certificateStoreUtilities::generateCertificateStoreTypeWithCertificatesComplexType(cAS);

				eC = Stonepine::Network::Protocol::httpStatusCode::HSC_OK;
				aC = certificateStoreBinding::certificateStoreUtilities::writeXmlData(cSCT);
			}
			else
			{
				eC = Stonepine::Network::Protocol::httpStatusCode::HSC_NOT_FOUND;
			}

			return std::make_tuple(aC, eC, eD, hF);
		}
		else
		{
			return std::make_tuple(std::string(), Stonepine::Network::Protocol::httpStatusCode::HSC_METHOD_NOT_ALLOWED, std::string("only GET method is allowed on this endpoint"), processHeaderFields(fm_headerFields));
		}
	}

	const std::tuple<std::string, Stonepine::Network::Protocol::httpStatusCode, std::string, std::map<std::string, std::string>> certificateStoreServer::answerCertificateRequest(const std::string& fm_peerCertificateSubjectCommonName, const std::string& fm_requestContent, const Network::Protocol::httpMethod& fm_requestType, const std::map<std::string, std::string>& fm_headerFields, const std::string& fm_requestPath) noexcept
	{
		if (m_httpsServer->isClientAuthorized(fm_peerCertificateSubjectCommonName) == true)
		{
			return std::make_tuple(std::string(), Stonepine::Network::Protocol::httpStatusCode::HSC_UNAUTHORIZED, std::string(), processHeaderFields(fm_headerFields));
		}

		if (fm_requestType == Network::Protocol::httpMethod::HM_GET)
		{
			return answerCertificateRequestGet(fm_headerFields);
		}
		else if (fm_requestType == Network::Protocol::httpMethod::HM_POST)
		{
			return answerCertificateRequestPost(fm_requestContent, fm_requestType, fm_headerFields, fm_requestPath);
		}
		else
		{
			return std::make_tuple(std::string(), Stonepine::Network::Protocol::httpStatusCode::HSC_METHOD_NOT_ALLOWED, std::string("only GET and POST methods are allowed on this endpoint"), processHeaderFields(fm_headerFields));
		}
	}

	const std::tuple<std::string, Stonepine::Network::Protocol::httpStatusCode, std::string, std::map<std::string, std::string>> certificateStoreServer::answerCertificate(const std::string& fm_peerCertificateSubjectCommonName, const std::string& fm_requestContent, const Network::Protocol::httpMethod& fm_requestType, const std::map<std::string, std::string>& fm_headerFields, const std::string& fm_requestPath) noexcept
	{
		if (m_httpsServer->isClientAuthorized(fm_peerCertificateSubjectCommonName) == true)
		{
			return std::make_tuple(std::string(), Stonepine::Network::Protocol::httpStatusCode::HSC_UNAUTHORIZED, std::string(), processHeaderFields(fm_headerFields));
		}

		if (fm_requestType == Network::Protocol::httpMethod::HM_GET)
		{
			std::string aC = {}, eD = {};

			Stonepine::Network::Protocol::httpStatusCode eC = Stonepine::Network::Protocol::httpStatusCode::HSC_NOT_IMPLEMENTED;

			std::map<std::string, std::string> hF = {};

			auto cAS = m_certificateStoreDatabase->getCertificateSubjectCommonNamesAndSubjectSerialNumbers();

			if (cAS->size() > 0)
			{
				auto cSCT = certificateStoreBinding::certificateStoreUtilities::generateCertificateStoreTypeWithCertificatesComplexType(cAS);

				eC = Stonepine::Network::Protocol::httpStatusCode::HSC_OK;
				aC = certificateStoreBinding::certificateStoreUtilities::writeXmlData(cSCT);
			}
			else
			{
				eC = Stonepine::Network::Protocol::httpStatusCode::HSC_NOT_FOUND;
			}

			return std::make_tuple(aC, eC, eD, hF);
		}
		else
		{
			return std::make_tuple(std::string(), Stonepine::Network::Protocol::httpStatusCode::HSC_METHOD_NOT_ALLOWED, std::string("only GET method is allowed on this endpoint"), processHeaderFields(fm_headerFields));
		}
	}

	const std::tuple<std::string, Stonepine::Network::Protocol::httpStatusCode, std::string, std::map<std::string, std::string>> certificateStoreServer::answerExtendedCertificateAuthority(const std::string& fm_peerCertificateSubjectCommonName, const std::string& fm_requestContent, const Network::Protocol::httpMethod& fm_requestType, const std::map<std::string, std::string>& fm_headerFields, const std::string& fm_requestPath) noexcept
	{
		if (m_httpsServer->isClientAuthorized(fm_peerCertificateSubjectCommonName) == true)
		{
			return std::make_tuple(std::string(), Stonepine::Network::Protocol::httpStatusCode::HSC_UNAUTHORIZED, std::string(), processHeaderFields(fm_headerFields));
		}

		std::string aC = {}, eD = {};

		Stonepine::Network::Protocol::httpStatusCode eC = Stonepine::Network::Protocol::httpStatusCode::HSC_BAD_REQUEST;

		std::map<std::string, std::string> hF = {};

		try
		{
			if (fm_requestType != Network::Protocol::httpMethod::HM_GET)
			{
				eC = Stonepine::Network::Protocol::httpStatusCode::HSC_METHOD_NOT_ALLOWED;

				throw std::runtime_error("only GET method is allowed on this endpoint");
			}
		}
		catch (const std::exception& e)
		{
			eD = e.what();

			Stonepine::LOG_ERROR(e.what());
		}

		const auto sCN = Stonepine::Utilities::regex::extractRegexMatchAsString(fm_requestPath, "^\/certificateAuthority\/");

		const auto cA = m_certificateStoreDatabase->getCertificateAuthority(sCN);

		if (cA == nullptr)
		{
			eC = Stonepine::Network::Protocol::httpStatusCode::HSC_NOT_FOUND;
		}
		else
		{
			auto cST = certificateStoreBinding::certificateStoreUtilities::generateCertificateStoreTypeWithACertificateComplexType(sCN, cA->getSerialNumber(), cA->getAsPemData());

			std::string a = certificateStoreBinding::certificateStoreUtilities::writeXmlData(cST);

			if (a.empty() == false)
			{
				hF.insert(std::make_pair("Content-Type", "text/xml; charset=UTF-8"));

				aC = a;

				eC = Stonepine::Network::Protocol::httpStatusCode::HSC_OK;
			}
		}

		return std::make_tuple(aC, eC, eD, hF);
	}

	const std::tuple<std::string, Stonepine::Network::Protocol::httpStatusCode, std::string, std::map<std::string, std::string>> certificateStoreServer::answerExtendedCertificateRequest(const std::string& fm_peerCertificateSubjectCommonName, const std::string& fm_requestContent, const Network::Protocol::httpMethod& fm_requestType, const std::map<std::string, std::string>& fm_headerFields, const std::string& fm_requestPath) noexcept
	{
		if (m_httpsServer->isClientAuthorized(fm_peerCertificateSubjectCommonName) == true)
		{
			return std::make_tuple(std::string(), Stonepine::Network::Protocol::httpStatusCode::HSC_UNAUTHORIZED, std::string(), processHeaderFields(fm_headerFields));
		}

		std::string aC = {}, eD = {};

		Stonepine::Network::Protocol::httpStatusCode eC = Stonepine::Network::Protocol::httpStatusCode::HSC_BAD_REQUEST;

		std::map<std::string, std::string> hF = {};

		try
		{
			if (fm_requestType != Network::Protocol::httpMethod::HM_GET)
			{
				eC = Stonepine::Network::Protocol::httpStatusCode::HSC_METHOD_NOT_ALLOWED;

				throw std::runtime_error("only GET method is allowed on this endpoint");
			}
		}
		catch (const std::exception& e)
		{
			eD = e.what();

			Stonepine::LOG_ERROR(e.what());
		}

		const auto sCN = Stonepine::Utilities::regex::extractRegexMatchAsString(fm_requestPath, "^\/certificateRequest\/");

		const auto cR = m_certificateStoreDatabase->getCertificateRequest(sCN);

		if (cR == nullptr)
		{
			eC = Stonepine::Network::Protocol::httpStatusCode::HSC_NOT_FOUND;
		}
		else
		{
			auto cST = certificateStoreBinding::certificateStoreUtilities::generateCertificateStoreTypeWithACertificateRequestType(sCN, cR->getX509CertificateRequestAsPemData());

			std::string a = certificateStoreBinding::certificateStoreUtilities::writeXmlData(cST);

			if (a.empty() == false)
			{
				hF.insert(std::make_pair("Content-Type", "text/xml; charset=UTF-8"));

				aC = a;

				eC = Stonepine::Network::Protocol::httpStatusCode::HSC_OK;
			}
		}

		return std::make_tuple(aC, eC, eD, hF);
	}

	const std::tuple<std::string, Stonepine::Network::Protocol::httpStatusCode, std::string, std::map<std::string, std::string>> certificateStoreServer::answerExtendedCertificate(const std::string& fm_peerCertificateSubjectCommonName, const std::string& fm_requestContent, const Network::Protocol::httpMethod& fm_requestType, const std::map<std::string, std::string>& fm_headerFields, const std::string& fm_requestPath) noexcept
	{
		if (m_httpsServer->isClientAuthorized(fm_peerCertificateSubjectCommonName) == true)
		{
			return std::make_tuple(std::string(), Stonepine::Network::Protocol::httpStatusCode::HSC_UNAUTHORIZED, std::string(), processHeaderFields(fm_headerFields));
		}

		std::string aC = {}, eD = {};

		Stonepine::Network::Protocol::httpStatusCode eC = Stonepine::Network::Protocol::httpStatusCode::HSC_BAD_REQUEST;

		std::map<std::string, std::string> hF = {};

		try
		{
			if (fm_requestType != Network::Protocol::httpMethod::HM_GET)
			{
				eC = Stonepine::Network::Protocol::httpStatusCode::HSC_METHOD_NOT_ALLOWED;

				throw std::runtime_error("only GET method is allowed on this endpoint");
			}
		}
		catch (const std::exception& e)
		{
			eD = e.what();

			Stonepine::LOG_ERROR(e.what());
		}

		const auto sCN = Stonepine::Utilities::regex::extractRegexMatchAsString(fm_requestPath, "^\/certificate\/");

		const auto c = m_certificateStoreDatabase->getCertificate(sCN);

		if (c == nullptr)
		{
			eC = Stonepine::Network::Protocol::httpStatusCode::HSC_NOT_FOUND;
		}
		else
		{
			auto cST = certificateStoreBinding::certificateStoreUtilities::generateCertificateStoreTypeWithACertificateComplexType(sCN, c->getSerialNumber(), c->getAsPemData());

			std::string a = certificateStoreBinding::certificateStoreUtilities::writeXmlData(cST);

			if (a.empty() == false)
			{
				hF.insert(std::make_pair("Content-Type", "text/xml; charset=UTF-8"));

				aC = a;

				eC = Stonepine::Network::Protocol::httpStatusCode::HSC_OK;
			}
		}

		return std::make_tuple(aC, eC, eD, hF);
	}

	const std::tuple<std::string, Stonepine::Network::Protocol::httpStatusCode, std::string, std::map<std::string, std::string>> certificateStoreServer::answerCertificateAuthorityChain(const std::string& fm_peerCertificateSubjectCommonName, const std::string& fm_requestContent, const Network::Protocol::httpMethod& fm_requestType, const std::map<std::string, std::string>& fm_headerFields, const std::string& fm_requestPath) noexcept
	{
		if (m_httpsServer->isClientAuthorized(fm_peerCertificateSubjectCommonName) == true)
		{
			return std::make_tuple(std::string(), Stonepine::Network::Protocol::httpStatusCode::HSC_UNAUTHORIZED, std::string(), processHeaderFields(fm_headerFields));
		}

		std::string aC = {}, eD = {};

		Stonepine::Network::Protocol::httpStatusCode eC = Stonepine::Network::Protocol::httpStatusCode::HSC_NOT_IMPLEMENTED;

		std::map<std::string, std::string> hF = {};

		if (fm_requestType == Network::Protocol::httpMethod::HM_GET)
		{
			return answerCertificateAuthorityChainGet(fm_headerFields);
		}
		else
		{
			return std::make_tuple(std::string(), Stonepine::Network::Protocol::httpStatusCode::HSC_METHOD_NOT_ALLOWED, std::string("only GET method is allowed on this endpoint"), processHeaderFields(fm_headerFields));
		}
	}

	const std::tuple<std::string, Stonepine::Network::Protocol::httpStatusCode, std::string, std::map<std::string, std::string>> certificateStoreServer::answerCertificateRevocationList(const std::string& fm_peerCertificateSubjectCommonName, const std::string& fm_requestContent, const Network::Protocol::httpMethod& fm_requestType, const std::map<std::string, std::string>& fm_headerFields, const std::string& fm_requestPath) noexcept
	{
		if (m_httpsServer->isClientAuthorized(fm_peerCertificateSubjectCommonName) == true)
		{
			return std::make_tuple(std::string(), Stonepine::Network::Protocol::httpStatusCode::HSC_UNAUTHORIZED, std::string(), processHeaderFields(fm_headerFields));
		}

		if (fm_requestType == Network::Protocol::httpMethod::HM_GET)
		{
			return answerCertificateRevocationListGet(fm_headerFields);
		}
		else if (fm_requestType == Network::Protocol::httpMethod::HM_POST)
		{
			return answerCertificateRevocationListPost(fm_requestContent, fm_requestType, fm_headerFields, fm_requestPath);
		}
		else
		{
			return std::make_tuple(std::string(), Stonepine::Network::Protocol::httpStatusCode::HSC_METHOD_NOT_ALLOWED, std::string("only GET and POST methods are allowed on this endpoint"), processHeaderFields(fm_headerFields));
		}
	}

	const std::tuple<std::string, Stonepine::Network::Protocol::httpStatusCode, std::string, std::map<std::string, std::string>> certificateStoreServer::answerOnlineCertificateStatusProtocol(const std::string& fm_peerCertificateSubjectCommonName, const std::string& fm_requestContent, const Network::Protocol::httpMethod& fm_requestType, const std::map<std::string, std::string>& fm_headerFields, const std::string& fm_requestPath) noexcept
	{
		if (m_httpsServer->isClientAuthorized(fm_peerCertificateSubjectCommonName) == true)
		{
			return std::make_tuple(std::string(), Stonepine::Network::Protocol::httpStatusCode::HSC_UNAUTHORIZED, std::string(), processHeaderFields(fm_headerFields));
		}

		std::string aC = {}, eD = {};

		Stonepine::Network::Protocol::httpStatusCode eC = Stonepine::Network::Protocol::httpStatusCode::HSC_NOT_IMPLEMENTED;

		std::map<std::string, std::string> hF = {};

		return std::make_tuple(aC, eC, eD, hF);
	}

	const std::tuple<std::string, Stonepine::Network::Protocol::httpStatusCode, std::string, std::map<std::string, std::string>> certificateStoreServer::answerExtendedCertificateRevocationList(const std::string& fm_peerCertificateSubjectCommonName, const std::string& fm_requestContent, const Network::Protocol::httpMethod& fm_requestType, const std::map<std::string, std::string>& fm_headerFields, const std::string& fm_requestPath) noexcept
	{
		if (m_httpsServer->isClientAuthorized(fm_peerCertificateSubjectCommonName) == true)
		{
			return std::make_tuple(std::string(), Stonepine::Network::Protocol::httpStatusCode::HSC_UNAUTHORIZED, std::string(), processHeaderFields(fm_headerFields));
		}

		std::string aC = {}, eD = {};

		Stonepine::Network::Protocol::httpStatusCode eC = Stonepine::Network::Protocol::httpStatusCode::HSC_NOT_IMPLEMENTED;

		std::map<std::string, std::string> hF = {};

		const auto sCN = Stonepine::Utilities::regex::extractRegexMatchAsString(fm_requestPath, "^\/certificateRevocationList\/");

		aC = Stonepine::Utilities::iO::readDataFromFile(sCN + ".crl.pem");

		if (aC.empty() != true)
		{
			eC = Stonepine::Network::Protocol::httpStatusCode::HSC_OK;
		}
		else
		{
			eC = Stonepine::Network::Protocol::httpStatusCode::HSC_NOT_FOUND;
		}

		return std::make_tuple(aC, eC, eD, hF);
	}

	const std::tuple<std::string, Stonepine::Network::Protocol::httpStatusCode, std::string, std::map<std::string, std::string>> certificateStoreServer::answerExtendedOnlineCertificateStatusProtocol(const std::string& fm_peerCertificateSubjectCommonName, const std::string& fm_requestContent, const Network::Protocol::httpMethod& fm_requestType, const std::map<std::string, std::string>& fm_headerFields, const std::string& fm_requestPath) noexcept
	{
		if (m_httpsServer->isClientAuthorized(fm_peerCertificateSubjectCommonName) == true)
		{
			return std::make_tuple(std::string(), Stonepine::Network::Protocol::httpStatusCode::HSC_UNAUTHORIZED, std::string(), processHeaderFields(fm_headerFields));
		}

		if (fm_requestType == Network::Protocol::httpMethod::HM_POST)
		{
			const auto sCN = Stonepine::Utilities::regex::extractRegexMatchAsString(fm_requestPath, "^\/onlineCertificateStatusProtocol\/");

			return answerExtendedOnlineCertificateStatusProtocolPost(fm_requestContent, fm_headerFields, sCN);
		}
		else
		{
			return std::make_tuple(std::string(), Stonepine::Network::Protocol::httpStatusCode::HSC_METHOD_NOT_ALLOWED, std::string("only POST method is allowed on this endpoint"), processHeaderFields(fm_headerFields));
		}
	}

	const std::tuple<std::string, Stonepine::Network::Protocol::httpStatusCode, std::string, std::map<std::string, std::string>> certificateStoreServer::answerVerifyCertificate(const std::string& fm_peerCertificateSubjectCommonName, const std::string& fm_requestContent, const Network::Protocol::httpMethod& fm_requestType, const std::map<std::string, std::string>& fm_headerFields, const std::string& fm_requestPath) noexcept
	{
		if (m_httpsServer->isClientAuthorized(fm_peerCertificateSubjectCommonName) == true)
		{
			return std::make_tuple(std::string(), Stonepine::Network::Protocol::httpStatusCode::HSC_UNAUTHORIZED, std::string(), processHeaderFields(fm_headerFields));
		}

		std::string aC = {}, eD = {};

		Stonepine::Network::Protocol::httpStatusCode eC = Stonepine::Network::Protocol::httpStatusCode::HSC_NOT_IMPLEMENTED;

		std::map<std::string, std::string> hF = {};

		if (fm_requestType == Network::Protocol::httpMethod::HM_POST)
		{
			return answerVerifyCertificatePost(fm_requestContent, fm_headerFields);
		}
		else
		{
			return std::make_tuple(std::string(), Stonepine::Network::Protocol::httpStatusCode::HSC_METHOD_NOT_ALLOWED, std::string("only POST methods is allowed on this endpoint"), processHeaderFields(fm_headerFields));
		}
	}

	const std::tuple<std::string, Stonepine::Network::Protocol::httpStatusCode, std::string, std::map<std::string, std::string>> certificateStoreServer::answerNotFoundPage(const std::string& fm_peerCertificateSubjectCommonName, const std::string& fm_requestContent, const Network::Protocol::httpMethod& fm_requestType, const std::map<std::string, std::string>& fm_headerFields, const std::string& fm_requestPath) noexcept
	{
		if (m_httpsServer->isClientAuthorized(fm_peerCertificateSubjectCommonName) == true)
		{
			return std::make_tuple(std::string(), Stonepine::Network::Protocol::httpStatusCode::HSC_UNAUTHORIZED, std::string(), processHeaderFields(fm_headerFields));
		}

		auto returnIfConcerned = [&](const std::tuple<std::string, Stonepine::Network::Protocol::httpStatusCode, std::string, std::map<std::string, std::string>> fa_answer)
		{
			if (std::get<1>(fa_answer) != Stonepine::Network::Protocol::httpStatusCode::HSC_NO_CONCERN)
			{
				return fa_answer;
			}
		};

		returnIfConcerned(answerExtendedCertificateAuthority(fm_peerCertificateSubjectCommonName, fm_requestContent, fm_requestType, fm_headerFields, fm_requestPath));
		returnIfConcerned(answerExtendedCertificateRequest(fm_peerCertificateSubjectCommonName, fm_requestContent, fm_requestType, fm_headerFields, fm_requestPath));
		returnIfConcerned(answerExtendedCertificate(fm_peerCertificateSubjectCommonName, fm_requestContent, fm_requestType, fm_headerFields, fm_requestPath));

		if (m_isCertificateRevocationListEnabled == true)
		{
			returnIfConcerned(answerExtendedCertificateRevocationList(fm_peerCertificateSubjectCommonName, fm_requestContent, fm_requestType, fm_headerFields, fm_requestPath));
		}

		if (m_isOnlineCertificateStatusProtocol == true)
		{
			returnIfConcerned(answerExtendedOnlineCertificateStatusProtocol(fm_peerCertificateSubjectCommonName, fm_requestContent, fm_requestType, fm_headerFields, fm_requestPath));
		}

		return std::make_tuple(std::string(), Stonepine::Network::Protocol::httpStatusCode::HSC_NOT_FOUND, std::string(), processHeaderFields(fm_headerFields));
	}

	const std::tuple<std::string, Stonepine::Network::Protocol::httpStatusCode, std::string, std::map<std::string, std::string>> certificateStoreServer::answerCertificateAuthorityChainGet(const std::map<std::string, std::string>& fm_headerFields) noexcept
	{
		std::string aC = {}, eD = {};

		Stonepine::Network::Protocol::httpStatusCode eC = Stonepine::Network::Protocol::httpStatusCode::HSC_BAD_REQUEST;

		std::map<std::string, std::string> hF = {};

		try
		{
			auto cSCT = std::make_shared<certificateStoreBinding::certificateStoreComplexType>();

			auto cS = certificateStoreBinding::certificatesComplexType();

			if (m_httpsClient != nullptr)
			{
				auto cCFMCS = getCertificateChainFromMasterCertificateStore();

				if (cCFMCS != nullptr)
				{
					for (auto &iTGCCFMCS : cCFMCS->certificate())
					{
						cS.certificate().push_back(iTGCCFMCS);
					}
				}
			}

			/*auto cC = generateCertificateAuthoritiesChain<std::unique_ptr<certificateStoreBinding::certificatesComplexType>>(Stonepine::utilities::extractFirstElements(m_certificateAuthoritiesSubjectCommonNamesAndPrivateKeysPasswords));

			if (cC != nullptr)
			{
				for (auto &iTCC : cC->certificate())
				{
					cS.certificate().push_back(iTCC);
				}
			}*/

			cSCT->certificates(cS);

			std::string a = certificateStoreBinding::certificateStoreUtilities::writeXmlData(cSCT);

			if (a.empty() == false)
			{
				hF.insert(std::make_pair("Content-Type", "text/xml; charset=UTF-8"));

				aC = a;
				eC = Stonepine::Network::Protocol::httpStatusCode::HSC_OK;
			}
		}
		catch (const std::exception& e)
		{
			eD = e.what();

			Stonepine::LOG_ERROR(e.what());
		}

		return std::make_tuple(aC, eC, eD, hF);
	}

	const std::tuple<std::string, Stonepine::Network::Protocol::httpStatusCode, std::string, std::map<std::string, std::string>> certificateStoreServer::answerCertificateRequestPost(const std::string& fm_requestContent, const Network::Protocol::httpMethod& fm_requestType, const std::map<std::string, std::string>& fm_headerFields, const std::string& fm_requestPath) noexcept
	{
		std::string aC = {}, eD = {};

		Stonepine::Network::Protocol::httpStatusCode eC = Stonepine::Network::Protocol::httpStatusCode::HSC_BAD_REQUEST;

		std::map<std::string, std::string> hF = {};

		auto r = certificateStoreBinding::certificateStoreUtilities::readXmlData(fm_requestContent);

		try
		{
			if (fm_requestContent.empty() == true)
			{
				throw std::runtime_error("empty request");
			}

			if (r == nullptr)
			{
				throw std::runtime_error("XML validation failed");
			}

			auto cSB = std::make_shared<certificateStoreBinding::certificateStoreComplexType>();

			if (r->certificateRequests().present() == true)
			{
				auto cRS = r->certificateRequests().get();

				auto cTS = certificateStoreBinding::certificatesComplexType();

				for (auto &iTCR : cRS.certificateRequest())
				{
					if (iTCR.issuerCommonName().present() != true)
					{
						throw std::runtime_error("issuer name is mandatory");
					}

					if (iTCR.x509CertificateType().present() != true)
					{
						throw std::runtime_error("X509 certificate type is mandatory");
					}

					/*if (iTCR.beginValidityDate().present() != true)
					{
					throw std::runtime_error("begin validity date is mandatory");
					}

					if (iTCR.endValidityDate().present() != true)
					{
					throw std::runtime_error("end validity date is mandatory");
					}*/

					if (iTCR.x509CertificateRequest().present() != true)
					{
						throw std::runtime_error("certificate request payload is mandatory");
					}

					auto iCN = iTCR.issuerCommonName().get();
					auto sCN = iTCR.subjectCommonName();

					auto iTP = m_certificateAuthoritiesSubjectCommonNamesAndPrivateKeysPasswords.find(iCN);

					if (iTP == m_certificateAuthoritiesSubjectCommonNamesAndPrivateKeysPasswords.end())
					{
						throw std::runtime_error("certificate authority " + iCN + " is not existing");
					}

					auto cT = certificateStoreBinding::certificateStoreUtilities::getCertificateType(iTCR.x509CertificateType().get());

					auto iCRAC = m_certificateStoreDatabase->addCertificateRequestAndGenerateACertificate(iCN, iTCR.x509CertificateRequest()->pemData().data(), sCN, cT, 3650, iTP->second);

					if (iCRAC.first < 1)
					{
						throw std::runtime_error("unable to persist the certificate request");
					}

					if (iCRAC.second < 1)
					{
						throw std::runtime_error("unable to persist the generated certificate");
					}

					std::shared_ptr<Security::Certificate::x509Certificate> c = { nullptr };

					if (Stonepine::Security::Certificate::x509CertificateTools::isCertificateAnAuthority(cT) == true)
					{
						c = m_certificateStoreDatabase->getCertificateAuthority(iCRAC.second);
					}
					else
					{
						c = m_certificateStoreDatabase->getCertificate(iCRAC.second);
					}

					if (c != nullptr)
					{
						auto cCT = certificateStoreBinding::certificateComplexType(iTCR.subjectCommonName());

						cCT.subjectSerialNumber(c->getSerialNumber());
						cCT.x509Certificate(x509CertificateBinding::x509CertificateUtilities::generateX509CertificateType(c->getAsPemData()));

						cTS.certificate().push_back(cCT);
					}
				}

				cSB->certificates(cTS);
			}
			else
			{
				throw std::runtime_error("unable to process certificate request");
			}

			std::string a = certificateStoreBinding::certificateStoreUtilities::writeXmlData(cSB);

			if (a.empty() == false)
			{
				hF.insert(std::make_pair("Content-Type", "text/xml; charset=UTF-8"));

				eC = Stonepine::Network::Protocol::httpStatusCode::HSC_OK;
				aC = a;
			}
		}
		catch (const std::exception& e)
		{
			eD = e.what();

			Stonepine::LOG_ERROR(e.what());
		}

		return std::make_tuple(aC, eC, eD, hF);
	}

	const std::tuple<std::string, Stonepine::Network::Protocol::httpStatusCode, std::string, std::map<std::string, std::string>> certificateStoreServer::answerCertificateRequestGet(const std::map<std::string, std::string>& fm_headerFields) noexcept
	{
		std::string aC = {}, eD = {};

		Stonepine::Network::Protocol::httpStatusCode eC = Stonepine::Network::Protocol::httpStatusCode::HSC_NOT_IMPLEMENTED;

		std::map<std::string, std::string> hF = {};

		auto sCNS = m_certificateStoreDatabase->getCertificateRequestSubjectCommonNames();

		if (sCNS->size() > 0)
		{
			auto cRCTS = certificateStoreBinding::certificateStoreUtilities::generateCertificateStoreTypeWithCertificateRequestsComplexType(sCNS);

			eC = Stonepine::Network::Protocol::httpStatusCode::HSC_OK;
			aC = certificateStoreBinding::certificateStoreUtilities::writeXmlData(cRCTS);
		}
		else
		{
			eC = Stonepine::Network::Protocol::httpStatusCode::HSC_NOT_FOUND;
		}

		return std::make_tuple(aC, eC, eD, hF);
	}

	const std::tuple<std::string, Stonepine::Network::Protocol::httpStatusCode, std::string, std::map<std::string, std::string>> certificateStoreServer::answerCertificateRevocationListGet(const std::map<std::string, std::string>& fm_headerFields) noexcept
	{
		std::string aC = {}, eD = {};

		Stonepine::Network::Protocol::httpStatusCode eC = Stonepine::Network::Protocol::httpStatusCode::HSC_BAD_REQUEST;

		std::map<std::string, std::string> hF = {};

		try
		{
			auto cSCT = std::make_shared<certificateStoreBinding::certificateStoreComplexType>();

			auto cRLSCT = certificateStoreBinding::certificateRevocationListsComplexType();

			if (m_httpsClient != nullptr)
			{
				auto cRLFMCS = getCertificateRevocationListFromMasterCertificateStore();

				if (cRLFMCS != nullptr)
				{
					for (auto &iTCRLFMCS : cRLFMCS->certificateRevocationList())
					{
						cRLSCT.certificateRevocationList().push_back(iTCRLFMCS);
					}
				}
			}

			/*auto cRL = generateCertificateRevocationListChain<std::unique_ptr<certificateStoreBinding::certificateRevocationListsComplexType>>(Stonepine::utilities::extractFirstElements(m_certificateAuthoritiesSubjectCommonNamesAndPrivateKeysPasswords));

			if (cRL != nullptr)
			{
				for (auto &iTCRL : cRL->certificateRevocationList())
				{
					cRLSCT.certificateRevocationList().push_back(iTCRL);
				}
			}*/

			cSCT->certificateRevocationLists(cRLSCT);

			std::string a = certificateStoreBinding::certificateStoreUtilities::writeXmlData(cSCT);

			if (a.empty() == false)
			{
				hF.insert(std::make_pair("Content-Type", "text/xml; charset=UTF-8"));

				aC = a;
				eC = Stonepine::Network::Protocol::httpStatusCode::HSC_OK;
			}
		}
		catch (const std::exception& e)
		{
			eD = e.what();

			Stonepine::LOG_ERROR(e.what());
		}

		return std::make_tuple(aC, eC, eD, hF);
	}

	const std::tuple<std::string, Stonepine::Network::Protocol::httpStatusCode, std::string, std::map<std::string, std::string>> certificateStoreServer::answerCertificateRevocationListPost(const std::string& fm_requestContent, const Network::Protocol::httpMethod& fm_requestType, const std::map<std::string, std::string>& fm_headerFields, const std::string& fm_requestPath) noexcept
	{
		std::string aC = {}, eD = {};

		Stonepine::Network::Protocol::httpStatusCode eC = Stonepine::Network::Protocol::httpStatusCode::HSC_BAD_REQUEST;

		std::map<std::string, std::string> hF = {};

		auto r = certificateStoreBinding::certificateStoreUtilities::readXmlData(fm_requestContent);

		try
		{
			if (fm_requestContent.empty() == true)
			{
				throw std::runtime_error("the request is empty");
			}

			if (r == nullptr)
			{
				throw std::runtime_error("XML validation failed");
			}

			auto cSB = std::make_shared<certificateStoreBinding::certificateStoreComplexType>();

			if (r->certificateRevocationLists().present() == true)
			{
				auto cRLS = r->certificateRevocationLists().get();

				for (auto &iTCRLS : cRLS.certificateRevocationList())
				{
					if (iTCRLS.issuerCommonName().present() != true)
					{
						throw std::runtime_error("issuer common name is mandatory");
					}

					if (iTCRLS.subjectSerialNumber().present() != true)
					{
						throw std::runtime_error("subject serial number is mandatory");
					}

					if (iTCRLS.revocationReason().present() != true)
					{
						throw std::runtime_error("revocation reason is mandatory");
					}

					if (m_certificateStoreDatabase->revokeCertificate(iTCRLS.subjectSerialNumber().get(), Stonepine::Security::Certificate::revocationReasons::RR_KEY_COMPROMISE) == false)
					{
						throw std::runtime_error("revocation procedure failed");
					}

					m_certificateStoreDatabase->updateCertificateRevocationList(iTCRLS.issuerCommonName().get(), m_certificateAuthoritiesSubjectCommonNamesAndPrivateKeysPasswords);
				}
			}
			else
			{
				throw std::runtime_error("no certificateRevocationLists node is present");
			}

			eC = Stonepine::Network::Protocol::httpStatusCode::HSC_OK;
		}
		catch (const std::exception& e)
		{
			eD = e.what();

			Stonepine::LOG_ERROR("unable to process to certificate revocation -> ", e.what());
		}

		return std::make_tuple(aC, eC, eD, hF);
	}

	const std::tuple<std::string, Stonepine::Network::Protocol::httpStatusCode, std::string, std::map<std::string, std::string>> certificateStoreServer::answerVerifyCertificatePost(const std::string& fm_request, const std::map<std::string, std::string>& fm_headerFields) noexcept
	{
		std::string aC = {}, eD = {};

		Stonepine::Network::Protocol::httpStatusCode eC = Stonepine::Network::Protocol::httpStatusCode::HSC_BAD_REQUEST;

		std::map<std::string, std::string> hF = {};

		auto r = certificateStoreBinding::certificateStoreUtilities::readXmlData(fm_request);

		try
		{
			if (fm_request.empty() == true)
			{
				throw std::runtime_error("empty request");
			}

			if (r == nullptr)
			{
				throw std::runtime_error("XML validation failed");
			}

			// Get certificate to valid
			auto xCC = std::make_shared<Stonepine::Security::Certificate::x509Certificate>();

			if (r->certificates().present() == true)
			{
				auto cS = r->certificates().get();

				for (auto &iTC : cS.certificate())
				{
					if (iTC.x509Certificate().present() != true)
					{
						throw std::runtime_error("certificate request payload is mandatory");
					}

					xCC->setX509CertificatePemData(iTC.x509Certificate()->pemData().data());
				}
			}

			// Get certificate authorities
			auto xCCAS = std::make_shared<std::vector<Stonepine::Security::Certificate::x509Certificate>>();

			// From master server
			if (m_httpsClient != nullptr)
			{
				auto cCFMCS = getCertificateChainFromMasterCertificateStore();

				if (cCFMCS != nullptr)
				{
					for (auto &iTGCCFMCS : cCFMCS->certificate())
					{
						if (iTGCCFMCS.x509Certificate().present() == true)
						{
							xCCAS->emplace_back(Stonepine::Security::Certificate::x509Certificate(iTGCCFMCS.x509Certificate()->pemData().data()));
						}
					}
				}
			}

			// From local server
			for (const auto &iTCASCNAPKPS : m_certificateAuthoritiesSubjectCommonNamesAndPrivateKeysPasswords)
			{
				const auto xCA = m_certificateStoreDatabase->getCertificateAuthority(iTCASCNAPKPS.first);

				if (xCA != nullptr)
				{
					xCCAS->emplace_back(*xCA.get());
				}
			}

			/*if (m_certificateAuthoritiesSubjectCommonNamesAndPrivateKeysPasswords.size() != xCCAS->size())
			{
				throw std::runtime_error("error while loading certificate authorities");
			}*/

			// Get certificate revocation lists
			auto xCCRLS = std::make_shared<std::vector<Stonepine::Security::Certificate::x509CertificateRevocationList>>();

			// From master server
			if (m_httpsClient != nullptr)
			{
				auto cRLFMCS = getCertificateRevocationListFromMasterCertificateStore();

				if (cRLFMCS != nullptr)
				{
					for (auto &iTCRLFMCS : cRLFMCS->certificateRevocationList())
					{
						if (iTCRLFMCS.x509CertificateRevocationList().present() == true)
						{
							xCCRLS->emplace_back(Stonepine::Security::Certificate::x509CertificateRevocationList(iTCRLFMCS.x509CertificateRevocationList()->pemData().data()));
						}
					}
				}
			}

			// From local server
			for (const auto &iTCASCNAPKPS : m_certificateAuthoritiesSubjectCommonNamesAndPrivateKeysPasswords)
			{
				auto xCCRL = Stonepine::Security::Certificate::x509CertificateRevocationList();

				if (xCCRL.read(iTCASCNAPKPS.first + ".crl.pem") != false)
				{
					xCCRLS->emplace_back(xCCRL);
				}
			}

			/*if (m_certificateAuthoritiesSubjectCommonNamesAndPrivateKeysPasswords.size() != xCCRLS->size())
			{
				throw std::runtime_error("error while loading certificate revocation lists");
			}*/

			eC = Stonepine::Network::Protocol::httpStatusCode::HSC_OK;

			if (Stonepine::Security::Certificate::x509CertificateTools::isCertificateValid(xCCAS, xCC, xCCRLS) == true)
			{
				aC = "VALID";
			}
			else
			{
				aC = "NOT VALID";
			}
		}
		catch (const std::exception& e)
		{
			eD = e.what();

			Stonepine::LOG_ERROR(e.what());
		}

		return std::make_tuple(aC, eC, eD, hF);
	}

	const std::tuple<std::string, Stonepine::Network::Protocol::httpStatusCode, std::string, std::map<std::string, std::string>> certificateStoreServer::answerExtendedOnlineCertificateStatusProtocolPost(const std::string& fm_request, const std::map<std::string, std::string>& fm_headerFields, const std::string& fm_regexMatchedData) noexcept
	{
		std::string aC = {}, eD = {};

		Stonepine::Network::Protocol::httpStatusCode eC = Stonepine::Network::Protocol::httpStatusCode::HSC_NOT_IMPLEMENTED;

		std::map<std::string, std::string> hF = {};

		try
		{
			if (fm_request.empty() == true)
			{
				throw std::runtime_error("empty request");
			}

			if (m_ocspSubjectCommonNamesAndPrivateKeysPasswords.find(fm_regexMatchedData + "-ocsp") != m_ocspSubjectCommonNamesAndPrivateKeysPasswords.end())
			{
				aC = m_certificateStoreDatabase->generateOcspResponse(fm_regexMatchedData, fm_request, m_ocspSubjectCommonNamesAndPrivateKeysPasswords[fm_regexMatchedData + "-ocsp"]);

				if (aC.empty() != true)
				{
					eC = Stonepine::Network::Protocol::httpStatusCode::HSC_OK;

					hF.insert(std::make_pair("Content-type", "application/ocsp-response; charset=UTF-8"));
					hF.insert(std::make_pair("Connection", "close"));
				}
			}
			else
			{
				eC = Stonepine::Network::Protocol::httpStatusCode::HSC_INTERNAL_SERVER_ERROR;
			}
		}
		catch (const std::exception& e)
		{
			eC = Stonepine::Network::Protocol::httpStatusCode::HSC_BAD_REQUEST;
			eD = e.what();

			Stonepine::LOG_ERROR(e.what());
		}

		return std::make_tuple(aC, eC, eD, hF);
	}

	certificateStoreServer::certificateStoreServer(void) noexcept
	{
		m_httpsServer = { nullptr };
		m_certificateStoreDatabase = { nullptr };
		m_httpsClient = { nullptr };

		m_isDatabaseInitilizationEnabled = { false };

		m_isDefalutCreatedCertificatesDumpEnabled = { false };

		m_isCertificateRevocationListEnabled = { true };
		m_isOnlineCertificateStatusProtocol = { false };

		m_xCommonNameRegex = { "([a-z0-9\-\.]{1,64})$" };

		m_certificateAuthoritiesSubjectCommonNamesAndPrivateKeysPasswords = {};
		m_certificateAuthoritiesSubjectCommonNamesAndDatabaseIndex = {};

		m_ocspSubjectCommonNamesAndPrivateKeysPasswords = {};
		m_ocspSubjectCommonNamesAndDatabaseIndex = {};

		m_certificateAuthoritiesSubjectCommonNamesAndOcspSubjectCommonNames = {};
	}

	certificateStoreServer::~certificateStoreServer(void)
	{

	}

	const void certificateStoreServer::setDatabaseInitialization(const bool fm_isDatabaseInitilizationEnabled) noexcept
	{
		m_isDatabaseInitilizationEnabled = fm_isDatabaseInitilizationEnabled;
	}

	const void certificateStoreServer::setDefaultCreatedCertificatesDump(const bool fm_isDefalutCreatedCertificatesDumpEnabled) noexcept
	{
		m_isDefalutCreatedCertificatesDumpEnabled = fm_isDefalutCreatedCertificatesDumpEnabled;
	}

	const void certificateStoreServer::forceClientAnthentication(void) noexcept
	{
		if (m_httpsServer->getClientCertificateAuthorityFilePath().empty() == true)
		{
			Stonepine::LOG_ERROR("[utilities.xml.server] client authentication is enabled, but option is not present in the configuration file");
			Stonepine::LOG_INFO("[utilities.xml.server] exiting...");

			exitProperly(EXIT_FAILURE);
		}

		m_httpsServer->setClientAuthentication(true);
	}

	const void certificateStoreServer::configureCertificateStore(void) noexcept
	{
		if (m_isDatabaseInitilizationEnabled == true)
		{
			if (m_certificateStoreDatabase->initializeDatabase() == true)
			{
				Stonepine::LOG_INFO("[certificateStoreServer] ", m_httpsServer->getServerName(), " certificate store database initialization...");
			}
			else
			{
				Stonepine::LOG_INFO("[certificateStoreServer] exiting...");
			}
		}

		generateOrLoadDefaultCertificateAuthoritiesAndCertificates();

		configureEndPoints();
	}

	const void certificateStoreServer::launchServer(void) const noexcept
	{
		m_httpsServer->launchServer();
	}
}
