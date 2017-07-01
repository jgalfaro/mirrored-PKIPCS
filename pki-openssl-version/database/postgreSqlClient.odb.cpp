/*
Télécom SudParis, Télécom Saint-Étienne
2017, May
Pierre-Olivier Rocher
*/

#include <odb/database.hxx>
#include <odb/session.hxx>
#include <odb/transaction.hxx>
#include <odb/schema-catalog.hxx>

#include <odb/database.hxx>

#if defined(DATABASE_MYSQL)
#  include <odb/mysql/database.hxx>
#elif defined(DATABASE_SQLITE)
#  include <odb/connection.hxx>
#  include <odb/transaction.hxx>
#  include <odb/schema-catalog.hxx>
#  include <odb/sqlite/database.hxx>
#elif defined(DATABASE_PGSQL)
#  include <odb/pgsql/database.hxx>
#elif defined(DATABASE_ORACLE)
#  include <odb/oracle/database.hxx>
#elif defined(DATABASE_MSSQL)
#  include <odb/mssql/database.hxx>
#else
#  error unknown database; did you forget to define the DATABASE_* macros?
#endif

#include <stonepine/logger/logger.h>

#include <stonepine/utilities/utilities.h>

#include <stonepine/network/client/dnsClient.h>

#include "postgreSqlClient.h"

#include "postgreSqlClientBase.odb.cpp"

namespace Stonepine
{
	namespace Database
	{
		const void postgreSqlClient::setServerIpAddressOrDomainName(const std::string& fm_serverIpAddressOrDomainName) noexcept
		{
			if (utilities::isAValidIpAddress(fm_serverIpAddressOrDomainName) == false)
			{
				Network::dnsClient dNS(fm_serverIpAddressOrDomainName);

				if (m_isServerUsingAnIpV6Address == false)
				{
					const auto ipV4 = dNS.getIpV4Addresses();

					if (ipV4.empty() == false)
					{
						m_serverIpAddress = ipV4.front();
					}
				}
				else
				{
					const auto ipV6 = dNS.getIpV6Addresses();

					if (ipV6.empty() == false)
					{
						m_serverIpAddress = ipV6.front();
					}
				}

				if (m_serverIpAddress.empty() == true)
				{
					LOG_ERROR("unable to get IP address for name server \"", fm_serverIpAddressOrDomainName, "\"");
				}
			}
			else
			{
				m_serverIpAddress = fm_serverIpAddressOrDomainName;
			}
		}

		const void postgreSqlClient::generateConnectionOptionsString(void) noexcept
		{
			m_postgreSqlClientBase->m_connectionOptions.append(" hostaddr=" + getServerIpAddress());
			m_postgreSqlClientBase->m_connectionOptions.append(" port=" + std::to_string(getServerPort()));
			m_postgreSqlClientBase->m_connectionOptions.append(" dbname=" + getName());

			if (isClientCertificateAuthenticationEnabled() == false && isClientSmartCardAuthenticationEnabled() == false)
			{
				m_postgreSqlClientBase->m_connectionOptions.append(" user=" + getUserName());
				m_postgreSqlClientBase->m_connectionOptions.append(" password=" + getPassword());
			}
			else
			{
				m_postgreSqlClientBase->m_connectionOptions.append(" host=" + m_authorizedCertificates->getSubjectCommonNames()->front());

				if (m_postgreSqlClientBase->m_secureConnectionMode == databaseSecureConnectionType::DSCT_DISABLE)
				{
					m_postgreSqlClientBase->m_connectionOptions.append(" sslmode=disable");
				}
				else if (m_postgreSqlClientBase->m_secureConnectionMode == databaseSecureConnectionType::DSCT_ALLOW)
				{
					m_postgreSqlClientBase->m_connectionOptions.append(" sslmode=allow");
				}
				else if (m_postgreSqlClientBase->m_secureConnectionMode == databaseSecureConnectionType::DSCT_PREFER)
				{
					m_postgreSqlClientBase->m_connectionOptions.append(" sslmode=prefer");
				}
				else if (m_postgreSqlClientBase->m_secureConnectionMode == databaseSecureConnectionType::DSCT_REQUIRE)
				{
					m_postgreSqlClientBase->m_connectionOptions.append(" sslmode=require");
				}
				else if (m_postgreSqlClientBase->m_secureConnectionMode == databaseSecureConnectionType::DSCT_VERIFY_CA)
				{
					m_postgreSqlClientBase->m_connectionOptions.append(" sslmode=verify-ca");
				}
				else if (m_postgreSqlClientBase->m_secureConnectionMode == databaseSecureConnectionType::DSCT_VERIFY_FULL)
				{
					m_postgreSqlClientBase->m_connectionOptions.append(" sslmode=verify-full");
				}
				else
				{
					LOG_WARNING("[database] unknow enumeration value");
				}

				m_postgreSqlClientBase->m_connectionOptions.append(" sslcert=" + getClientCertificateFileFilePath());
				m_postgreSqlClientBase->m_connectionOptions.append(" sslkey=" + getClientPrivateKeyFileFilePath());
				m_postgreSqlClientBase->m_connectionOptions.append(" sslrootcert=" + getServerCertificateAuthorityFileFilePath());
			}
		}

