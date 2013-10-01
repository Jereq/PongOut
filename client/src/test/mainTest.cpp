#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>
#include <iostream>

#include "ICoreSystem.h"
#include "ResourceLoader/ResourceLoader.h"

struct cout_redirect {
    cout_redirect( std::streambuf * new_buffer ) 
        : old( std::cout.rdbuf( new_buffer ) )
    { }

    ~cout_redirect( ) {
        std::cout.rdbuf( old );
    }

private:
    std::streambuf * old;
};

BOOST_AUTO_TEST_CASE(TestCoreSystem)
{
	int argc = 1;
	char *argv[2];
	char testPath[] = "Fake/test/path/to/executable";
	argv[0] = testPath;

	boost::test_tools::output_test_stream output;
	{
		cout_redirect guard(output.rdbuf());
		BOOST_CHECK(ICoreSystem::init(argc, argv) == false);			// Fake path should not work
		BOOST_CHECK(output.is_equal("Error: Invalid argv[0], stop hacking!\n"));	// Check error message (stupid Windows)
	}

	int argc2 = boost::unit_test::framework::master_test_suite().argc;
	char** argv2 = boost::unit_test::framework::master_test_suite().argv;
	
	BOOST_REQUIRE(ICoreSystem::init(argc2, argv2));						// Init should work with valid path

	ICoreSystem::ptr coreSystem = ICoreSystem::getInstance();
	BOOST_REQUIRE(coreSystem.lock());									// Returns an instance

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

BOOST_AUTO_TEST_CASE(TestResourceLoader)
{
	int argc2 = boost::unit_test::framework::master_test_suite().argc;
	char** argv2 = boost::unit_test::framework::master_test_suite().argv;

	BOOST_REQUIRE(ICoreSystem::init(argc2, argv2));						// Init should work with valid path

	ICoreSystem::ptr coreSystem = ICoreSystem::getInstance();
	BOOST_REQUIRE(coreSystem.lock());

	std::vector<ResourceLoader::Resource> res;

	BOOST_CHECK_EQUAL((int)ResourceLoader::getResources(res, boost::filesystem::path()), (int)ResourceLoader::ErrorCode::DIRECTORY_NOT_FOUND);

	boost::filesystem::path texturePath = coreSystem.lock()->getRootDir() / "resources" / "textures";
	if (boost::filesystem::exists(texturePath))
	{
		BOOST_CHECK_EQUAL((int)ResourceLoader::getResources(res, texturePath), (int)ResourceLoader::ErrorCode::RESOURCE_FILE_NOT_FOUND);
	}
	else
	{
		BOOST_ERROR("\"resources/textures\" does not exist.");
	}

	BOOST_CHECK_EQUAL((int)ResourceLoader::getResources(res, coreSystem.lock()->getRootDir() / "resources"), (int)ResourceLoader::ErrorCode::SUCCESS);
}

