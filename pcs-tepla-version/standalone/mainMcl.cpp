#include "mainMcl.h"

#if CYBOZU_CPP_VERSION >= CYBOZU_CPP_VERSION_CPP11
#include <random>
std::random_device g_rg2;
#endif

void mainMcl(void)
{
	/***** SETUP *****/

	/*const char *aa = "12723517038133731887338407189719511622662176727675373276651903807414909099441";
	const char *ab = "4168783608814932154536427934509895782246573715297911553964171371032945126671";
	const char *ba = "13891744915211034074451795021214165905772212241412891944830863846330766296736";
	const char *bb = "7937318970632701341203597196594272556916396164729705624521405069090520231616";

	mcl::bn::CurveParam cp = mcl::bn::CurveFp254BNb;

	bn256init(cp);

	G1 Q(-1, 1);
	G2 P(Fp2(aa, ab), Fp2(ba, bb));*/

	bn256init();

	G1 g1, h1;
	G2 g2, h2;

	setup(g1, g2, h1, h2);

	mcl::bn256::Fr issuerPrivateKey;
	mcl::bn256::G1 issuerPublicKey;
	mcl::bn256::Fp12 xO;
	mcl::bn256::G2 yO;

	generateIssuerKeyPair(g1, g2, h2, issuerPrivateKey, issuerPublicKey, xO, yO);

	mcl::bn256::Fr userPrivateKey;
	mcl::bn256::G1 userPublicKey;

	generateAUserKeyPair(h1, userPrivateKey, userPublicKey);

	auto aS = std::vector<std::string>();

	aS.emplace_back("attribute1");
	aS.emplace_back("attribute2");
	aS.emplace_back("attribute3");

	auto aHVS = std::vector<std::string>();

	aHVS.emplace_back("5738e5d4979a2a7c9515884f1e398c0c7ccb3ed1");
	aHVS.emplace_back("83d4b3514715c817c31a8d90515626222ce3b02c");
	aHVS.emplace_back("e624d7f34ba7e07326cfee105b58ffa022603940");

	auto aGVS = std::vector<mcl::bn256::G1>();

	for (const auto &iTA : aS)
	{
		mcl::bn256::Fp gVN;
		gVN.setRand(g_rg2);

		mcl::bn256::G1 gV;

		mcl::bn256::BN::mapToG1(gV, gVN);

		aGVS.emplace_back(gV);
	}

	mcl::bn256::Fr hsInverse = computeInverseHashProduct(aHVS);
	mcl::bn256::G1 c1;
	mcl::bn256::G2 c2;
	std::vector<mcl::bn256::G1> c3;

	issue(g2, h1, issuerPrivateKey, issuerPublicKey, userPublicKey, hsInverse, aGVS, c1, c2, c3);

	if (obtain(g1, g2, h1, c1, c2, xO, yO, userPrivateKey, hsInverse) == true)
	{
		std::cout << "equation (1) OK" << std::endl;
	}

	mcl::bn256::Fr issuerRandomMessage;
	issuerRandomMessage.setRand(g_rg2);

	mcl::bn256::G1 issuerRandomizedMessage;

	mcl::bn256::G1::mul(issuerRandomizedMessage, g1, issuerRandomMessage);

	mcl::bn256::G1 c1Prime;
	mcl::bn256::G2 c2Prime;
	std::vector<mcl::bn256::G1> c3Prime;
	mcl::bn256::G1 sigma1;
	mcl::bn256::G1 sigma2;
	mcl::bn256::G2 a;
	std::vector<mcl::bn256::G2> w;

	show(g1, g2, h1, issuerRandomMessage, issuerRandomizedMessage, userPrivateKey, yO, aHVS, c1, c2, c3, c1Prime, c2Prime, c3Prime, sigma1, sigma2, a, w);

	if (verify(h1, g2, c2Prime, sigma1, sigma2, xO, issuerPublicKey, issuerRandomMessage, std::vector<std::string>(), a, w, aGVS) == true)
	{
		std::cout << "equation (2) OK" << std::endl;
	}
}