		postgreSqlClient::postgreSqlClient(void) noexcept : secureClient()
		{
			m_postgreSqlClientBase = std::make_unique<postgreSqlClientBase>();

			LOG_DEBUG("[database] database initialization");
		}

		postgreSqlClient::postgreSqlClient(const std::string& fm_type, const std::string& fm_userName, const std::string& fm_password, const std::string& fm_name, const std::string& fm_serverIpAddressOrDomainName, const uint16_t fm_serverPort) noexcept : postgreSqlClient()
		{
			setType(fm_type);
			setName(fm_name);

			setUserName(fm_userName);
			setPassword(fm_password);

			setServerIpAddressOrDomainName(fm_serverIpAddressOrDomainName);
			setServerPort(fm_serverPort);

			m_postgreSqlClientBase->m_secureConnectionMode = { databaseSecureConnectionType::DSCT_ALLOW };
		}

		postgreSqlClient::postgreSqlClient(const std::string& fm_type, const std::string& fm_name, const std::string& fm_serverIpAddressOrDomainName, const uint16_t fm_serverPort, const std::string& fm_clientCertificateFileFilePath, const std::string& fm_clientPrivateKeyFileFilePath, const std::string& fm_serverCertificateAuthorityFileFilePath) noexcept : postgreSqlClient(fm_type, {}, {}, fm_name, fm_serverIpAddressOrDomainName, fm_serverPort)
		{
			setClientCertificateFileFilePath(fm_clientCertificateFileFilePath);
			setClientPrivateKeyFileFilePath(fm_clientPrivateKeyFileFilePath);
			setServerCertificateAuthorityFileFilePath(fm_serverCertificateAuthorityFileFilePath);

			m_postgreSqlClientBase->m_secureConnectionMode = { databaseSecureConnectionType::DSCT_VERIFY_CA };
		}

		postgreSqlClient::postgreSqlClient(const std::string& fm_type, const std::string& fm_name, const std::string& fm_serverIpAddressOrDomainName, const uint16_t fm_serverPort, const std::string& fm_clientCertificateFileFilePath, const std::string& fm_clientPrivateKeyFileFilePath, const std::string& fm_serverCertificateAuthorityFileFilePath, const std::string& fm_databaseServerSubjectCommonName) noexcept : postgreSqlClient(fm_type, fm_name, fm_serverIpAddressOrDomainName, fm_serverPort, fm_clientCertificateFileFilePath, fm_clientPrivateKeyFileFilePath, fm_serverCertificateAuthorityFileFilePath)
		{
			m_authorizedCertificates->addASubjectCommonName(fm_databaseServerSubjectCommonName);

			m_postgreSqlClientBase->m_secureConnectionMode = { databaseSecureConnectionType::DSCT_VERIFY_FULL };
		}

