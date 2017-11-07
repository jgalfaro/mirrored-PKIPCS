#include "mclUtilities.h"

mcl::bn256::Fp12 pkuSisHsIG2;
mcl::bn256::Fr rRPrime;

#if CYBOZU_CPP_VERSION >= CYBOZU_CPP_VERSION_CPP11
#include <random>
std::random_device g_rg;
#endif

const mcl::bn256::Fr computeInverseHashProduct(const std::vector<std::string> fm_attributesHashes)
{
	mcl::bn256::Fr hsI;

	for (auto iTAH = fm_attributesHashes.begin(); iTAH != fm_attributesHashes.end(); ++iTAH)
	{
		mcl::bn256::Fr v(*iTAH, 16);

		if (iTAH == fm_attributesHashes.begin())
		{
			hsI = v;
		}
		else
		{
			mcl::bn256::Fr::mul(hsI, hsI, v);
		}
	}

	mcl::bn256::Fr::inv(hsI, hsI);

	return hsI;
}

void setup(mcl::bn256::G1& fm_g1, mcl::bn256::G2& fm_g2, mcl::bn256::G1& fm_h1, mcl::bn256::G2& fm_h2) noexcept
{
	mcl::bn256::Fp g1N;
	g1N.setRand(g_rg);
	mcl::bn256::BN::mapToG1(fm_g1, g1N);

	mcl::bn256::Fp g2N;
	g2N.setRand(g_rg);
	mcl::bn256::BN::mapToG2(fm_g2, mcl::bn256::Fp2(g2N, 0));

	mcl::bn256::Fr alpha;
	alpha.setRand(g_rg);

	mcl::bn256::G1::mul(fm_h1, fm_g1, alpha);

	mcl::bn256::G2::mul(fm_h2, fm_g2, alpha);
}

void generateIssuerKeyPair(const mcl::bn256::G1& fm_g1, const mcl::bn256::G2& fm_g2, const mcl::bn256::G2& fm_h2, mcl::bn256::Fr& fm_privateKey, mcl::bn256::G1& fm_publicKey, mcl::bn256::Fp12& fm_xO, mcl::bn256::G2& fm_yO) noexcept
{
	fm_privateKey.setRand(g_rg);

	mcl::bn256::G1::mul(fm_publicKey, fm_g1, fm_privateKey);

	mcl::bn256::Fp12 eg1g2;

	mcl::bn256::BN::pairing(eg1g2, fm_g1, fm_g2);

	mcl::bn256::Fp12::pow(fm_xO, eg1g2, fm_privateKey);

	mcl::EcT<mcl::bn256::Fp2>::mul(fm_yO, fm_h2, fm_privateKey);

	//// TEST

	mcl::bn256::Fp12 xoG2;

	mcl::bn256::BN::pairing(xoG2, fm_publicKey, fm_g2);

	if (xoG2 == fm_xO)
	{
		std::cout << "generateIssuerKeyPair, e(xo, g2) == Xo" << std::endl;
	}
	else
	{
		std::cout << "generateIssuerKeyPair, e(xo, g2) != Xo" << std::endl;
	}

	//// TEST
}

void generateAUserKeyPair(const mcl::bn256::G1& fm_h1, mcl::bn256::Fr& fm_privateKey, mcl::bn256::G1& fm_publicKey) noexcept
{
	fm_privateKey.setRand(g_rg);

	mcl::bn256::G1::mul(fm_publicKey, fm_h1, fm_privateKey);
}

