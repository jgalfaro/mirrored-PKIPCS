#include <map>

#include <mcl/bn256.hpp>

#include <mcl/fp.hpp>
#include <mcl/gmp_util.hpp>
#include <mcl/ecparam.hpp>
#include <cybozu/random_generator.hpp>

#include <mcl/fp.hpp>

#if CYBOZU_CPP_VERSION >= CYBOZU_CPP_VERSION_CPP11
#include <random>
std::random_device g_rg;
#else
#include <cybozu/random_generator.hpp>
cybozu::RandomGenerator g_rg;
#endif

using namespace mcl::bn256;

int main(int argc, char **argv)
{
	/***** SETUP *****/

	const char *aa = "12723517038133731887338407189719511622662176727675373276651903807414909099441";
	const char *ab = "4168783608814932154536427934509895782246573715297911553964171371032945126671";
	const char *ba = "13891744915211034074451795021214165905772212241412891944830863846330766296736";
	const char *bb = "7937318970632701341203597196594272556916396164729705624521405069090520231616";


	mcl::bn::CurveParam cp = mcl::bn::CurveFp254BNb;

	bn256init(cp);

	G1 Q(-1, 1);
	G2 P(Fp2(aa, ab), Fp2(ba, bb));

	Fp g1N;
	g1N.setRand(g_rg);


	std::cout << "setup g1N: " << g1N << std::endl;

	G1 g1;
	BN::mapToG1(g1, g1N);

	std::cout << "setup g1: " << g1 << std::endl;

	Fp g2a;
	g2a.setRand(g_rg);

	Fp g2b;
	g2b.setRand(g_rg);

	Fp2 g2N = Fp2(g2a, g2b);
	std::cout << "setup g2N: " << g2N << std::endl;

	G2 g2;
	BN::mapToG2(g2, g2N);
	std::cout << "setup g2: " << g2 << std::endl;

	Fr alpha;
	alpha.setRand(g_rg);
	std::cout << "setup alpha: " << alpha << std::endl;

	// Compute h1
	G1 h1;
	G1::mul(h1, g1, alpha);
	std::cout << "setup h1: " << h1 << std::endl;

	// Compute h2
	Fr moinsAlpha;
	Fr::inv(moinsAlpha, alpha);

	G2 h2;
	G2::mul(h2, g2, moinsAlpha);
	std::cout << "setup h2: " << h2 << std::endl;


	/***** KEYGEN *****/

	// Compute user public key elements
	Fr sk1;
	sk1.setRand(g_rg);
	std::cout << "keygen sk1: " << sk1 << std::endl;

	G1 x1;
	G1::mul(x1, g1, sk1);
	std::cout << "keygen x1 = g1 * sk1: " << x1 << std::endl;

	Fp12 y1b;
	BN::pairing(y1b, g1, g2);
	//std::cout << "keygen y1b: " << y1b << std::endl;

	Fp12 y1;
	Fp12::pow(y1, y1b, sk1);
	std::cout << "keygen y1 = e(g1, g2)^sk1: " << y1b << std::endl;

	// Compute issuer private key
	Fr skis;
	skis.setRand(g_rg);
	std::cout << "keygen skis: " << skis << std::endl;

	G1 xkis;
	G1::mul(xkis, g1, skis);
	std::cout << "keygen xkis = g1^skis: " << skis << std::endl;

	Fp12 xisb;
	BN::pairing(xisb, g1, g2);
	//std::cout << "keygen xisb: " << y1b << std::endl;

	Fp12 xis;
	Fp12::pow(xis, xisb, skis);
	std::cout << "keygen xis = e(g1, g2)^skis: " << y1b << std::endl;

	G2 yis;
	G2::mul(yis, h2, skis);
	std::cout << "keygen yis = h2^skis: " << y1b << std::endl;

	/***** ISSUE *****/

	// Generate randomly
	Fr r;
	r.setRand(g_rg);
	std::cout << "issue r: " << r << std::endl;

	Fr moinsR;
	Fr::inv(moinsR, r);

	// Compute c2
	G1 c2;
	G1::mul(c2, g1, moinsR);
	std::cout << "issue c2 = g1^-r: " << c2 << std::endl;

	// Compute c3
	G2 c3;
	G2::mul(c3, g2, r);
	std::cout << "issue c3 = g2^r: " << c3 << std::endl;

	std::string attribute1 = { "attribute1" };
	std::string hash1 = "e57e21836b0f4ab2c2238c6aaaa2eebd0b5d1e7c5e3a62b4d7ba4f243b2fdfdc";

	std::string attribute2 = { "attribute2" };
	std::string hash2 = "748090159650086fc380c609bdc7c66d34840428ffa356b2d327a10b8afb1179";

	std::string attribute3 = { "attribute3" };
	std::string hash3 = "7253f6e42af7406c9b09dd8b9c2d2cac1f5d8719d014a7bdfe57dd4f2bc02f0e";

        /*
	// Compute hs
	Fp ha1;
	ha1.setMsg(hash1);
	//BN::mapToG1(g1, a1);
	std::cout << "issue H(a1): " << ha1 << std::endl;

	Fp ha2;
	ha2.setMsg(hash2);
	//BN::mapToG1(g1, a1);
	std::cout << "issue H(a2): " << ha2 << std::endl;

	Fp ha3;
	ha3.setMsg(hash3);
	//BN::mapToG1(g1, a1);
	std::cout << "issue H(a3): " << ha3 << std::endl;

	Fp ha1ha2;
	Fp::mul(ha1ha2, ha1, ha2);

	Fp hs;
	Fp::mul(hs, ha1ha2, ha3);
	std::cout << "issue H(s): " << hs << std::endl;

	Fp hsI;
	Fp::inv(hsI, hs);
	std::cout << "issue H(s)^-1: " << hsI << std::endl;

	// Compute c1
	G1 xuHs;
	G1::mul(xuHs, x1, hsI);
	std::cout << "issue Xu^(H(s)^-1): " << xuHs << std::endl;

	G1 h1r;
	G1::mul(h1r, h1, r);
	std::cout << "issue h1^r: " << h1r << std::endl;

	G1 c11;
	G1::add(c11, xkis, xuHs);
        */
	return 0;
}