		postgreSqlClient::~postgreSqlClient(void)
		{
			LOG_DEBUG("[database] database termination");
		}

		postgreSqlClient::postgreSqlClient(const postgreSqlClient& fm_database)
		{
			this->secureClient::operator=(fm_database);

			m_postgreSqlClientBase = std::make_unique<postgreSqlClientBase>(*fm_database.m_postgreSqlClientBase);
		}

		postgreSqlClient& postgreSqlClient::operator=(const postgreSqlClient& fm_database)
		{
			if (this != &fm_database)
			{
				this->secureClient::operator=(fm_database);

				m_postgreSqlClientBase.reset(new postgreSqlClientBase(*fm_database.m_postgreSqlClientBase));
			}

			return *this;
		}

		const void postgreSqlClient::setUuid(const Utilities::uuid& fm_uuid) noexcept
		{
			m_uuid = fm_uuid;
		}

		const void postgreSqlClient::setType(const std::string& fm_type) noexcept
		{
			m_postgreSqlClientBase->m_type = fm_type;
		}

		const void postgreSqlClient::setUserName(const std::string& fm_user) noexcept
		{
			m_authentication->setUserName(fm_user);
		}

		const void postgreSqlClient::setPassword(const std::string& fm_password) noexcept
		{
			m_authentication->setPassword(fm_password);
		}

		const void postgreSqlClient::setName(const std::string& fm_name) noexcept
		{
			m_postgreSqlClientBase->m_name = fm_name;
		}

		const Utilities::uuid & postgreSqlClient::getUuid(void) const noexcept
		{
			return m_uuid;
		}

		const std::string& postgreSqlClient::getType(void) const noexcept
		{
			return m_postgreSqlClientBase->m_type;
		}

		const std::string& postgreSqlClient::getUserName(void) const noexcept
		{
			return m_authentication->getUserName();
		}

		const std::string& postgreSqlClient::getPassword(void) const noexcept
		{
			return m_authentication->getPassword();
		}

		const bool postgreSqlClient::openConnection(void) noexcept
		{
			generateConnectionOptionsString();

			try
			{
#if defined(DATABASE_PGSQL)
				m_postgreSqlClientBase->m_database = std::make_shared<odb::pgsql::database>(m_postgreSqlClientBase->m_connectionOptions);
#endif
				return true;
			}
			catch (const odb::exception& e)
			{
				LOG_ERROR("[database] unable to connect to ", getName(), " database -> ", e.what());
			}

			return false;
		}

		const bool postgreSqlClient::closeConnection(void) const noexcept
		{
			return true;
		}

		const bool postgreSqlClient::createSchema(void) const noexcept
		{
			try
			{
				m_postgreSqlClientBase->isDatabaseObjectNullptr();

				odb::transaction t(m_postgreSqlClientBase->m_database->begin());
				odb::schema_catalog::create_schema(*m_postgreSqlClientBase->m_database);

				t.commit();

				return true;
			}
			catch (const odb::exception& e)
			{
				LOG_ERROR("[database] unable to create ", getName(), " database schema -> ", e.what());
			}
			catch (const std::exception& e)
			{
				LOG_ERROR("[database] ", e.what());
			}

			return false;
		}

		const bool postgreSqlClient::dropSchema(void) const noexcept
		{
			try
			{
				m_postgreSqlClientBase->isDatabaseObjectNullptr();

				odb::transaction t(m_postgreSqlClientBase->m_database->begin());
				odb::schema_catalog::drop_schema(*m_postgreSqlClientBase->m_database);

				t.commit();

				return true;
			}
			catch (const odb::exception& e)
			{
				LOG_ERROR("[database] unable to drop ", getName(), " database schema -> ", e.what());
			}
			catch (const std::exception& e)
			{
				LOG_ERROR("[database] ", e.what());
			}

			return false;
		}

		const std::string& postgreSqlClient::getName(void) const noexcept
		{
			return m_postgreSqlClientBase->m_name;
		}
	}
}
