/*
Télécom SudParis
2017, February
Pierre-Olivier Rocher
*/

#include <stonepine/utilities/utilities.h>
#include <stonepine/utilities/stonepine.utilities.iO.h>

#include "certificateStoreServerFromXml.h"

#include <stonepine/network/client/httpsClientFromXml.h>

certificateStoreServerFromXml::certificateStoreServerFromXml(void) noexcept
{
	m_certificateStoreConfiguration = { nullptr };

	m_dumpCreatedCertificates = { false };
}

const void certificateStoreServerFromXml::configureCertificateStoreServer(void) noexcept
{
	try
	{
		if (m_certificateStoreConfiguration == nullptr)
		{
			throw std::runtime_error("");
		}

		m_certificateStoreServerName = m_certificateStoreConfiguration->name();

		setHttpsServerConfiguration(m_certificateStoreConfiguration->serverConfiguration());

		if (isClientAuthenticationEnabled() == true)
		{
			m_isClientAuthenticationEnabled = true;
		}

		if (isClientAuthorizationEnabled() == true)
		{
			m_isClientAuthorizationEnabled = true;

			m_authorizedClientCertificatesCommoneNames = getAuthorizedClientCertificatesCommoneNames();
		}

		setDatabaseConfiguration(httpsServerFromXml::getDatabaseConfiguration({}));

		setStaticInstance(odbDatabase::getThisInstance());

		if (m_isDatabaseInitilizationEnabled == true)
		{
			if (initializeDatabase() == false)
			{
				throw std::runtime_error("");
			}
		}

		if (m_certificateStoreConfiguration->masterServerHttpsClient().present() == true)
		{
			setHttpsClientConfiguration(m_certificateStoreConfiguration->masterServerHttpsClient().get());
		}

		generateOrLoadDefaultCertificateAuthoritiesAndCertificates();

		configureEndPoints();
	}
	catch (const std::exception&)
	{
		Stonepine::LOG_ERROR("unable to configure ", m_certificateStoreServerName, " certificate store server options");
		Stonepine::LOG_ERROR("exiting...");

		exitProperly(EXIT_FAILURE);
	}
}

