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

class Map {
public:
	Map();
	virtual ~Map();
	void initialize(glm::vec2 _playAreaSize, float _frameThickness,
						std::string _bgTextureName, std::string _frameTextureName	);
	void update(double _dt, IGraphics::ptr _graphics);
	std::string getTextureName();
	std::string getFrameTextureName();
	glm::vec2 getSize();
	bool loadMap(std::string mapName, std::vector<CommonTypes::Block> _blocks, GraphicsComponent::ptr gc, InputComponent::ptr ic, PhysicsComponent::ptr pc);

	bool addObject(Paddle::ptr _paddle);
	bool addObject(Ball::ptr _ball);

public:

	void setPlayAreaBounds(glm::vec2 _size);

	std::vector<BlockC::ptr>		blocks;
	std::vector<Paddle::ptr>	paddles;
	std::vector<Ball::ptr>		balls;

	std::string 			bgTextureName;
	std::string			frameTextureName;
	glm::vec2		playAreaSize;

	float 			frameThickness;
};

#endif /* MAP_H_ */
