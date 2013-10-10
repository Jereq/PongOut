/*
 * Map.h
 *
 *
 *
 */

#ifndef MAP_H_
#define MAP_H_

#include <malloc.h>
#include <iostream>
#include <string>
#include <vector>

#include "Block.h"
#include "MapResources.h"
#include "GraphicsComponent.h"
#include "Paddle.h"
#include "Ball.h"

class Map {
public:
	Map();
	virtual ~Map();
	void initialize(	glm::vec2 _playAreaSize, float _frameThickness,
						string _bgTextureName, string _frameTextureName							);
	void update(double _dt);
	string getTextureName();
	string getFrameTextureName();
	glm::vec2 getSize();
	void initBlockArray(int _size, GraphicsComponent::ptr _graphicsComponent);

	bool addObject(Paddle::ptr _paddle);
	bool addObject(Ball::ptr _ball);

public:
	
	void remBlockArray();
	void tempSetBlockPos();

	void setPlayAreaBounds(glm::vec2 _size);

	std::vector<Block::ptr>			blocks;
	std::vector<Paddle::ptr>	paddles;
	std::vector<Ball::ptr>		balls;

	string 			bgTextureName;
	string			frameTextureName;
	glm::vec2		playAreaSize;

	float 			frameThickness;
};

#endif /* MAP_H_ */
