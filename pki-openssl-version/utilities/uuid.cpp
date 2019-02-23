/*
Télécom SudParis
2017, May
Pierre-Olivier Rocher
*/

#include <stonepine/architecture/architecture.h>

#ifdef __WINDOWS__
#include <Rpc.h>
#else
#include <uuid/uuid.h>
#endif

#include "uuid.h"

namespace Stonepine
{
	namespace Utilities
	{
		class uuid::uuidBase final
		{

		public:

#ifdef __WINDOWS__
			mutable UUID m_uuid;
#else
			mutable uuid_t uuid;
#endif

		public:

			uuidBase(void) noexcept
			{
				m_uuid = {};
			}

			~uuidBase(void)
			{

			}
		};

		uuid::uuid(void) noexcept
		{
			m_uuidBase = std::make_unique<uuidBase>();
		}

		uuid::uuid(const std::string& fm_uuid) noexcept (false) : uuid()
		{
#ifdef __WINDOWS__
			if (UuidFromStringA((RPC_CSTR)fm_uuid.c_str(), &m_uuidBase->m_uuid) != RPC_S_OK)
			{
				throw std::runtime_error("[utilities] unable to intepret string as a UUID");
			}
#else
			if (uuid_parse(fm_uuid.c_str(), m_uuid) != 0)
			{
				throw std::runtime_error("[utilities] unable to intepret string as a UUID");
			}
#endif
		}

		uuid::~uuid(void)
		{

		}

		/*uuid::uuid(uuid&& fm_uuid) noexcept = default;

		uuid& uuid::operator=(uuid && fm_uuid) noexcept = default;*/

		uuid::uuid(const uuid& fm_uuid)
		{
			m_uuidBase = std::make_unique<uuidBase>(*fm_uuid.m_uuidBase);
		}

		uuid& uuid::operator=(const uuid& fm_uuid)
		{
			if (this != &fm_uuid)
			{
				m_uuidBase.reset(new uuidBase(*fm_uuid.m_uuidBase));
			}

			return *this;
		}

		const void uuid::generate(void) const noexcept
		{
#ifdef __WINDOWS__
			UuidCreate(&m_uuidBase->m_uuid);
#else
			uuid_generate_random(m_uuid);
#endif
		}

		const std::string uuid::getUuidAsString(void) const noexcept
		{
#ifdef __WINDOWS__
			unsigned char* str;
			UuidToStringA(&m_uuidBase->m_uuid, &str);

			std::string s((char*)str);

			RpcStringFreeA(&str);
#else
			char s[37];
			uuid_unparse(m_uuid, s);
#endif
			return s;
		}
	}
}
 