void issue(const mcl::bn256::G2& fm_g2, const mcl::bn256::G1& fm_h1, const mcl::bn256::Fr& fm_issuerPrivateKey, const mcl::bn256::G1& fm_issuerPublicKey, const mcl::bn256::G1& fm_userPublicKey, const mcl::bn256::Fr& fm_hsInverse, const std::vector<mcl::bn256::G1>& fm_gammaValues, mcl::bn256::G1& fm_c1, mcl::bn256::G2& fm_c2, std::vector<mcl::bn256::G1>& fm_c3) noexcept
{
	mcl::bn256::Fr r;
	r.setRand(g_rg);

	mcl::bn256::G2::mul(fm_c2, fm_g2, r);

	mcl::EcT<mcl::bn256::Fp> h1r;

	mcl::EcT<mcl::bn256::Fp>::mul(h1r, fm_h1, r);

	mcl::bn256::Fr sisHsI;

	mcl::bn256::Fr::mul(sisHsI, fm_issuerPrivateKey, fm_hsInverse);

	mcl::EcT<mcl::bn256::Fp> pkuSisHsI;

	mcl::EcT<mcl::bn256::Fp>::mul(pkuSisHsI, fm_userPublicKey, sisHsI);

	mcl::EcT<mcl::bn256::Fp> pkuSisHsIH1r;

	mcl::EcT<mcl::bn256::Fp>::add(pkuSisHsIH1r, pkuSisHsI, h1r);

	mcl::EcT<mcl::bn256::Fp>::add(fm_c1, fm_issuerPublicKey, pkuSisHsIH1r);

	for (const auto &iTGV : fm_gammaValues)
	{
		mcl::EcT<mcl::bn256::Fp> gR;

		mcl::EcT<mcl::bn256::Fp>::mul(gR, iTGV, r);

		fm_c3.emplace_back(gR);
	}

	//// TEST

	mcl::bn256::BN::pairing(pkuSisHsIG2, pkuSisHsI, fm_g2);

	rRPrime = r;

	//// TEST
}

bool obtain(const mcl::bn256::G1& fm_g1, const mcl::bn256::G2& fm_g2, const mcl::bn256::G1& fm_h1, const mcl::bn256::G1& fm_c1, const mcl::bn256::G2& fm_c2, const mcl::bn256::Fp12& fm_xO, const mcl::bn256::G2& fm_yO, const mcl::bn256::Fr& fm_userPrivateKey, const mcl::bn256::Fr& fm_hsInverse) noexcept
{
	mcl::bn256::Fp12 ec1g2;

	mcl::bn256::BN::pairing(ec1g2, fm_c1, fm_g2);

	mcl::bn256::Fr skuHsI;

	mcl::bn256::Fr::mul(skuHsI, fm_userPrivateKey, fm_hsInverse);

	mcl::EcT<mcl::bn256::Fp> g1SkuHsI;

	mcl::EcT<mcl::bn256::Fp>::mul(g1SkuHsI, fm_g1, skuHsI);

	mcl::bn256::Fp12 eG1SkuHsIYYis;

	mcl::bn256::BN::pairing(eG1SkuHsIYYis, g1SkuHsI, fm_yO);

	mcl::bn256::Fp12 eh1c2;

	mcl::bn256::BN::pairing(eh1c2, fm_h1, fm_c2);

	mcl::bn256::Fp12 eG1SkuHsIYYisEh1c2;

	mcl::bn256::Fp12::mul(eG1SkuHsIYYisEh1c2, eG1SkuHsIYYis, eh1c2);

	mcl::bn256::Fp12 XXisEG1SkuHsIYYisEh1c2;

	mcl::bn256::Fp12::mul(XXisEG1SkuHsIYYisEh1c2, fm_xO, eG1SkuHsIYYisEh1c2);

	if (ec1g2 == XXisEG1SkuHsIYYisEh1c2)
	{
		return true;
	}

	return false;
}

