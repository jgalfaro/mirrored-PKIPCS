
#include <iostream>

#include <stonepine/utilities/math/finiteFieldFp.h>
#include <stonepine/utilities/math/finiteFieldFp2.h>


using namespace Stonepine::Utilities;

int mainfiniteFieldFp(void)
{
	finiteFieldFp a, b, c, d;

	// Init
	a.set("214C703B4E25BB6E6729B1F388989CF38F33FB3685956D7F39DB604FCC4FB79C");
	b.set("1AC62B775FF7059B6E5A03A65097C6C73BD85A016CD6452D3E99C936216BBEEA");
	d.set("18A1A0AE10DBB14B870D9B01524B61794E0A15F7DA6BB2AAF9F4C985EDBB7685");

	// Add
	c.add(a, b);

	if (c.isEqual(d) == true)
	{
		std::cout << "OK" << std::endl;
	}
	else
	{
		std::cout << "NOT OK" << std::endl;
	}

	// Sub
	d.set(c);
	c.sub(c, d);

	if (c.isZero() == true)
	{
		std::cout << "OK" << std::endl;
	}
	else
	{
		std::cout << "NOT OK" << std::endl;
	}

	// Mul
	c.mul(a, b);
	d.set("EFAE50A68D3DC9F7A6D75A1D0CE556C074FCD360B4B58BD2AA489722F7378AD");

	if (c.isEqual(d) == true)
	{
		std::cout << "OK" << std::endl;
	}
	else
	{
		std::cout << "NOT OK" << std::endl;
	}

	/*d.setOne();

	if (c.isEqual(d) == false)
	{
		std::cout << "OK" << std::endl;
	}
	else
	{
		std::cout << "NOT OK" << std::endl;
	}*/

	// Random
	a.random();
	b.random();

	// Inv
	c.mul(a, b);
	b.inv(b);
	c.mul(c, b);
	d.inv(a);
	d.mul(a, d);

	if (c.isEqual(a) == true)
	{
		std::cout << "OK" << std::endl;
	}
	else
	{
		std::cout << "NOT OK" << std::endl;
	}

	if (d.isOne() == true)
	{
		std::cout << "OK" << std::endl;
	}
	else
	{
		std::cout << "NOT OK" << std::endl;
	}

	// Pow
	b.setOne();

	for (uint64_t i = 0; i < 1000; i++)
	{
		b.mul(b, a);
	}

	c.pow(a, "1000");

	if (b.isEqual(c) == true)
	{
		std::cout << "OK" << std::endl;
	}
	else
	{
		std::cout << "NOT OK" << std::endl;
	}

	return 0;
}

int mainfiniteFieldFp2(void)
{
	finiteFieldFp2 a, b, c, d;

	// Init
	a.set(std::make_pair("1C12C39A2AD14054EDC9EE504301127AFFEEAADC59A78B50FCFFED87AC6EB8BF", "20E1A922384561EA82602CD664D85D442DAC5D391E142ABB3CFEC2A095C22DF9"));
	b.set(std::make_pair("F1B91250A124F268B8239185B23B31EB25179A11A9A0398E61B701F7D4F7265", "20D206C5F7D007EDBA34A4B041622289D64F04CA28CEAC490619585AA14F7B2F"));
	d.set(std::make_pair("7BD59BA97A27FBD2AD60CD0173FC358353DE53D5C418EE8649AFDA729BE2B23", "1E42B4E392D45A19EE1EB6EE1F557D8C86F922C32EE2D702C497BAFB3711A927"));

	// Add
	c.add(a, b);

	if (c.isEqual(d) == true)
	{
		std::cout << "OK" << std::endl;
	}

	c.mul(a, b);

	return 0;
}

int main(void)
{
	return mainfiniteFieldFp2();
}