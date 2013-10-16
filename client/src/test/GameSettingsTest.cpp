#include <boost/test/unit_test.hpp>

#include "GameSettings.h"

//////////////////////////////////////////////////////////////////////////
//Actually too easy for testing but did it anyway to learn how to test.
//////////////////////////////////////////////////////////////////////////
BOOST_AUTO_TEST_SUITE(GameSettingsTestSuite)

BOOST_AUTO_TEST_CASE(GameSettingsTest)
{
	GameSettings settings;

	settings.setSuddenDeathTime(1000);
	BOOST_CHECK(settings.getSuddenDeathTime() == 1000); //Check that timer is correct.
	
	settings.setSuddenDeathTime(1500);
	BOOST_CHECK(settings.getSuddenDeathTime() == 1500); //Check that timer is still correct.

	settings.setBallSpeed(10.f);
	BOOST_CHECK(settings.getBallSpeed() == 10.f); //Check that ball speed is correct.

	settings.setBallSpeed(15.f);
	BOOST_CHECK(settings.getBallSpeed() == 15.f); //Check that ball speed is still correct.
	
	settings.setMapId(2);
	BOOST_CHECK(settings.getMapId() == 2); //Check that id is correct.

	settings.setMapId(3);
	BOOST_CHECK(settings.getMapId() == 3); //Check that id is still correct.
	
	settings.setNumOfLives(3);
	BOOST_CHECK(settings.getNumOfLives() == 3); //Check that timer is correct.

	settings.setNumOfLives(4);
	BOOST_CHECK(settings.getNumOfLives() == 4); //Check that timer is still correct.

	settings.setFOW(false);
	BOOST_CHECK(!settings.getFOW()); //Game should start without FOW.

	settings.setFOW(true);
	BOOST_CHECK(settings.getFOW()); //Game should start with FOW.

	settings.setPOW(false);
	BOOST_CHECK(!settings.getPOW()); //Game should start without POW.

	settings.setPOW(true);
	BOOST_CHECK(settings.getPOW()); //Game should start with POW.
}

BOOST_AUTO_TEST_SUITE_END()