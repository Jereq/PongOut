/*
 * Map.h
 *
 *
 *
 */

#ifndef MAP_H_
#define MAP_H_

#include "BlockC.h"
#include "MapResources.h"
#include "GraphicsComponent.h"
#include "InputComponent.h"
#include "PhysicsComponent.h"

#include "Paddle.h"
#include "Ball.h"
#include "CommonTypes.h"
#include "Server.h"

class Map 
{
public:
	Map();
	virtual ~Map();
	void initialize(glm::vec2 _playAreaSize, float _frameThickness,
					std::string _bgTextureName, std::string _frameTextureName);

	bool loadMap(std::string mapName, std::vector<CommonTypes::Block> _blocks, 
					GraphicsComponent::ptr gc, InputComponent::ptr ic, PhysicsComponent::ptr pc);
	void setUserInfo(const CommonTypes::PlayerMatchInfo& _me, const CommonTypes::PlayerMatchInfo& _op);

	bool addObject(Paddle::ptr _paddle);
	bool addObject(Ball::ptr _ball);

	void resetBall(const int _id);
	void setBallPosition(const int, glm::vec3 _position);	
	void setPlayAreaBounds(glm::vec2 _size);
	void setBallData(const CommonTypes::Ball& _bd);
	void setBlockData(const std::vector<CommonTypes::Block>& _bd);
	void setPaddleData(const CommonTypes::Paddle& _pd);
	void update(double _dt, IGraphics::ptr _graphics);

	std::string getTextureName();
	std::string getFrameTextureName();
	glm::vec2	getSize();
	Paddle::ptr	getPaddle(const int _paddleId);

private:
	std::vector<BlockC::ptr>	blocks;
	std::vector<Paddle::ptr>	paddles;
	std::vector<Ball::ptr>		balls;

	std::string	bgTextureName;
	std::string	frameTextureName;
	glm::vec2	playAreaSize;
	float 		frameThickness;

	CommonTypes::PlayerMatchInfo userInfos[2];

	friend class PhysicsComponent;
};

#endif /* MAP_H_ */
