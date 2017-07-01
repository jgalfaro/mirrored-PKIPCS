/*
Télécom SudParis, Télécom Saint-Étienne
2017, May
Pierre-Olivier Rocher
*/

#pragma once

#ifdef _WIN32
#ifdef DATABASE_EXPORTS
#define DATABASE_API __declspec(dllexport)
#else
#define DATABASE_API __declspec(dllimport)
#endif
#else
#define DATABASE_API
#endif

#include <string>
#include <memory>

#include <stonepine/network/secureClient.h>

#include <stonepine/database/database.h>

namespace Stonepine
{
	namespace Database
	{
		class postgreSqlClient : public Network::secureClient
		{

		protected:

			class postgreSqlClientBase;

			std::unique_ptr<postgreSqlClientBase> m_postgreSqlClientBase;

		protected:

			DATABASE_API const void setServerIpAddressOrDomainName(const std::string& fm_serverIpAddressOrDomainName) noexcept;

			DATABASE_API const void generateConnectionOptionsString(void) noexcept;

		public:

			DATABASE_API postgreSqlClient(void) noexcept;
			DATABASE_API postgreSqlClient(const std::string& fm_type, const std::string& fm_userName, const std::string& fm_password, const std::string& fm_name, const std::string& fm_serverIpAddressOrDomainName, const uint16_t fm_serverPort) noexcept;
			DATABASE_API postgreSqlClient(const std::string& fm_type, const std::string& fm_name, const std::string& fm_serverIpAddressOrDomainName, const uint16_t fm_serverPort, const std::string& fm_clientCertificateFileFilePath, const std::string& fm_clientPrivateKeyFileFilePath, const std::string& fm_serverCertificateAuthorityFileFilePath) noexcept;
			DATABASE_API postgreSqlClient(const std::string& fm_type, const std::string& fm_name, const std::string& fm_serverIpAddressOrDomainName, const uint16_t fm_serverPort, const std::string& fm_clientCertificateFileFilePath, const std::string& fm_clientPrivateKeyFileFilePath, const std::string& fm_serverCertificateAuthorityFileFilePath, const std::string& m_databaseServerSubjectCommonName) noexcept;

			DATABASE_API virtual ~postgreSqlClient(void);

			// Copy
			DATABASE_API postgreSqlClient(const postgreSqlClient& fm_database);
			DATABASE_API postgreSqlClient& operator=(const postgreSqlClient& fm_database);

			// Configure
			DATABASE_API const void setUuid(const Utilities::uuid& fm_uuid) noexcept;
			DATABASE_API const void setType(const std::string& fm_type) noexcept;
			DATABASE_API const void setName(const std::string& fm_name) noexcept;

			DATABASE_API const void setUserName(const std::string& fm_userName) noexcept;
			DATABASE_API const void setPassword(const std::string& fm_password) noexcept;

			DATABASE_API const Utilities::uuid& getUuid(void) const noexcept;
			DATABASE_API const std::string& getType(void) const noexcept;
			DATABASE_API const std::string& getName(void) const noexcept;

			DATABASE_API const std::string& getUserName(void) const noexcept;
			DATABASE_API const std::string& getPassword(void) const noexcept;

			// Database basic operations
			DATABASE_API const bool openConnection(void) noexcept;
			DATABASE_API const bool closeConnection(void) const noexcept;

			DATABASE_API const bool createSchema(void) const noexcept;
			DATABASE_API const bool dropSchema(void) const noexcept;

		};
	}
}
