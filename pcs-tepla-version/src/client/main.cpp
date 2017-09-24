/*
Télécom SudParis
2017, September
Pierre-Olivier Rocher
*/

#include <stonepine/architecture/architecture.h>

#ifdef __WINDOWS__
#include <stonepine/common/targetver.h>
#include <stonepine/common/applink.c>
#endif

#include <stonepine/libraries/pcsStore/pcsStoreClient.h>

#include <stonepine/utilities/xml/client/httpsClientFromXml.h>
#include <stonepine/utilities/stonepine.utilities.dumpCore.h>
#include <stonepine/utilities/commandLineOptions/commandLineOptions.h>

#include <stonepine/security/attributeBasedCryptography.h>

#include <stonepine/obfuscate/obfuscate.h>

int main(int argc, char **argv)
{
	auto l = Stonepine::logger::getInstance();

#if defined(__DEBUG__)
	l->enableConsoleLogging();
#else
	l->enableFileLogging();
#endif

#if __WITHOUT_LOGGER__ == 1
	l->disableLog();
#else
	l->enableLog();
#endif

#if (!defined(__DEBUG__))
	const auto o = Stonepine::obfuscate();
#else
	const auto dC = std::make_unique<Stonepine::Utilities::dumpCore>();
#endif

	const auto cLI = std::make_unique<Stonepine::commandLineOptions>("PCS store client", "3.0");

	try
	{
		cLI->addOptionS("-c", "--configuration-file", "Use this configuration file", false, "pcsStore.client.config.xml");

		cLI->setOptions(argc, argv);

		const auto cF = cLI->getOption<std::string>("--configuration-file");

		const auto hCFX = std::make_shared<Stonepine::Utilities::httpsClientFromXml>(cF, true);

		const auto pSC = std::make_unique<Stonepine::Libraries::pcsStoreClient>(hCFX);

		const auto sPS = pSC->getSetupParameters();

		if (sPS != nullptr)
		{
			const auto aBC = std::make_unique<Stonepine::Security::attributeBasedCryptography>(sPS);

			const auto uPK = aBC->generateANewUserKeyPair();

			const auto iD = pSC->postUserPublicKey(uPK);

			const auto v = aBC->obtain(std::get<1>(*iD.get()), std::get<2>(*iD.get()));
		}
	}
	catch (const std::exception& e)
	{
		Stonepine::LOG_ERROR(e.what());

		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
