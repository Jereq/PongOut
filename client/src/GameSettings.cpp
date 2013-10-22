#include "GameSettings.h"

GameSettings::GameSettings(void)	: suddenDeathTime(120), ballSpeed(1.f), fogOfWar(false), powerUps(false), 
										mapId(0), numOfLives(3)
{
}

GameSettings::~GameSettings(void)
{
}

void GameSettings::setSuddenDeathTime(int _time)
{
	suddenDeathTime = _time;
}

int GameSettings::getSuddenDeathTime(void) const
{
	return suddenDeathTime;
}

void GameSettings::setBallSpeed(float _speed)
{
	ballSpeed = _speed;
}

float GameSettings::getBallSpeed(void) const
{
	return ballSpeed;
}

void GameSettings::setFOW(bool _fow)
{
	fogOfWar = _fow;
}

bool GameSettings::getFOW(void) const
{
	return fogOfWar;
}

void GameSettings::setPOW(bool _pow)
{
	powerUps = _pow;
}

bool GameSettings::getPOW(void) const
{
	return powerUps;
}

void GameSettings::setMapId(int _id)
{
	mapId = _id;
}

int GameSettings::getMapId(void) const
{
	return mapId;
}

void GameSettings::setNumOfLives(int _lives)
{
	numOfLives = _lives;
}

int GameSettings::getNumOfLives(void) const
{
	return numOfLives;
}