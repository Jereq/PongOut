#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include "ICoreSystem.h"

BOOST_AUTO_TEST_CASE(CoreSystem)
{
	int argc = 1;
	char *argv[2];
	char testPath[] = "Fake/test/path/to/executable";
	argv[0] = testPath;

	BOOST_CHECK(ICoreSystem::createInstance(argc, argv) == nullptr);	// Fake path should not work

	int argc2 = boost::unit_test::framework::master_test_suite().argc;
	char** argv2 = boost::unit_test::framework::master_test_suite().argv;

	ICoreSystem* coreSystem = ICoreSystem::createInstance(argc2, argv2);
	BOOST_REQUIRE(coreSystem != nullptr);								// Returns an instance

	boost::filesystem::path rootDir = coreSystem->getRootDir();
	BOOST_CHECK(boost::filesystem::exists(rootDir));					// Extracted path exists
	BOOST_CHECK(boost::filesystem::is_directory(rootDir));				// Extracted path is a directory

	ICoreSystem::freeInstance(coreSystem);
	BOOST_CHECK(coreSystem == nullptr);									// Free instance should null the pointer

	coreSystem = ICoreSystem::createInstance(argc2, argv2);
	BOOST_REQUIRE(coreSystem != nullptr);								// Creating instance works repeatedly

	double time = coreSystem->getTime();
	BOOST_CHECK(time > 0.0);											// Time is always positive
	BOOST_CHECK(time <= coreSystem->getTime());							// Time is always increasing

	BOOST_CHECK(coreSystem->windowIsClosing() == false);				// No graphics, no closing

	IGraphics::ptr graphics = coreSystem->getGraphics();
	BOOST_REQUIRE(graphics);											// Graphics getter returns instance
	BOOST_CHECK(graphics == coreSystem->getGraphics());					// CoreSystem returns same instance

	ICoreSystem::freeInstance(coreSystem);
}