const void show1(const mcl::bn256::G2& fm_g2, const mcl::bn256::G1& fm_h1, const mcl::bn256::G1& fm_c1, const mcl::bn256::G2& fm_c2, const std::vector<mcl::bn256::G1>& fm_c3, mcl::bn256::G1& fm_c1Prime, mcl::bn256::G2& fm_c2Prime, std::vector<mcl::bn256::G1>& fm_c3Prime) noexcept
{
	mcl::bn256::Fr rPrime;
	rPrime.setRand(g_rg);

	mcl::EcT<mcl::bn256::Fp> h1rPrime;

	mcl::EcT<mcl::bn256::Fp>::mul(h1rPrime, fm_h1, rPrime);

	mcl::EcT<mcl::bn256::Fp>::add(fm_c1Prime, fm_c1, h1rPrime);

	mcl::EcT<mcl::bn256::Fp2> g2rPrime;

	mcl::EcT<mcl::bn256::Fp2>::mul(g2rPrime, fm_g2, rPrime);

	mcl::EcT<mcl::bn256::Fp2>::add(fm_c2Prime, fm_c2, g2rPrime);

	for (const auto &iTC3 : fm_c3)
	{
		mcl::EcT<mcl::bn256::Fp> gIRP;

		mcl::EcT<mcl::bn256::Fp>::mul(gIRP, iTC3, rPrime);

		mcl::EcT<mcl::bn256::Fp> c3iPrime;

		mcl::EcT<mcl::bn256::Fp>::add(c3iPrime, iTC3, gIRP);

		fm_c3Prime.emplace_back(c3iPrime);
	}

	//// TEST

	mcl::bn256::Fr::add(rRPrime, rRPrime, rPrime);

	mcl::EcT<mcl::bn256::Fp> h1RRPrime;

	mcl::EcT<mcl::bn256::Fp>::mul(h1RRPrime, fm_h1, rRPrime);

	mcl::bn256::Fp12 eh1RRPrimeG2;

	mcl::bn256::BN::pairing(eh1RRPrimeG2, h1RRPrime, fm_g2);

	mcl::bn256::Fp12 eh1C2;

	mcl::bn256::BN::pairing(eh1C2, fm_h1, fm_c2Prime);

	if (eh1RRPrimeG2 == eh1C2)
	{
		std::cout << "show1, e(h1^(r+r'), g2) == e(h1, c2')" << std::endl;
	}
	else
	{
		std::cout << "show1, e(h1^(r+r'), g2) != e(h1, c2')" << std::endl;
	}

	//// TEST
}

const std::vector<mcl::bn256::Fr> show2(void) noexcept
{
	auto rS2 = std::vector<mcl::bn256::Fr>();

	rS2.emplace_back(mcl::bn256::Fr(1));
	rS2.emplace_back(mcl::bn256::Fr(-1));
	rS2.emplace_back(mcl::bn256::Fr(1));

	return rS2;
}

const void show3(const mcl::bn256::G2& fm_c2Prime, const std::vector<mcl::bn256::G1>& fm_c3Prime, const std::vector<mcl::bn256::Fr>& fm_v, mcl::bn256::G1& fm_b, std::vector<mcl::bn256::G2>& fm_wi) noexcept
{
	for (const auto &iTV : fm_v)
	{
		mcl::EcT<mcl::bn256::Fp2> wi;

		if (iTV.isZero() != true)
		{
			mcl::EcT<mcl::bn256::Fp2>::mul(wi, fm_c2Prime, iTV);

			fm_wi.emplace_back(wi);
		}
		else
		{
			wi.set(mcl::Fp2T<mcl::bn256::Fp>(0), mcl::Fp2T<mcl::bn256::Fp>(0));

			fm_wi.emplace_back(wi);
		}
	}

	mcl::EcT<mcl::bn256::Fp> c31PrimeV1;

	mcl::EcT<mcl::bn256::Fp>::mul(c31PrimeV1, fm_c3Prime[0], fm_v[0]);

	mcl::EcT<mcl::bn256::Fp> c33PrimeV2;

	mcl::EcT<mcl::bn256::Fp>::mul(c33PrimeV2, fm_c3Prime[2], fm_v[1]);

	/*mcl::EcT<mcl::bn256::Fp> c32PrimeV3;

	mcl::EcT<mcl::bn256::Fp>::mul(c32PrimeV3, fm_c3Prime[1], fm_v[2]);*/

	//mcl::EcT<mcl::bn256::Fp> c33PrimeV2c32PrimeV3;

	//mcl::EcT<mcl::bn256::Fp>::add(c33PrimeV2c32PrimeV3, c33PrimeV2, c32PrimeV3);
	mcl::EcT<mcl::bn256::Fp>::add(fm_b, c31PrimeV1, c33PrimeV2);
}

