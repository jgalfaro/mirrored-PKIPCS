/*
Télécom SudParis
2017, May
Pierre-Olivier Rocher
*/

#include <stonepine/logger/logger.h>
#include <codeSynthesis/authorizedCertificatesConfigBinding/authorizedCertificatesUtilities.h>

#include <stonepine/utilities/xml/codeSynthesis/authorizedCertificatesFromCodeSynthesis.h>

#include "authorizedCertificatesFromXml.h"

namespace Stonepine
{
	namespace Utilities
	{
		class authorizedCertificatesFromXml::authorizedCertificatesFromXmlBase final : public authorizedCertificatesConfigBinding::authorizedCertificatesUtilities
		{

		public:

			authorizedCertificatesFromXmlBase(const std::string& fm_xmlConfigurationFileFilePath, const bool fm_isXmlValidationEnabled) noexcept (false) : authorizedCertificatesUtilities(fm_xmlConfigurationFileFilePath, fm_isXmlValidationEnabled)
			{
				if (readXmlFile() == false)
				{
					throw std::runtime_error("[utilities.xml] exiting");
				}

				Stonepine::LOG_DEBUG("[utilities.xml] authorizedCertificatesFromXmlBase initialization");
			}

			virtual ~authorizedCertificatesFromXmlBase(void)
			{
				Stonepine::LOG_DEBUG("[utilities.xml] authorizedCertificatesFromXmlBase termination");
			}
		};

		const void authorizedCertificatesFromXml::configureAuthorizedCertificates(void) noexcept
		{
			auto aCS = m_authorizedCertificatesFromXmlBase->getAuthorizedCertificates();

			//authorizedCertificates *test = new CodeSynthesis::authorizedCertificatesFromCodeSynthesis(aCS);

			//setInstance(test.getInstance());		

			if (aCS != nullptr)
			{
				for (const auto &iTCCN : aCS->certificateCommmonName())
				{
					m_subjectCommonNames->push_back(iTCCN);
				}
			}
			else
			{
				Stonepine::LOG_ERROR("[utilities.xml] authorizedCertificates object is empty");

				exitProperly(EXIT_FAILURE);
			}
		}

		inline const void authorizedCertificatesFromXml::exitProperly(const bool fm_exitStatus) const noexcept
		{
			authorizedCertificatesFromXml::~authorizedCertificatesFromXml();

			exit(fm_exitStatus);
		}

		authorizedCertificatesFromXml::authorizedCertificatesFromXml(void) noexcept
		{
			m_authorizedCertificatesFromXmlBase = { nullptr };

			Stonepine::LOG_DEBUG("[utilities.xml] authorizedCertificatesFromXml initialization");
		}

		authorizedCertificatesFromXml::authorizedCertificatesFromXml(const std::string& fm_xmlConfigurationFileFilePath, const bool fm_isXmlValidationEnabled) noexcept (false) : authorizedCertificatesFromXml()
		{
			try
			{
				m_authorizedCertificatesFromXmlBase = std::make_unique<authorizedCertificatesFromXmlBase>(fm_xmlConfigurationFileFilePath, fm_isXmlValidationEnabled);
			}
			catch (const std::exception& e)
			{
				Stonepine::LOG_ERROR(e.what());

				exitProperly(EXIT_FAILURE);
			}

			configureAuthorizedCertificates();
		}

		authorizedCertificatesFromXml::~authorizedCertificatesFromXml(void)
		{
			Stonepine::LOG_DEBUG("[utilities.xml] authorizedCertificatesFromXml termination");
		}

		const std::string authorizedCertificatesFromXml::getXmlConfigurationFileFilePath(void) const noexcept
		{
			return m_authorizedCertificatesFromXmlBase->getXmlFileFilePath();
		}
	}
}
