#include "mainMcl.h"
#include "mainPcs.h"

int main(void)
{
	std::cout << " *** PCS version *** " << std::endl;

	mainPcs();

	std::cout << std::endl << " *** MCL version *** " << std::endl;

	mainMcl();

	return 0;
}
 
