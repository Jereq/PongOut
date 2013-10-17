#pragma once

class GameSettings
{
private:
	//Time in seconds to sudden death, -1 equals no time.
	int suddenDeathTime;
	float ballSpeed;
	bool fogOfWar;
	bool powerUps;
	int mapId;
	int numOfLives;

public:
	GameSettings(void);
	~GameSettings(void);

	void setSuddenDeathTime(int _time);
	int getSuddenDeathTime(void) const;
	void setBallSpeed(float _speed);
	float getBallSpeed(void) const;
	void setFOW(bool _fow);
	bool getFOW(void) const;
	void setPOW(bool _fow);
	bool getPOW(void) const;
	void setMapId(int _id);
	int getMapId(void) const;
	void setNumOfLives(int _lives);
	int getNumOfLives(void) const;
};