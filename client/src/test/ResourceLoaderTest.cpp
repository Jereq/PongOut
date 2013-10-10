#include <boost/test/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>
#include <iostream>

#include <CoreSystem/ICoreSystem.h>
#include "ResourceLoader/ResourceLoader.h"

BOOST_AUTO_TEST_SUITE(ResourceLoaderTestSuite)

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
		BOOST_ERROR(texturePath.string() + " does not exist.");
	}

	BOOST_CHECK_EQUAL((int)ResourceLoader::getResources(res, coreSystem.lock()->getRootDir() / "resources"), (int)ResourceLoader::ErrorCode::SUCCESS);
}

BOOST_AUTO_TEST_SUITE_END()
