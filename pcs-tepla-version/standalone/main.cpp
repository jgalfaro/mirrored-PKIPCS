#include <stonepine/security/attributeBasedEncryption/pcsServer.h>
#include <stonepine/security/attributeBasedEncryption/pcsClient.h>

int main(void)
{
	const auto pS = std::make_unique<Stonepine::Security::pcsServer>();

	pS->setup();
	pS->generateIssuerKeyPair();

	pS->addAnAttribute("attribute1");
	pS->addAnAttribute("attribute2");
	pS->addAnAttribute("attribute3");
	
	const auto pC = std::make_unique<Stonepine::Security::pcsClient>(pS->getSetupParameters());

	pC->generateANewUserKeyPair();

	const auto i = pS->issue(pC->getUserPublicKey());
	
	if (pC->obtain(i) == true)
	{
		std::cout << "equation (1) OK" << std::endl;
	}

	const auto m = pS->generateRandomizedMessage();

	const auto s = pC->show(m.second);

	if (pS->verify(m.first, s) == true)
	{
		std::cout << "equation (2) OK" << std::endl;
	}

	return 0;
}
