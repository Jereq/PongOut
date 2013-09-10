#include <iostream>

#include "PongOutConfig.h"

int main(int argc, char* argv[])
{
	std::cout << "PongOut " << PongOut_VERSION_MAJOR << "." << PongOut_VERSION_MINOR << "." << PongOut_VERSION_PATCH << std::endl;
	std::cout << "Hello World!" << std::endl;
	std::cout << "This was compiled on a Linux machine." << std::endl;

	return 0;
}
