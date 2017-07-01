/*
Télécom SudParis
2017, February
Pierre-Olivier Rocher
*/

#pragma once

#ifdef _WIN32
#ifdef CERTIFICATESTORESERVER_EXPORTS
#define CERTIFICATESTORESERVER_API __declspec(dllexport)
#else
#define CERTIFICATESTORESERVER_API __declspec(dllimport)
#endif
#else
#define CERTIFICATESTORESERVER_API
#endif

#include <mutex>

#include <codeSynthesis/certificateStoreBinding/certificateStoreUtilities.h>

#include <stonepine/libraries/openssl.h>
#include <stonepine/libraries/xercesc.h>

#include <stonepine/network/network.h>

#include <stonepine/network/server/httpsServer.h>
#include <stonepine/network/client/httpsClient.h>

#include <stonepine/utilities/xml/certificates.h>

#include <stonepine/database/certificateStoreDatabase/certificateStoreDatabase.h>

namespace Stonepine
{
	class certificateStoreServer : network, Libraries::openssl
	{

	protected:

		// Regex rules
		std::string m_xCommonNameRegex;

		// Server
		std::shared_ptr<Network::httpsServer> m_httpsServer;

		// Database
		std::shared_ptr<Database::certificateStoreDatabase> m_certificateStoreDatabase;

		// Client
		std::shared_ptr<Network::httpsClient> m_httpsClient;

		// Initial certificate store configuration
		Utilities::certificates m_defaultCertificateAuthorities;
		Utilities::certificates m_defaultCertificates;

	protected:

		// Certificate store details
		std::string m_certificateStoreServerName;

		bool m_isCertificateRevocationListEnabled;
		bool m_isOnlineCertificateStatusProtocol;

		bool m_isDatabaseInitilizationEnabled;
		bool m_isDefalutCreatedCertificatesDumpEnabled;

		std::shared_ptr<std::vector<std::string>> m_authorizedClientCertificatesCommoneNames;

		// Certificate authorities names and private key passwords
		std::map<std::string, std::string> m_certificateAuthoritiesSubjectCommonNamesAndPrivateKeysPasswords;

		// Certificate authorities names and database index
		std::map<std::string, int> m_certificateAuthoritiesSubjectCommonNamesAndDatabaseIndex;

		// OCSP Certificates names and private key passwords
		std::map<std::string, std::string> m_ocspSubjectCommonNamesAndPrivateKeysPasswords;

		// OCSP Certificate names and database index
		std::map<std::string, int> m_ocspSubjectCommonNamesAndDatabaseIndex;

		// Relation between CAs and OSCP certificates
		std::map<std::string, std::string> m_certificateAuthoritiesSubjectCommonNamesAndOcspSubjectCommonNames;

	private:

		// Proper exit
		const void exitProperly(const bool fm_exitStatus) const noexcept;

		const std::shared_ptr<certificateStoreBinding::certificateStoreComplexType> getXFromMasterCertificateStore(const std::string& fm_endPoint) noexcept;

		const std::shared_ptr<certificateStoreBinding::certificateRevocationListsComplexType> getCertificateRevocationListFromMasterCertificateStore(void) noexcept;

		const void updateCertificateExtensions(const std::shared_ptr<Utilities::certificate> fm_defaultCertificateAuthorityOrCertificate) noexcept (false);

		const void checkIssuerAndSubjectCommonNamesAndSubjectCertificateType(const std::shared_ptr<Utilities::certificate> fm_defaultCertificateAuthorityOrCertificate) noexcept (false);

		const void generateDefaultCertificateAuthorities(const std::shared_ptr<Utilities::certificate> fm_defaultCertificateAuthority) noexcept (false);
		const void generateDefaultSelfSignedCertificateAuthorities(const std::shared_ptr<Utilities::certificate> fm_defaultCertificateAuthority) noexcept (false);
		const void generateDefaultOtherCertificateAuthorities(const std::shared_ptr<Utilities::certificate> fm_defaultCertificateAuthority) noexcept (false);

		const void generateDefaultCertificates(const std::shared_ptr<Utilities::certificate> fm_defaultCertificate) noexcept (false);
		const void generateDefaultOtherCertificates(const std::shared_ptr<Utilities::certificate> fm_defaultCertificate) noexcept (false);

		const void loadCertificateAuthorities(const std::shared_ptr<Utilities::certificate> fm_defaultCertificateAuthority) noexcept (false);
		const void loadOCSPSigningCertificates(const std::shared_ptr<Utilities::certificate> fm_defaultCertificate) noexcept (false);

