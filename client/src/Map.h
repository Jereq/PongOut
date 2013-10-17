/*
 * Map.h
 *
 *
 *
 */

#ifndef MAP_H_
#define MAP_H_

#include "Block.h"
#include "MapResources.h"
#include "GraphicsComponent.h"
#include "InputComponent.h"
#include "PhysicsComponent.h"

#include "Paddle.h"
#include "Ball.h"

struct Level
{
	std::string name;
	std::string texture;
	glm::vec3 origo;
};

class Map {
public:
	Map();
	virtual ~Map();
	void initialize(glm::vec2 _playAreaSize, float _frameThickness,
						string _bgTextureName, string _frameTextureName	);
	void update(double _dt, IGraphics::ptr _graphics);
	string getTextureName();
	string getFrameTextureName();
	glm::vec2 getSize();
	bool loadMap(std::string mapName, GraphicsComponent::ptr gc, InputComponent::ptr ic, PhysicsComponent::ptr pc);

	bool addObject(Paddle::ptr _paddle);
	bool addObject(Ball::ptr _ball);

public:

	void setPlayAreaBounds(glm::vec2 _size);

	std::vector<Block::ptr>			blocks;
	std::vector<Paddle::ptr>	paddles;
	std::vector<Ball::ptr>		balls;

	string 			bgTextureName;
	string			frameTextureName;
	glm::vec2		playAreaSize;

	float 			frameThickness;
	Level level;
};

#endif /* MAP_H_ */