const void show4(const mcl::bn256::G1& fm_g1, const mcl::bn256::G2& fm_g2, const mcl::bn256::G1& fm_randomizedIssuerMessage, const mcl::bn256::G1& fm_c1Prime, const mcl::bn256::Fr& fm_randomMessage, const mcl::bn256::Fr& fm_issuerRandomMessage, const mcl::bn256::G1& fm_b, mcl::bn256::G1& fm_sigma1, mcl::bn256::G1& fm_sigma2) noexcept
{
	mcl::EcT<mcl::bn256::Fp>::mul(fm_sigma2, fm_g1, fm_randomMessage);

	mcl::EcT<mcl::bn256::Fp> mRm;

	mcl::EcT<mcl::bn256::Fp>::mul(mRm, fm_randomizedIssuerMessage, fm_randomMessage);

	mcl::EcT<mcl::bn256::Fp> bMRm;

	mcl::EcT<mcl::bn256::Fp>::add(bMRm, fm_b, mRm);
	mcl::EcT<mcl::bn256::Fp>::add(fm_sigma1, fm_c1Prime, bMRm);

	//// TEST

	mcl::bn256::Fp12 mRmG2;

	mcl::bn256::BN::pairing(mRmG2, mRm, fm_g2);

	mcl::EcT<mcl::bn256::Fp2> g2m;

	mcl::EcT<mcl::bn256::Fp2>::mul(g2m, fm_g2, fm_issuerRandomMessage);

	mcl::bn256::Fp12 sigma2G2m;

	mcl::bn256::BN::pairing(sigma2G2m, fm_sigma2, g2m);

	if (mRmG2 == sigma2G2m)
	{
		std::cout << "show4, e(M^rm, g2) == e(sigma2, g2^m)" << std::endl;
	}
	else
	{
		std::cout << "show4, e(M^rm, g2) != e(sigma2, g2^m)" << std::endl;
	}

	//// TEST
}

const void show5(const mcl::bn256::Fr& fm_userPrivateKey, const mcl::bn256::G2& fm_yO, const std::vector<std::string>& fm_nonRevealedAttributeHashes, const mcl::bn256::Fr& fm_randomMessage, mcl::bn256::G2& fm_a) noexcept
{
	const auto iHP = computeInverseHashProduct(fm_nonRevealedAttributeHashes);

	mcl::bn256::Fr skuSh;

	mcl::bn256::Fr::mul(skuSh, fm_userPrivateKey, iHP);

	mcl::bn256::Fr skuShRm;

	mcl::bn256::Fr::div(skuShRm, skuSh, fm_randomMessage);

	mcl::EcT<mcl::bn256::Fp2>::mul(fm_a, fm_yO, skuShRm);
}

const void show(const mcl::bn256::G1& fm_g1, const mcl::bn256::G2& fm_g2, const mcl::bn256::G1& fm_h1, const mcl::bn256::Fr& fm_issuerRandomMessage, const mcl::bn256::G1& fm_issuerRandomizedMessage, const mcl::bn256::Fr& fm_userPrivateKey, const mcl::bn256::G2& fm_yO, const std::vector<std::string>& fm_nonRevealedAttributeNameAndHashes, const mcl::bn256::G1& fm_c1, const mcl::bn256::G2& fm_c2, const std::vector<mcl::bn256::G1>& fm_c3, mcl::bn256::G1& fm_c1Prime, mcl::bn256::G2& fm_c2Prime, std::vector<mcl::bn256::G1>& fm_c3Prime, mcl::bn256::G1& fm_sigma1, mcl::bn256::G1& fm_sigma2, mcl::bn256::G2& fm_a, std::vector<mcl::bn256::G2>& fm_w) noexcept
{
	show1(fm_g2, fm_h1, fm_c1, fm_c2, fm_c3, fm_c1Prime, fm_c2Prime, fm_c3Prime);

	const auto v = show2();

	mcl::EcT<mcl::bn256::Fp> b;

	show3(fm_c2Prime, fm_c3Prime, v, b, fm_w);

	mcl::bn256::Fr rM;
	rM.setRand(g_rg);

	show4(fm_g1, fm_g2, fm_issuerRandomizedMessage, fm_c1Prime, rM, fm_issuerRandomMessage, b, fm_sigma1, fm_sigma2);

	show5(fm_userPrivateKey, fm_yO, fm_nonRevealedAttributeNameAndHashes, rM, fm_a);
}

