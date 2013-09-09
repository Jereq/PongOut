#include <iostream>

#include "PongOutConfig.h"

int main(int argc, char* argv[])
{
	std::cout << argv[0] << " " << PongOut_VERSION_MAJOR << "." << PongOut_VERSION_MINOR << "." << PongOut_VERSION_BUGFIX << std::endl;
	std::cout << "Hello World!" << std::endl;

	return 0;
}
