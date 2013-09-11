#include "Graphics.h"
#include "PongOutConfig.h"

#include <iostream>

int main(int argc, char* argv[])
{
	std::cout << "PongOut " << PongOut_VERSION_MAJOR << "." << PongOut_VERSION_MINOR << "." << PongOut_VERSION_PATCH << std::endl;

	Graphics g;
	if (!g.init())
	{
		std::cout << "Failed to initialize graphics." << std::endl;
		return 1;
	}

	g.destroy();

	return 0;
}
