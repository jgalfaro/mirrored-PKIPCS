/*
Télécom SudParis, Télécom Saint-Étienne
2017, May
Pierre-Olivier Rocher
*/

namespace Stonepine
{
	namespace Database
	{
		class postgreSqlClient::postgreSqlClientBase final
		{

		public:

			// Database object
			std::shared_ptr<odb::database> m_database;

			// Basics
			mutable std::string m_connectionOptions;

			std::string m_type;
			std::string m_name;

			// Security options
			databaseSecureConnectionType m_secureConnectionMode;

		public:

			postgreSqlClientBase(void) noexcept
			{
				m_database = { nullptr };

				m_secureConnectionMode = { databaseSecureConnectionType::DSCT_DISABLE };

				LOG_DEBUG("[database] databaseBase initialization");
			}

			~postgreSqlClientBase(void)
			{
				LOG_DEBUG("[database] databaseBase termination");
			}

			const void isDatabaseObjectNullptr(void) const noexcept (false)
			{
				if (m_database == nullptr)
				{
					throw std::runtime_error("the database object is nullptr");
				}
			}
		};
	}
}
 
