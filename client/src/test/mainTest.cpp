#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include "ICoreSystem.h"

BOOST_AUTO_TEST_CASE(CoreSystem)
{
	int argc = 1;
	char *argv[2];
	char testPath[] = "Fake/test/path/to/executable";
	argv[0] = testPath;

	BOOST_CHECK(ICoreSystem::init(argc, argv) == false);				// Fake path should not work

	int argc2 = boost::unit_test::framework::master_test_suite().argc;
	char** argv2 = boost::unit_test::framework::master_test_suite().argv;

	BOOST_REQUIRE(ICoreSystem::init(argc2, argv2));						// Init should work with valid path

	ICoreSystem::ptr coreSystem = ICoreSystem::getInstance();
	BOOST_REQUIRE(coreSystem.lock());											// Returns an instance

	boost::filesystem::path rootDir = coreSystem.lock()->getRootDir();
	BOOST_CHECK(boost::filesystem::exists(rootDir));					// Extracted path exists
	BOOST_CHECK(boost::filesystem::is_directory(rootDir));				// Extracted path is a directory

	ICoreSystem::destroy();
	BOOST_CHECK(!ICoreSystem::getInstance().lock());					// destroy should null the pointer

	BOOST_REQUIRE(ICoreSystem::init(argc2, argv2));						// Creating instance works repeatedly
	coreSystem = ICoreSystem::getInstance();

	double time = coreSystem.lock()->getTime();
	BOOST_CHECK(time > 0.0);											// Time is always positive
	BOOST_CHECK(time <= coreSystem.lock()->getTime());							// Time is always increasing

	BOOST_CHECK(coreSystem.lock()->windowIsClosing() == false);				// No graphics, no closing

	IGraphics::ptr graphics = coreSystem.lock()->getGraphics();
	BOOST_REQUIRE(graphics);											// Graphics getter returns instance
	BOOST_CHECK(graphics == coreSystem.lock()->getGraphics());					// CoreSystem returns same instance

	ICoreSystem::destroy();
}
