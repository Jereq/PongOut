#include <boost/test/unit_test.hpp>

#include <Sound/SoundManager.h>


BOOST_AUTO_TEST_CASE(SoundEngineTest)
{
	SoundManager soundM;
	BOOST_REQUIRE(soundM.initialize());

	//int argc = boost::unit_test::framework::master_test_suite().argc;
	char** argv = boost::unit_test::framework::master_test_suite().argv;

	boost::filesystem::path resPath = boost::filesystem::path(argv[0]).parent_path();
	boost::filesystem::path fakePath = "fake/test/resources";

	BOOST_CHECK(!soundM.loadSounds(fakePath)); //Should fail to load sounds, wrong path.
	BOOST_CHECK(soundM.loadSounds(resPath / "resources")); //Should load sounds.
	BOOST_CHECK(!soundM.loadSounds(resPath / "resources")); //Should fail since resources are already loaded.

	soundM.shutdown();

	BOOST_CHECK(soundM.initialize()); //Try to reinitialize the the sound manager.	
	BOOST_CHECK(!soundM.initialize()); //Should return false since already initialized.

	soundM.shutdown();
}