		const std::shared_ptr<certificateStoreBinding::certificateStoreComplexType> postToMasterCertificateStoreCertificateRequestEndPoint(const std::string& fm_subjectCommonName, const std::string& fm_issuerCommonName, const Utilities::certificateType& fm_certificateType, const std::string& fm_certificateRequestPemData) noexcept;

		const void addCertificateFromMasterCertificateStore(const std::shared_ptr<Utilities::certificate> fm_defalutCertificateAuthorityOrCertificate) noexcept (false);

	protected:

		CERTIFICATESTORESERVER_API const std::shared_ptr<certificateStoreBinding::certificatesComplexType> getCertificateChainFromMasterCertificateStore(void) noexcept;

		CERTIFICATESTORESERVER_API const std::map<std::string, std::string> processHeaderFields(const std::map<std::string, std::string>& fm_inputHeaderFields) const noexcept;

		CERTIFICATESTORESERVER_API const void generateOrLoadDefaultCertificateAuthoritiesAndCertificates(void) noexcept (false);

	private:

		const std::tuple<std::string, Network::Protocol::httpStatusCode, std::string, std::map<std::string, std::string>> answerCertificateAuthority(const std::string& fm_peerCertificateSubjectCommonName, const std::string& fm_requestContent, const Network::Protocol::httpMethod& fm_requestType, const std::map<std::string, std::string>& fm_headerFields, const std::string& fm_requestPath) noexcept;
		const std::tuple<std::string, Network::Protocol::httpStatusCode, std::string, std::map<std::string, std::string>> answerCertificateRequest(const std::string& fm_peerCertificateSubjectCommonName, const std::string& fm_requestContent, const Network::Protocol::httpMethod& fm_requestType, const std::map<std::string, std::string>& fm_headerFields, const std::string& fm_requestPath) noexcept;
		const std::tuple<std::string, Network::Protocol::httpStatusCode, std::string, std::map<std::string, std::string>> answerCertificate(const std::string& fm_peerCertificateSubjectCommonName, const std::string& fm_requestContent, const Network::Protocol::httpMethod& fm_requestType, const std::map<std::string, std::string>& fm_headerFields, const std::string& fm_requestPath) noexcept;

		const std::tuple<std::string, Network::Protocol::httpStatusCode, std::string, std::map<std::string, std::string>> answerExtendedCertificateAuthority(const std::string& fm_peerCertificateSubjectCommonName, const std::string& fm_requestContent, const Network::Protocol::httpMethod& fm_requestType, const std::map<std::string, std::string>& fm_headerFields, const std::string& fm_requestPath) noexcept;
		const std::tuple<std::string, Network::Protocol::httpStatusCode, std::string, std::map<std::string, std::string>> answerExtendedCertificateRequest(const std::string& fm_peerCertificateSubjectCommonName, const std::string& fm_requestContent, const Network::Protocol::httpMethod& fm_requestType, const std::map<std::string, std::string>& fm_headerFields, const std::string& fm_requestPath) noexcept;
		const std::tuple<std::string, Network::Protocol::httpStatusCode, std::string, std::map<std::string, std::string>> answerExtendedCertificate(const std::string& fm_peerCertificateSubjectCommonName, const std::string& fm_requestContent, const Network::Protocol::httpMethod& fm_requestType, const std::map<std::string, std::string>& fm_headerFields, const std::string& fm_requestPath) noexcept;

		const std::tuple<std::string, Network::Protocol::httpStatusCode, std::string, std::map<std::string, std::string>> answerCertificateAuthorityChain(const std::string& fm_peerCertificateSubjectCommonName, const std::string& fm_requestContent, const Network::Protocol::httpMethod& fm_requestType, const std::map<std::string, std::string>& fm_headerFields, const std::string& fm_requestPath) noexcept;

		const std::tuple<std::string, Network::Protocol::httpStatusCode, std::string, std::map<std::string, std::string>> answerCertificateRevocationList(const std::string& fm_peerCertificateSubjectCommonName, const std::string& fm_requestContent, const Network::Protocol::httpMethod& fm_requestType, const std::map<std::string, std::string>& fm_headerFields, const std::string& fm_requestPath) noexcept;
		const std::tuple<std::string, Network::Protocol::httpStatusCode, std::string, std::map<std::string, std::string>> answerOnlineCertificateStatusProtocol(const std::string& fm_peerCertificateSubjectCommonName, const std::string& fm_requestContent, const Network::Protocol::httpMethod& fm_requestType, const std::map<std::string, std::string>& fm_headerFields, const std::string& fm_requestPath) noexcept;

