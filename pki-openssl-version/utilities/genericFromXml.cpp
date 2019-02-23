/*
Télécom SudParis
2016, December
Pierre-Olivier Rocher
*/

#include "genericFromXml.h"

namespace Stonepine
{
	namespace Utilities
	{
		bool genericFromXml::m_isXmlValidationEnabled = { true };

		genericFromXml::genericFromXml(void) noexcept
		{
			m_isXmlValidationEnabled = { true };
		}

		genericFromXml::genericFromXml(const std::string& fm_xmlFileFilePath, const bool fm_isXmlValidationEnabled) noexcept
		{
			setXmlFileFilePath(fm_xmlFileFilePath);
			setXmlValidation(fm_isXmlValidationEnabled);
		}

		genericFromXml::~genericFromXml(void)
		{

		}

		const std::string& genericFromXml::getXmlFileFilePath(void) const noexcept
		{
			return m_xmlFileFilePath;
		}

		const bool genericFromXml::getXmlValidation(void) noexcept
		{
			return m_isXmlValidationEnabled;
		}

		const void genericFromXml::setXmlFileFilePath(const std::string& fm_xmlFileFilePath) noexcept
		{
			m_xmlFileFilePath = fm_xmlFileFilePath;
		}

		const void genericFromXml::setXmlValidation(const bool fm_isXmlValidationEnabled) noexcept
		{
			m_isXmlValidationEnabled = fm_isXmlValidationEnabled;
		}
	}
}
 
