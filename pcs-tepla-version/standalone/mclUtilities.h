/*
Télécom SudParis
2017, September
Pierre-Olivier Rocher
*/

#pragma once

#include <stonepine/architecture/architecture.h>

#ifdef __WINDOWS__
#ifdef LIBRARIES_EXPORTS
#define LIBRARIES_API __declspec(dllexport)
#else
#define LIBRARIES_API __declspec(dllimport)
#endif
#else
#define LIBRARIES_API
#endif

#include <chrono>
#include <ctime>
#include <map>

#include <mcl/bn256.hpp>
#include <mcl/fp.hpp>
#include <mcl/gmp_util.hpp>
#include <mcl/ecparam.hpp>

#include <cybozu/random_generator.hpp>

#define __TRUE_CALCULUS__ 0

using namespace mcl::bn256;

LIBRARIES_API const mcl::bn256::Fr computeInverseHashProduct(const std::vector<std::string> fm_attributesHashes);
LIBRARIES_API void setup(mcl::bn256::G1& fm_g1, mcl::bn256::G2& fm_g2, mcl::bn256::G1& fm_h1, mcl::bn256::G2& fm_h2) noexcept;
LIBRARIES_API void generateIssuerKeyPair(const mcl::bn256::G1& fm_g1, const mcl::bn256::G2& fm_g2, const mcl::bn256::G2& fm_h2, mcl::bn256::Fr& fm_privateKey, mcl::bn256::G1& fm_publicKey, mcl::bn256::Fp12& fm_xO, mcl::bn256::G2& fm_yO) noexcept;
LIBRARIES_API void generateAUserKeyPair(const mcl::bn256::G1& fm_h1, mcl::bn256::Fr& fm_privateKey, mcl::bn256::G1& fm_publicKey) noexcept;
LIBRARIES_API void issue(const mcl::bn256::G2& fm_g2, const mcl::bn256::G1& fm_h1, const mcl::bn256::Fr& fm_issuerPrivateKey, const mcl::bn256::G1& fm_issuerPublicKey, const mcl::bn256::G1& fm_userPublicKey, const mcl::bn256::Fr& fm_hsInverse, const std::vector<mcl::bn256::G1>& fm_gammaValues, mcl::bn256::G1& fm_c1, mcl::bn256::G2& fm_c2, std::vector<mcl::bn256::G1>& fm_c3) noexcept;
LIBRARIES_API bool obtain(const mcl::bn256::G1& fm_g1, const mcl::bn256::G2& fm_g2, const mcl::bn256::G1& fm_h1, const mcl::bn256::G1& fm_c1, const mcl::bn256::G2& fm_c2, const mcl::bn256::Fp12& fm_xO, const mcl::bn256::G2& fm_yO, const mcl::bn256::Fr& fm_userPrivateKey, const mcl::bn256::Fr& fm_hsInverse) noexcept;
LIBRARIES_API const void show1(const mcl::bn256::G2& fm_g2, const mcl::bn256::G1& fm_h1, const mcl::bn256::G1& fm_c1, const mcl::bn256::G2& fm_c2, const std::vector<mcl::bn256::G1>& fm_c3, mcl::bn256::G1& fm_c1Prime, mcl::bn256::G2& fm_c2Prime, std::vector<mcl::bn256::G1>& fm_c3Prime) noexcept;
LIBRARIES_API const std::vector<mcl::bn256::Fr> show2(void) noexcept;
LIBRARIES_API const void show3(const mcl::bn256::G2& fm_c2Prime, const std::vector<mcl::bn256::G1>& fm_c3Prime, const std::vector<mcl::bn256::Fr>& fm_v, mcl::bn256::G1& fm_b, std::vector<mcl::bn256::G2>& fm_wi) noexcept;
LIBRARIES_API const void show4(const mcl::bn256::G1& fm_g1, const mcl::bn256::G2& fm_g2, const mcl::bn256::G1& fm_randomizedIssuerMessage, const mcl::bn256::G1& fm_c1Prime, const mcl::bn256::Fr& fm_randomMessage, const mcl::bn256::Fr& fm_issuerRandomMessage, const mcl::bn256::G1& fm_b, mcl::bn256::G1& fm_sigma1, mcl::bn256::G1& fm_sigma2) noexcept;
LIBRARIES_API const void show5(const mcl::bn256::Fr& fm_userPrivateKey, const mcl::bn256::G2& fm_yO, const std::vector<std::string>& fm_nonRevealedAttributeHashes, const mcl::bn256::Fr& fm_randomMessage, mcl::bn256::G2& fm_a) noexcept;
LIBRARIES_API const void show(const mcl::bn256::G1& fm_g1, const mcl::bn256::G2& fm_g2, const mcl::bn256::G1& fm_h1, const mcl::bn256::Fr& fm_issuerRandomMessage, const mcl::bn256::G1& fm_issuerRandomizedMessage, const mcl::bn256::Fr& fm_userPrivateKey, const mcl::bn256::G2& fm_yO, const std::vector<std::string>& fm_nonRevealedAttributeNameAndHashes, const mcl::bn256::G1& fm_c1, const mcl::bn256::G2& fm_c2, const std::vector<mcl::bn256::G1>& fm_c3, mcl::bn256::G1& fm_c1Prime, mcl::bn256::G2& fm_c2Prime, std::vector<mcl::bn256::G1>& fm_c3Prime, mcl::bn256::G1& fm_sigma1, mcl::bn256::G1& fm_sigma2, mcl::bn256::G2& fm_a, std::vector<mcl::bn256::G2>& fm_w) noexcept;
LIBRARIES_API const bool verify(const mcl::bn256::G1& fm_h1, const mcl::bn256::G2& fm_g2, const mcl::bn256::G2& fm_c2Prime, const mcl::bn256::G1& fm_sigma1, const mcl::bn256::G1& fm_sigma2, const mcl::bn256::Fp12 fm_xO, const mcl::bn256::G1& fm_issuerPublicKey, const mcl::bn256::Fr& fm_issuerMessage, const std::vector<std::string>& fm_revealedAttributeNameAndHashes, const mcl::bn256::G2& fm_a, const std::vector<mcl::bn256::G2>& fm_w, const std::vector<mcl::bn256::G1>& fm_attributeGammaValues) noexcept;