		const std::tuple<std::string, Network::Protocol::httpStatusCode, std::string, std::map<std::string, std::string>> answerExtendedCertificateRevocationList(const std::string& fm_peerCertificateSubjectCommonName, const std::string& fm_requestContent, const Network::Protocol::httpMethod& fm_requestType, const std::map<std::string, std::string>& fm_headerFields, const std::string& fm_requestPath) noexcept;
		const std::tuple<std::string, Network::Protocol::httpStatusCode, std::string, std::map<std::string, std::string>> answerExtendedOnlineCertificateStatusProtocol(const std::string& fm_peerCertificateSubjectCommonName, const std::string& fm_requestContent, const Network::Protocol::httpMethod& fm_requestType, const std::map<std::string, std::string>& fm_headerFields, const std::string& fm_requestPath) noexcept;

		const std::tuple<std::string, Network::Protocol::httpStatusCode, std::string, std::map<std::string, std::string>> answerVerifyCertificate(const std::string& fm_peerCertificateSubjectCommonName, const std::string& fm_requestContent, const Network::Protocol::httpMethod& fm_requestType, const std::map<std::string, std::string>& fm_headerFields, const std::string& fm_requestPath) noexcept;

		const std::tuple<std::string, Network::Protocol::httpStatusCode, std::string, std::map<std::string, std::string>> answerNotFoundPage(const std::string& fm_peerCertificateSubjectCommonName, const std::string& fm_requestContent, const Network::Protocol::httpMethod& fm_requestType, const std::map<std::string, std::string>& fm_headerFields, const std::string& fm_requestPath) noexcept;

	private:

		const std::tuple<std::string, Network::Protocol::httpStatusCode, std::string, std::map<std::string, std::string>> answerCertificateAuthorityChainGet(const std::map<std::string, std::string>& fm_headerFields) noexcept;

		const std::tuple<std::string, Network::Protocol::httpStatusCode, std::string, std::map<std::string, std::string>> answerCertificateRequestGet(const std::map<std::string, std::string>& fm_headerFields) noexcept;
		const std::tuple<std::string, Network::Protocol::httpStatusCode, std::string, std::map<std::string, std::string>> answerCertificateRequestPost(const std::string& fm_requestContent, const Network::Protocol::httpMethod& fm_requestType, const std::map<std::string, std::string>& fm_headerFields, const std::string& fm_requestPath) noexcept;

		const std::tuple<std::string, Network::Protocol::httpStatusCode, std::string, std::map<std::string, std::string>> answerCertificateRevocationListGet(const std::map<std::string, std::string>& fm_headerFields) noexcept;
		const std::tuple<std::string, Network::Protocol::httpStatusCode, std::string, std::map<std::string, std::string>> answerCertificateRevocationListPost(const std::string& fm_requestContent, const Network::Protocol::httpMethod& fm_requestType, const std::map<std::string, std::string>& fm_headerFields, const std::string& fm_requestPath) noexcept;

		const std::tuple<std::string, Network::Protocol::httpStatusCode, std::string, std::map<std::string, std::string>> answerVerifyCertificatePost(const std::string& fm_request, const std::map<std::string, std::string>& fm_headerFields) noexcept;

		const std::tuple<std::string, Network::Protocol::httpStatusCode, std::string, std::map<std::string, std::string>> answerExtendedOnlineCertificateStatusProtocolPost(const std::string& fm_request, const std::map<std::string, std::string>& fm_headerFields, const std::string& fm_regexMatchedData) noexcept;

	protected:

		CERTIFICATESTORESERVER_API const void configureEndPoints(void) noexcept;

	public:

		CERTIFICATESTORESERVER_API certificateStoreServer(void) noexcept;
		CERTIFICATESTORESERVER_API virtual ~certificateStoreServer(void);

		CERTIFICATESTORESERVER_API const void setDatabaseInitialization(const bool fm_isDatabaseInitilizationEnabled = { true }) noexcept;
		CERTIFICATESTORESERVER_API const void setDefaultCreatedCertificatesDump(const bool fm_isDefalutCreatedCertificatesDumpEnabled = { true }) noexcept;

		CERTIFICATESTORESERVER_API const void forceClientAnthentication(void) noexcept;

		CERTIFICATESTORESERVER_API const void configureCertificateStore(void) noexcept;

		CERTIFICATESTORESERVER_API const void launchServer(void) const noexcept;

	};
}
