/*
Télécom SudParis
2017, August
Pierre-Olivier Rocher
*/

#include <stonepine/architecture/architecture.h>

#ifdef __WINDOWS__
#include "../../../common/targetver.h"
#include "../../../common/applink.c"
#endif

#include <stonepine/libraries/pcsStore/pcsStoreServer.h>

#include <stonepine/utilities/xml/application/applicationFromXml.h>
#include <stonepine/utilities/stonepine.utilities.dumpCore.h>
#include <stonepine/utilities/commandLineOptions/commandLineOptions.h>

#include <stonepine/obfuscate/obfuscate.h>

int main(int argc, char** argv)
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

	const auto cLI = std::make_unique<Stonepine::commandLineOptions>("PCS store", "3.0");

	try
	{
		cLI->addOptionS("-c", "--configuration-file", "Use this configuration file", true, "pcsStore.server.config.xml");

		cLI->setOptions(argc, argv);

		const auto cF = cLI->getOption<std::string>("--configuration-file");

		const auto a = std::make_shared<Stonepine::Utilities::applicationFromXml>(cF, true);

		const auto hS = a->getHttpServers()->getFirstObject();

		const auto cSS = std::make_unique<Stonepine::Libraries::pcsStoreServer>(hS);

		cSS->configure();
		cSS->launchServer();
	}
	catch (const std::exception& e)
	{
		Stonepine::LOG_ERROR(e.what());
	}

	return EXIT_SUCCESS;
}
