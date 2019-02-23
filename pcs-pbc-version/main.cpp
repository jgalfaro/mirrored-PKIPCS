/*
Telecom SudParis
2017, Pierre-Olivier Rocher
*/

#include <iostream>

#include <vector>
#include <pbc/pbc.h>
#include <gsl/gsl_linalg.h>

void thoI(int i, element_t fm_micro1, element_t fm_micro2, gsl_matrix *fm_M, pairing_t pairing, element_t& returnValue)
{
	element_t one;
	element_t two;
	element_t three;

	element_t first;
	element_t second;
	element_t third;

	element_t f1;

	element_init_Zr(one, pairing);
	element_init_Zr(two, pairing);
	element_init_Zr(three, pairing);

	element_init_Zr(first, pairing);
	element_init_Zr(second, pairing);
	element_init_Zr(third, pairing);

	element_init_Zr(f1, pairing);
	element_init_Zr(returnValue, pairing);

	element_set_si(one, gsl_matrix_get(fm_M, i, 0));
	element_set_si(two, gsl_matrix_get(fm_M, i, 1));
	element_set_si(three, gsl_matrix_get(fm_M, i, 2));

	element_mul(second, fm_micro1, two);
	element_mul(third, fm_micro2, three);

	element_add(f1, one, second);
	element_add(returnValue, f1, third);
}