const bool verify(const mcl::bn256::G1& fm_h1, const mcl::bn256::G2& fm_g2, const mcl::bn256::G2& fm_c2Prime, const mcl::bn256::G1& fm_sigma1, const mcl::bn256::G1& fm_sigma2, const mcl::bn256::Fp12 fm_xO, const mcl::bn256::G1& fm_issuerPublicKey, const mcl::bn256::Fr& fm_issuerMessage, const std::vector<std::string>& fm_revealedAttributeNameAndHashes, const mcl::bn256::G2& fm_a, const std::vector<mcl::bn256::G2>& fm_w, const std::vector<mcl::bn256::G1>& fm_attributeGammaValues) noexcept
{
	mcl::EcT<mcl::bn256::Fp2> g2m;

	mcl::EcT<mcl::bn256::Fp2>::mul(g2m, fm_g2, fm_issuerMessage);

	mcl::bn256::Fp12 eSigma2G2M;

	mcl::bn256::BN::pairing(eSigma2G2M, fm_sigma2, g2m);

	mcl::bn256::Fp12 eSigma1G2;

	mcl::bn256::BN::pairing(eSigma1G2, fm_sigma1, fm_g2);

	mcl::bn256::Fp12 eArA;

	mcl::bn256::BN::pairing(eArA, fm_sigma2, fm_a);

	mcl::bn256::Fp12 eH1C2Prime;

	mcl::bn256::BN::pairing(eH1C2Prime, fm_h1, fm_c2Prime);

	mcl::bn256::Fp12 eGamma1H1Tho1W1;

	mcl::bn256::BN::pairing(eGamma1H1Tho1W1, fm_attributeGammaValues[0], fm_w[0]);

	mcl::bn256::Fp12 eGamma3H1Tho2W2;

	mcl::bn256::BN::pairing(eGamma3H1Tho2W2, fm_attributeGammaValues[2], fm_w[1]);

	/*mcl::bn256::Fp12 eGamma2H1Tho3W3;

	mcl::bn256::BN::pairing(eGamma2H1Tho3W3, fm_attributeGammaValues[1], fm_w[2]);*/

	mcl::bn256::Fp12 product2;

	mcl::bn256::Fp12::mul(product2, eGamma1H1Tho1W1, eGamma3H1Tho2W2);

	mcl::bn256::Fp12 product;

	mcl::bn256::Fp12::mul(product, product2, eSigma2G2M);

	mcl::bn256::Fp12 test2;

	mcl::bn256::Fp12::mul(test2, eH1C2Prime, product);

	mcl::bn256::Fp12 test1;

	mcl::bn256::Fp12::mul(test1, eArA, test2);

	mcl::bn256::Fp12 test;

	mcl::bn256::Fp12::mul(test, fm_xO, test1);

	std::cout << eSigma1G2 << std::endl;
	std::cout << test << std::endl;

	//// TEST

	if (pkuSisHsIG2 == eArA)
	{
		std::cout << "verify, e(pku^(so * H(S)-1), g2) == e(A, Ar)" << std::endl;
	}
	else
	{
		std::cout << "verify, e(pku^(so * H(S)-1), g2) != e(A, Ar)" << std::endl;
	}

	//// TEST

	if (eSigma1G2 == test)
	{
		return true;
	}

	return false;
}