const void certificateStoreServerFromXml::generateDefaultCertificateAuthorities(const certificateStoreConfigBinding::defaultCertificateAuthorityComplexType& fm_defaultCertificateAuthority) noexcept (false)
{
	checkIssuerAndSubjectCommonNamesAndSubjectCertificateType(fm_defaultCertificateAuthority);

	if (fm_defaultCertificateAuthority.subject().privateKeyPassword().present() == false)
	{
		throw std::runtime_error("private key password is mandatory for certificate authorities");
	}

	if (fm_defaultCertificateAuthority.subject().commonName().get() == fm_defaultCertificateAuthority.issuer().commonName().get())
	{
		generateDefaultSelfSignedCertificateAuthorities(fm_defaultCertificateAuthority);
	}
	else
	{
		if (m_httpsClientConfiguration == nullptr)
		{
			if (m_certificateAuthoritiesSubjectCommonNamesAndPrivateKeysPasswords.find(fm_defaultCertificateAuthority.issuer().commonName().get()) != m_certificateAuthoritiesSubjectCommonNamesAndPrivateKeysPasswords.end())
			{
				generateDefaultOtherCertificateAuthorities(fm_defaultCertificateAuthority);
			}
			else
			{
				throw std::runtime_error("unable to find \"" + fm_defaultCertificateAuthority.issuer().commonName().get() + "\" certificate authority");
			}
		}
		else
		{
			if (m_certificateAuthoritiesSubjectCommonNamesAndPrivateKeysPasswords.find(fm_defaultCertificateAuthority.issuer().commonName().get()) != m_certificateAuthoritiesSubjectCommonNamesAndPrivateKeysPasswords.end())
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

const void certificateStoreServerFromXml::generateDefaultCertificates(const certificateStoreConfigBinding::defaultCertificateComplexType & fm_defaultCertificate) noexcept (false)
{
	checkIssuerAndSubjectCommonNamesAndSubjectCertificateType(fm_defaultCertificate);

	if (fm_defaultCertificate.subject().commonName().get() == fm_defaultCertificate.issuer().commonName().get())
	{
		throw std::runtime_error("unable to generate self signed certificate");
	}
	else
	{
		if (m_httpsClientConfiguration == nullptr)
		{
			if (m_certificateAuthoritiesSubjectCommonNamesAndPrivateKeysPasswords.find(fm_defaultCertificate.issuer().commonName().get()) != m_certificateAuthoritiesSubjectCommonNamesAndPrivateKeysPasswords.end())
			{
				generateDefaultOtherCertificates(fm_defaultCertificate);
			}
			else
			{
				throw std::runtime_error("unable to find \"" + fm_defaultCertificate.issuer().commonName().get() + "\" certificate authority");
			}
		}
		else
		{
			if (m_certificateAuthoritiesSubjectCommonNamesAndPrivateKeysPasswords.find(fm_defaultCertificate.issuer().commonName().get()) != m_certificateAuthoritiesSubjectCommonNamesAndPrivateKeysPasswords.end())
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

const void certificateStoreServerFromXml::generateDefaultSelfSignedCertificateAuthorities(const certificateStoreConfigBinding::defaultCertificateAuthorityComplexType & fm_defaultCertificateAuthority) noexcept (false)
{
	// Sanity check
	if (fm_defaultCertificateAuthority.subject().certificateType().get() != certificateStoreConfigBinding::certificateTypeSimpleType::X509_SELF_SIGNED_CA)
	{
		throw std::runtime_error("incoherent configuration");
	}

	const auto xCCE = getCertificateExtensionsToAdd(fm_defaultCertificateAuthority);

	const auto sCN = fm_defaultCertificateAuthority.subject().commonName().get();
	const auto pKP = fm_defaultCertificateAuthority.subject().privateKeyPassword().get();

	const auto iSSCA = generateAndAddSelfSignedCertificateAuthorityCertificate(sCN, fm_defaultCertificateAuthority.subject().privateKeySize().get(), fm_defaultCertificateAuthority.subject().numberOfValidDaysFromNow().get(), pKP, xCCE);

	if (iSSCA > 0)
	{
		m_certificateAuthoritiesSubjectCommonNamesAndPrivateKeysPasswords.insert(std::make_pair(sCN, pKP));
		m_certificateAuthoritiesSubjectCommonNamesAndDatabaseIndex.insert(std::make_pair(sCN, iSSCA));
	}
	else
	{
		throw std::runtime_error("");
	}

	if (m_dumpCreatedCertificates == true)
	{
		writeCertificateAuthority({}, sCN);
	}
}

const void certificateStoreServerFromXml::generateDefaultOtherCertificateAuthorities(const certificateStoreConfigBinding::defaultCertificateAuthorityComplexType& fm_defaultCertificateAuthority) noexcept (false)
{
	const auto cT = fm_defaultCertificateAuthority.subject().certificateType().get();

	if (cT == certificateStoreConfigBinding::certificateTypeSimpleType::X509_CA || cT == certificateStoreConfigBinding::certificateTypeSimpleType::X509_INTERMEDIATE_CA)
	{
		const auto iCN = fm_defaultCertificateAuthority.issuer().commonName().get();
		const auto sCN = fm_defaultCertificateAuthority.subject().commonName().get();
		const auto pKP = fm_defaultCertificateAuthority.subject().privateKeyPassword().get();

		// Create and add the CA certificate request in the database
		const auto iTICAR = generateAndAddCertificateRequest(sCN, fm_defaultCertificateAuthority.subject().privateKeySize().get(), pKP);

		if (iTICAR > 0)
		{
			const auto xCCE = getCertificateExtensionsToAdd(fm_defaultCertificateAuthority);

			const auto iICA = generateAndAddCertificate(m_certificateAuthoritiesSubjectCommonNamesAndDatabaseIndex[iCN], iTICAR, certificateStoreBinding::certificateStoreUtilities::getCertificateType(cT), fm_defaultCertificateAuthority.subject().numberOfValidDaysFromNow().get(), m_certificateAuthoritiesSubjectCommonNamesAndPrivateKeysPasswords[iCN], xCCE);

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

		if (m_dumpCreatedCertificates == true)
		{
			writeCertificateAuthority({}, sCN);
		}
	}
}

const void certificateStoreServerFromXml::generateDefaultOtherCertificates(const certificateStoreConfigBinding::defaultCertificateComplexType & fm_defaultCertificate) noexcept (false)
{
	const auto cT = fm_defaultCertificate.subject().certificateType().get();

	if (cT == certificateStoreConfigBinding::certificateTypeSimpleType::X509_CLIENT || cT == certificateStoreConfigBinding::certificateTypeSimpleType::X509_SERVER || cT == certificateStoreConfigBinding::certificateTypeSimpleType::X509_OCSP_SIGNING)
	{
		const auto xCCE = getCertificateExtensionsToAdd(fm_defaultCertificate);

		const auto iCN = fm_defaultCertificate.issuer().commonName().get();
		const auto sCN = fm_defaultCertificate.subject().commonName().get();

		std::string pKP = {};

		if (fm_defaultCertificate.subject().privateKeyPassword().present() == true)
		{
			pKP = fm_defaultCertificate.subject().privateKeyPassword().get();
		}

		const auto iCRAC = generateAndAddCertificateRequestAndGenerateAndAddCertificate(iCN, sCN, certificateStoreBinding::certificateStoreUtilities::getCertificateType(cT), fm_defaultCertificate.subject().privateKeySize().get(), fm_defaultCertificate.subject().numberOfValidDaysFromNow().get(), m_certificateAuthoritiesSubjectCommonNamesAndPrivateKeysPasswords[iCN], pKP, xCCE);

		if (!(iCRAC.first > 0 && iCRAC.second > 0))
		{
			throw std::runtime_error("");
		}

		if (cT == certificateStoreConfigBinding::certificateTypeSimpleType::X509_OCSP_SIGNING)
		{
			m_ocspSubjectCommonNamesAndPrivateKeysPasswords.insert(std::make_pair(sCN, pKP));
			m_ocspSubjectCommonNamesAndDatabaseIndex.insert(std::make_pair(sCN, iCRAC.second));

			m_certificateAuthoritiesSubjectCommonNamesAndOcspSubjectCommonNames.insert(std::make_pair(iCN, sCN));
		}

		if (cT != certificateStoreConfigBinding::certificateTypeSimpleType::X509_OCSP_SIGNING && m_dumpCreatedCertificates == true)
		{
			writeCertificateAndCertificateRequestAndPrivateKey({}, sCN);
		}
	}
}

const std::shared_ptr<certificateStoreBinding::certificateStoreComplexType> certificateStoreServerFromXml::postToMasterCertificateStoreCertificateRequestEndPoint(const std::string& fm_subjectCommonName, const std::string& fm_issuerCommonName, const certificateStoreBinding::certificateTypeSimpleType & fm_certificateType, const std::string & fm_certificateRequestPemData) noexcept
{
	std::shared_ptr<certificateStoreBinding::certificateStoreComplexType> r = { nullptr };

	if (m_httpsClientConfiguration != nullptr)
	{
		const auto hCFX = std::make_unique<Stonepine::Network::httpsClientFromXml>(m_httpsClientConfiguration);

		auto hF = std::vector<std::pair<std::string, std::string>>();

		hF.emplace_back(std::make_pair("Content-Type", "text/xml; charset=UTF-8"));
		hF.emplace_back(std::make_pair("Accept", "text/xml; charset=UTF-8"));
		hF.emplace_back(std::make_pair("Connection", "close"));

		hCFX->setCommandToSend("POST");
		hCFX->setHeaderFieldsToSend(hF);
		hCFX->setServerEndPoint("/certificateRequest");

		const auto cSCT = certificateStoreBinding::certificateStoreUtilities::generateCertificateStoreTypeWithACertificateRequestType(fm_subjectCommonName, fm_issuerCommonName, fm_certificateType, fm_certificateRequestPemData);

		const std::string a = certificateStoreBinding::certificateStoreUtilities::writeXmlData(cSCT);

		hCFX->setDataToSend(a);

		if (a.empty() == false)
		{
			if (hCFX->sendPacket() == true)
			{
				auto rD = hCFX->getResponseData();
				auto rC = hCFX->getResponseCode();

				if (rD.empty() == false && rC == 200)
				{
					r = certificateStoreBinding::certificateStoreUtilities::readXmlData(rD);
				}
			}
		}
	}

	return r;
}

const void certificateStoreServerFromXml::loadCertificateAuthorities(const certificateStoreConfigBinding::defaultCertificateAuthorityComplexType& fm_defaultCertificateAuthority) noexcept (false)
{
	checkIssuerAndSubjectCommonNamesAndSubjectCertificateType(fm_defaultCertificateAuthority);

	if (Stonepine::Security::Certificate::x509CertificateTools::isCertificateAnAuthority(certificateStoreBinding::certificateStoreUtilities::getCertificateType(fm_defaultCertificateAuthority.subject().certificateType().get())) == false)
	{
		throw std::runtime_error("incoherent configuration");
	}

	auto sCN = fm_defaultCertificateAuthority.subject().commonName().get();

	auto iSSCA = getCertificateAuthorityId(sCN);
	
	if (iSSCA > 0)
	{
		m_certificateAuthoritiesSubjectCommonNamesAndPrivateKeysPasswords.insert(std::make_pair(sCN, fm_defaultCertificateAuthority.subject().privateKeyPassword().get()));
		m_certificateAuthoritiesSubjectCommonNamesAndDatabaseIndex.insert(std::make_pair(sCN, iSSCA));

		Stonepine::LOG_INFO("\"" + sCN + "\" certificate auhtority has been successfully loaded...");
	}
	else
	{
		throw std::runtime_error("");
	}
}

const void certificateStoreServerFromXml::loadOCSPSigningCertificates(const certificateStoreConfigBinding::defaultCertificateComplexType& fm_defaultCertificate) noexcept (false)
{
	checkIssuerAndSubjectCommonNamesAndSubjectCertificateType(fm_defaultCertificate);

	if (fm_defaultCertificate.subject().certificateType().get() == certificateStoreConfigBinding::certificateTypeSimpleType::X509_OCSP_SIGNING)
	{
		auto sCN = fm_defaultCertificate.subject().commonName().get();

		auto iIC = getCertificateId(sCN);
		auto c = getCertificate(sCN);

		if (iIC > 0 && c != nullptr)
		{
			m_ocspSubjectCommonNamesAndPrivateKeysPasswords.insert(std::make_pair(sCN, fm_defaultCertificate.subject().privateKeyPassword().get()));
			m_ocspSubjectCommonNamesAndDatabaseIndex.insert(std::make_pair(sCN, iIC));

			m_certificateAuthoritiesSubjectCommonNamesAndOcspSubjectCommonNames.insert(std::make_pair(c->getIssuerCommonName(), sCN));

			Stonepine::LOG_INFO("\"" + sCN + "\" OCSP signing certificate as been successfully loaded...");
		}
		else
		{
			throw std::runtime_error("");
		}
	}
}

void certificateStoreServerFromXml::setCertificateStoreConfiguration(const std::shared_ptr<certificateStoreConfigBinding::certificateStoreConfigurationComplexType> fm_certificateStoreConfiguration) noexcept
{
	m_certificateStoreConfiguration = fm_certificateStoreConfiguration;

	configureCertificateStoreServer();
}

const void certificateStoreServerFromXml::generateOrLoadDefaultCertificateAuthoritiesAndCertificates(void) noexcept (false)
{
	if (m_certificateStoreConfiguration->defaultCertificateAuthorities().present() == true)
	{
		const auto dCAS = m_certificateStoreConfiguration->defaultCertificateAuthorities().get();

		for (auto &iTDCA : dCAS.defaultCertificateAuthority())
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
	}

	if (m_dumpCreatedCertificates == true)
	{
		std::string fCC = {};

		if (m_httpsClientConfiguration != nullptr)
		{
			auto cC = getCertificateChainFromMasterCertificateStore();

			fCC.append(certificateStoreBinding::certificateStoreUtilities::certificatesComplexTypeToString(cC).first);
		}

		fCC.append(generateCertificateAuthoritiesChain<std::string>(Stonepine::utilities::extractFirstElements(m_certificateAuthoritiesSubjectCommonNamesAndDatabaseIndex)));

		if (m_certificateAuthoritiesSubjectCommonNamesAndDatabaseIndex.empty() == false)
		{
			Stonepine::Utilities::iO::writeBinaryDataToFile(fCC, m_certificateAuthoritiesSubjectCommonNamesAndDatabaseIndex.rbegin()->first + "-chain.cert.pem");
		}
	}

	if (m_certificateStoreConfiguration->defaultCertificates().present() == true)
	{
		const auto dCS = m_certificateStoreConfiguration->defaultCertificates().get();

		for (auto &iTDC : dCS.defaultCertificate())
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
	}

	if (m_ocspSubjectCommonNamesAndPrivateKeysPasswords.empty() == true || m_ocspSubjectCommonNamesAndDatabaseIndex.empty() == true || m_certificateAuthoritiesSubjectCommonNamesAndOcspSubjectCommonNames.empty() == true)
	{
		Stonepine::LOG_INFO("no OCSP informations in the XML configuration file, only certificateRevocationList endpoint will be available");
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

	updateCertificateRevocationLists(m_certificateAuthoritiesSubjectCommonNamesAndPrivateKeysPasswords);
}

const void certificateStoreServerFromXml::exitProperly(const bool fm_exitStatus) const noexcept
{
	certificateStoreServerFromXml::~certificateStoreServerFromXml();

	exit(fm_exitStatus);
}

certificateStoreServerFromXml::certificateStoreServerFromXml(const std::string& fm_xmlConfigurationFileFilePath, const bool fm_initializeDatabase, const bool fm_dumpCreatedCertificates) noexcept : certificateStoreConfigUtilities(fm_xmlConfigurationFileFilePath, true), certificateStoreServer()
{
	m_isDatabaseInitilizationEnabled = fm_initializeDatabase;
	m_dumpCreatedCertificates = fm_dumpCreatedCertificates;

	setCertificateStoreConfiguration(certificateStoreConfigBinding::certificateStoreConfigUtilities::readXmlFile());
}

certificateStoreServerFromXml::~certificateStoreServerFromXml(void)
{

}
 