int main(int argc, char **argv)
{
	pbc_param_t param;

	pairing_t pairing;

	pbc_param_init_a_gen(param, 160, 512);

	// Random generator
	/*pairing_init_set_str(pairing,
		"type a\n"
		"q 8780710799663312522437781984754049815806883199414208211028653399266475630880222957078625179422662221423155858769582317459277713367317481324925129998224791\n"
		"h 12016012264891146079388821366740534204802954401251311822919615131047207289359704531102844802183906537786776\n"
		"r 730750818665451621361119245571504901405976559617\n"
		"exp2 159\n"
		"exp1 107\n"
		"sign1 1\n"
		"sign0 1\0a b a b\n"
	);*/

	pairing_init_pbc_param(pairing, param);

	/***** SETUP *****/

	element_t g1, g2;
	element_t alpha, moinsAlpha;
	element_t h1, h2;

	// Initilization
	element_init_G1(g1, pairing);
	element_init_G2(g2, pairing);
	element_init_Zr(alpha, pairing);
	element_init_Zr(moinsAlpha, pairing);

	element_init_G1(h1, pairing);
	element_init_G2(h2, pairing);

	// Generate random g1
	element_random(g1);
	element_printf("setup, g1: %B\n", g1);

	// Generate random g2
	element_random(g2);
	element_printf("setup, g2: %B\n", g2);

	// Generate random alpha
	element_random(alpha);
	element_printf("setup, alpha: %B\n", alpha);

	// Compute h1
	element_pow_zn(h1, g1, alpha);
	element_printf("setup, h1 = g1^alpha: %B\n", h1);

	element_neg(moinsAlpha, alpha);

	// Compute h2
	element_pow_zn(h2, g2, moinsAlpha);
	element_printf("setup, h2 = g2^-alpha: %B\n", h2);

	/***** KEYGEN *****/

	element_t sk1, x1, y1b, y1;
	element_t skis, xkis, xisb, xis, yis;

	// Initilization
	element_init_Zr(sk1, pairing);
	element_init_Zr(skis, pairing);

	element_init_G1(x1, pairing);
	element_init_G1(xkis, pairing);

	element_init_G2(yis, pairing);

	element_init_GT(y1b, pairing);
	element_init_GT(y1, pairing);
	element_init_GT(xisb, pairing);
	element_init_GT(xis, pairing);
	
	// Compute user public key elements
	element_random(sk1);
	element_printf("keygen, sk1: %B\n", sk1);

	element_pow_zn(x1, g1, sk1);
	element_printf("keygen, x1 = g1^sk1: %B\n", x1);

	element_pairing(y1b, g1, g2);
	element_pow_zn(y1, y1b, sk1);
	element_printf("keygen, y1 = e(g1, g2)^sk1: %B\n", y1);

	// Compute issuer private key
	element_random(skis);
	element_printf("keygen, skis: %B\n", skis);

	element_pow_zn(xkis, g1, skis);
	element_printf("keygen, xkis = g1^skis: %B\n", xkis);

	// Compute issuer public key elements
	element_pairing(xisb, g1, g2);
	element_pow_zn(xis, xisb, skis);
	element_printf("keygen, xis = e(g1, g2)^skis: %B\n", xis);

	element_pow_zn(yis, h2, skis);
	element_printf("keygen, yis = h2^skis: %B\n", yis);

	/***** ISSUE *****/

	std::string attribute1 = {"attribute1"};
	std::string hash1 = "e57e21836b0f4ab2c2238c6aaaa2eebd0b5d1e7c5e3a62b4d7ba4f243b2fdfdc";

	std::string attribute2 = { "attribute2" };
	std::string hash2 = "748090159650086fc380c609bdc7c66d34840428ffa356b2d327a10b8afb1179";

	std::string attribute3 = { "attribute3" };
	std::string hash3 = "7253f6e42af7406c9b09dd8b9c2d2cac1f5d8719d014a7bdfe57dd4f2bc02f0e";

	element_t r, rCarre, rCube, moinsR;
	element_t c2, c3;
	element_t ha1, ha2, ha3, ha1ha2, hs, hsI, xuHs;
	element_t h1r;
	element_t c1, c11;
	element_t u1, u2, u3;
	element_t u1r, u2r, u3r;

	element_init_Zr(r, pairing);
	element_init_Zr(rCarre, pairing);
	element_init_Zr(rCube, pairing);

	element_init_Zr(moinsR, pairing);

	element_init_Zr(u1, pairing);
	element_init_Zr(u2, pairing);
	element_init_Zr(u3, pairing);
	element_init_Zr(u1r, pairing);
	element_init_Zr(u2r, pairing);
	element_init_Zr(u3r, pairing);

	element_init_G1(c2, pairing);
	element_init_G2(c3, pairing);

	element_init_Zr(ha1, pairing);
	element_init_Zr(ha2, pairing);
	element_init_Zr(ha3, pairing);
	element_init_Zr(ha1ha2, pairing);
	element_init_Zr(hs, pairing);
	element_init_Zr(hsI, pairing);
	element_init_G1(xuHs, pairing);
	element_init_G1(h1r, pairing);
	element_init_G1(c1, pairing);
	element_init_G1(c11, pairing);

	// Genreate randomly
	element_random(r);
	element_printf("issue, r: %B\n", r);

	element_mul(rCarre, r, r);
	element_mul(rCube, rCarre, r);

	element_neg(moinsR, r);

	// Compute c2
	element_pow_zn(c2, g1, moinsR);
	element_printf("issue, c2 = g1^-r: %B\n", c2);

	// Compute c3
	element_pow_zn(c3, g2, r);
	element_printf("issue, c3 = g2^r: %B\n", c3);

	// Load hashes
	element_from_hash(ha1, (void*)hash1.c_str(), hash1.size());
	element_printf("issue, H(a1): %B\n", ha1);

	element_from_hash(ha2, (void*)hash2.c_str(), hash2.size());
	element_printf("issue, H(a2): %B\n", ha2);

	element_from_hash(ha3, (void*)hash3.c_str(), hash3.size());
	element_printf("issue, H(a3): %B\n", ha3);
	
	// Compute hs
	//element_from_hash(hs, (void*)hash1.c_str(), hash1.size());
	element_mul(ha1ha2, ha1, ha2);
	element_mul(hs, ha1ha2, ha3);
	element_printf("issue, H(s): %B\n", hs);

	element_invert(hsI, hs);
	element_printf("issue, H(s)^-1: %B\n", hsI);

	// Compute c1
	element_pow_zn(xuHs, x1, hsI);
	element_printf("issue, Xu^(H(s)^-1): %B\n", xuHs);

	element_pow_zn(h1r, h1, r);
	element_printf("issue, h1^r: %B\n", h1r);

	element_mul(c11, xkis, xuHs);
	element_mul(c1, c11, h1r);
	element_printf("issue, c1: %B\n", c1);

	// Create ui
	element_random(u1);
	element_printf("issue, u1: %B\n", u1);

	element_random(u2);
	element_printf("issue, u2: %B\n", u2);

	element_random(u3);
	element_printf("issue, u3: %B\n", u3);

	// Compute u1^r
	element_pow_zn(u1r, u1, r);
	element_printf("issue, c4,1 = u1^r: %B\n", u1r);

	element_pow_zn(u2r, u2, r);
	element_printf("issue, c4,2 = u2^r: %B\n", u2r);

	element_pow_zn(u3r, u3, r);
	element_printf("issue, c4,3 = u3^r: %B\n", u3r);

	/***** OBTAIN *****/

	element_t c1g2;
	element_t xuhsig2;
	element_t h1c3;

	element_t p1, p2;

	element_init_GT(c1g2, pairing);
	element_init_GT(xuhsig2, pairing);
	element_init_GT(h1c3, pairing);
	element_init_GT(p1, pairing);
	element_init_GT(p2, pairing);

	// Compute c1g2
	element_pairing(c1g2, c1, g2);
	element_printf("obtain, e(c1, g2): %B\n", c1g2);

	element_pairing(xuhsig2, xuHs, g2);
	element_printf("obtain, e(Xu^H(s)^-1, g2): %B\n", xuhsig2);

	element_pairing(h1c3, h1, c3);
	element_printf("obtain, e(h1, c3): %B\n", h1c3);

	element_mul(p1, xis, xuhsig2);
	element_mul(p2, p1, h1c3);
	element_printf("obtain, Xis * e(Xu^H(s)^-1, g2) * e(h1, c3): %B\n", p2);

	/***** SHOW *****/

	element_t rPrime;
	element_t rPlusRPrime, rPlusRPrimeCarre, rPlusRPrimeCube;
	element_t rPlusRPrimeNeg;
	element_t xisXuHsI;
	element_t h1rrPrime;
	element_t c1Prime;
	element_t c2Prime;
	element_t c3Prime;
	element_t c41Prime, c42Prime, c43Prime;

	element_init_Zr(rPrime, pairing);
	element_init_Zr(rPlusRPrime, pairing);
	element_init_Zr(rPlusRPrimeCarre, pairing);
	element_init_Zr(rPlusRPrimeCube, pairing);
	element_init_Zr(rPlusRPrimeNeg, pairing);

	element_init_G1(h1rrPrime, pairing);
	element_init_G1(xisXuHsI, pairing);
	element_init_G1(c1Prime, pairing);
	element_init_G1(c2Prime, pairing);

	element_init_Zr(c41Prime, pairing);
	element_init_Zr(c42Prime, pairing);
	element_init_Zr(c43Prime, pairing);

	element_init_G2(c3Prime, pairing);

	element_random(rPrime);
	element_printf("show, rPrime: %B\n", rPrime);

	element_add(rPlusRPrime, r, rPrime);
	element_mul(rPlusRPrimeCarre, rPlusRPrime, rPlusRPrime);
	element_mul(rPlusRPrimeCube, rPlusRPrimeCarre, rPlusRPrime);

	element_mul(xisXuHsI, xis, xuHs);
	element_pow_zn(h1rrPrime, h1, rPlusRPrime);
	element_mul(c1Prime, xisXuHsI, h1rrPrime);
	element_printf("show, c'1 = xis * Xu^(H(s)^-1) * h1^(r + r'): %B\n", c1Prime);

	element_neg(rPlusRPrimeNeg, rPlusRPrime);
	element_pow_zn(c2Prime, g1, rPlusRPrimeNeg);
	element_printf("show, c'2 = g1^-(r + r'): %B\n", c2Prime);

	element_pow_zn(c3Prime, g2, rPlusRPrime);
	element_printf("show, c'3 = g2^(r + r'): %B\n", c3Prime);

	element_pow_zn(c41Prime, u1, rPlusRPrime);
	element_printf("show, c'4, 1 = u1^(r + r'): %B\n", c41Prime);

	element_pow_zn(c42Prime, u2, rPlusRPrime);
	element_printf("show, c'4, 2 = u2^(r + r'): %B\n", c42Prime);

	element_pow_zn(c43Prime, u3, rPlusRPrime);
	element_printf("show, c'4, 3 = u3^(r + r'): %B\n", c43Prime);

	std::vector<double> m = { 0, -1, 0, 1, 1, 1, 0, 0, -1 };
	std::vector<double> res = { 1, 0, 0 };

	gsl_matrix_view gM = gsl_matrix_view_array(&m.front(), 3, 3);
	gsl_vector_view gRes = gsl_vector_view_array(&res.front(), 3);

	gsl_vector *x = gsl_vector_alloc(3);

	gsl_permutation *p = gsl_permutation_alloc(3);

	int s = 0;

	gsl_linalg_LU_decomp(&gM.matrix, p, &s);

	gsl_linalg_LU_solve(&gM.matrix, p, &gRes.vector, x);

	printf("v = \n");
	gsl_vector_fprintf(stdout, x, "%g");

	gsl_permutation_free(p);
	gsl_vector_free(x);

	element_t v1, v2, v3;
	element_t w1, w2, w3;

	element_init_Zr(v1, pairing);
	element_init_Zr(v2, pairing);
	element_init_Zr(v3, pairing);

	element_from_bytes(v1, (unsigned char*)"1");
	element_from_bytes(v2, (unsigned char*)"-1");
	element_from_bytes(v3, (unsigned char*)"0");

	element_init_G2(w1, pairing);
	element_init_G2(w2, pairing);
	element_init_G2(w3, pairing);

	element_pow_zn(w1, c3Prime, v1);
	element_printf("show, w1 = c'3^v1: %B\n", w1);

	element_pow_zn(w2, c3Prime, v2);
	element_printf("show, w2 = c'3^v2: %B\n", w2);

	element_pow_zn(w3, c3Prime, v3);
	element_printf("show, w3 = c'3^v3: %B\n", w3);

	element_t c41PrimeV1, c43PrimeV2, c42PrimeV3;
	element_t c41PrimeV1c43PrimeV2;
	element_t b;

	element_init_Zr(c41PrimeV1, pairing);
	element_init_Zr(c43PrimeV2, pairing);
	element_init_Zr(c42PrimeV3, pairing);
	element_init_Zr(c41PrimeV1c43PrimeV2, pairing);
	element_init_Zr(b, pairing);

	element_pow_zn(c41PrimeV1, c41Prime, v1);
	element_pow_zn(c43PrimeV2, c43Prime, v2);
	element_pow_zn(c42PrimeV3, c42Prime, v3);

	element_mul_zn(c41PrimeV1c43PrimeV2, c41PrimeV1, c43PrimeV2);
	element_mul_zn(b, c41PrimeV1c43PrimeV2, c42PrimeV3);
	element_printf("show, B = (u'ro(i))^vi: %B\n", b);

	element_t rm, rmI, message;
	element_t c1PrimeB;
	element_t rmMessage;
	element_t g1RmMessage;
	element_t rho1, rho2;
	element_t skuHsI, skuHsIRm;
	element_t A;

	element_init_Zr(rm, pairing);
	element_init_Zr(rmI, pairing);
	element_init_Zr(message, pairing);
	element_init_Zr(rmMessage, pairing);
	element_init_Zr(skuHsI, pairing);
	element_init_Zr(skuHsIRm, pairing);

	element_init_G1(c1PrimeB, pairing);
	element_init_G1(g1RmMessage, pairing);
	element_init_G1(rho1, pairing);
	element_init_G1(rho2, pairing);

	element_init_G2(A, pairing);

	element_random(rm);
	element_printf("show, rm: %B\n", rm);

	element_random(message);
	element_printf("show, m: %B\n", message);

	element_mul_zn(c1PrimeB, c1Prime, b);
	element_printf("show, C'1 * B: %B\n", c1PrimeB);

	element_mul_zn(rmMessage, rm, message);
	element_printf("show, rm * m: %B\n", rmMessage);

	element_pow_zn(g1RmMessage, g1, rmMessage);
	element_printf("show, g1^(rm * m): %B\n", g1RmMessage);

	element_mul(rho1, c1PrimeB, g1RmMessage);
	element_printf("show, rho1 = C'1 * B * g1^(rm*m): %B\n", rho1);

	element_pow_zn(rho2, g1, rm);
	element_printf("show, rho2 = g1^rm: %B\n", rho2);

	element_mul_zn(skuHsI, sk1, hsI);
	element_invert(rmI, rm);
	element_mul_zn(skuHsIRm, skuHsI, rmI);

	element_pow_zn(A, g2, skuHsIRm);
	element_printf("show, A = g2^(sk1 * H(S)^-1 * rm^-1): %B\n", A);

	/***** VERIFY *****/

	element_t Ar;
	element_t micro1, micro2;
	element_t thoi;

	element_init_G1(Ar, pairing);

	element_init_Zr(micro1, pairing);
	element_init_Zr(micro2, pairing);

	element_pow_zn(Ar, rho2, hsI);
	element_printf("verify, Ar = rho2^(H(S)^-1): %B\n", Ar);

	element_random(micro1);
	element_random(micro2);

	thoI(0, micro1, micro2, &gM.matrix, pairing, thoi);

	element_t eRho1G2;
	element_t eArA;
	element_t eCPrime2h2;
	element_t g2m;
	element_t eRho2G2M;

	element_t x1seArA;
	element_t x1seArAeCPrime2h2;

	element_init_GT(eRho1G2, pairing);
	element_init_GT(eArA, pairing);
	element_init_GT(eCPrime2h2, pairing);
	element_init_G2(g2m, pairing);
	element_init_GT(eRho2G2M, pairing);

	element_init_GT(x1seArA, pairing);
	element_init_GT(x1seArAeCPrime2h2, pairing);

	element_pairing(eRho1G2, rho1, g2);
	element_pairing(eArA, Ar, A);
	element_pairing(eCPrime2h2, c2Prime, h2);

	element_pow_zn(g2m, g2, message);

	element_pairing(eRho2G2M, rho2, g2m);

	element_mul(x1seArA, xis, eArA);
	element_mul(x1seArAeCPrime2h2, x1seArA, eCPrime2h2);

	/***** INSPEC *****/

	// First part
	element_t C2PrimeAlpha;
	element_t wEtoile;
	element_t ewEtoileg2;
	element_t XisI;
	element_t ewEtoileg2XisI;

	element_init_G1(C2PrimeAlpha, pairing);
	element_init_G1(wEtoile, pairing);
	element_init_GT(ewEtoileg2, pairing);
	element_init_GT(XisI, pairing);
	element_init_GT(ewEtoileg2XisI, pairing);

	element_pow_zn(C2PrimeAlpha, c2Prime, alpha);
	element_mul(wEtoile, c1Prime, C2PrimeAlpha);

	element_pairing(ewEtoileg2, wEtoile, g2);

	element_invert(XisI, xis);
	element_mul(ewEtoileg2XisI, ewEtoileg2, XisI);
	element_printf("insepc, e(w*, g2) * Xis^-1: %B\n", ewEtoileg2XisI);

	// Second part
	element_t moinsHs;
	element_t XuMoinsHs;
	element_t eXuMoinsHsg2;

	element_init_Zr(moinsHs, pairing);
	element_init_G1(XuMoinsHs, pairing);
	element_init_GT(eXuMoinsHsg2, pairing);

	element_neg(moinsHs, hs);

	element_pow_zn(XuMoinsHs, x1, moinsHs);

	element_pairing(eXuMoinsHsg2, XuMoinsHs, g2);
	element_printf("insepc, e(Xu^(-H(S), g2): %B\n", eXuMoinsHsg2);

	/***** CLEAN SHOW *****/

	element_clear(rPrime);
	element_clear(rPlusRPrime);
	element_clear(rPlusRPrimeNeg);
	element_clear(xisXuHsI);
	element_clear(h1rrPrime);
	element_clear(c1Prime);
	element_clear(c2Prime);
	element_clear(c3Prime);
	element_clear(c41Prime);

	/***** CLEAN OBTAIN *****/

	element_clear(c1g2);
	element_clear(xuhsig2);
	element_clear(h1c3);
	element_clear(p1);
	element_clear(p2);

	/***** CLEAN ISSUE *****/

	element_clear(r);
	element_clear(moinsR);
	element_clear(c2);
	element_clear(c3);
	element_clear(hs);
	element_clear(hsI);
	element_clear(xuHs);
	element_clear(h1r);
	element_clear(c1);
	element_clear(c11);
	element_clear(u1r);

	/***** CLEAN KEYGEN *****/

	element_clear(sk1);
	element_clear(x1);
	element_clear(y1b);
	element_clear(y1);
	element_clear(skis);
	element_clear(xkis);
	element_clear(xisb);
	element_clear(xis);
	element_clear(yis);

	/***** CLEAN SETUP *****/

	element_clear(g1);
	element_clear(g2);
	element_clear(alpha);
	element_clear(moinsAlpha);
	element_clear(h1);
	element_clear(h2);

	pairing_clear(pairing);

	return EXIT_SUCCESS;
}
 
