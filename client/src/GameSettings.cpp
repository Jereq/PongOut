#include "GameSettings.h"

GameSettings::GameSettings(void)
{
}

GameSettings::~GameSettings(void)
{
}

void GameSettings::setSuddenDeathTime(int _time)
{
	suddenDeathTime = _time;
}

int GameSettings::getSuddenDeathTime(void)
{
	return suddenDeathTime;
}

void GameSettings::setBallSpeed(float _speed)
{
	ballSpeed = _speed;
}

float GameSettings::getBallSpeed(void)
{
	return ballSpeed;
}

void GameSettings::setFOW(bool _fow)
{
	fogOfWar = _fow;
}

bool GameSettings::getFOW(void)
{
	return fogOfWar;
}

void GameSettings::setPOW(bool _pow)
{
	powerUps = _pow;
}

bool GameSettings::getPOW(void)
{
	return powerUps;
}

void GameSettings::setMapId(int _id)
{
	mapId = _id;
}

int GameSettings::getMapId(void)
{
	return mapId;
}

void GameSettings::setNumOfLives(int _lives)
{
	numOfLives = _lives;
}

int GameSettings::getNumOfLives(void)
{
	return numOfLives;
}