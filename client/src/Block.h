/*
 * Block.h
 *
 *
 *
 */

#ifndef BLOCK_H_
#define BLOCK_H_

#include <iostream>
#include <string>

#include "glm/glm.hpp"
#include "GameObject.h"

using std::string;

class Block : public GameObject
{
public:
	Block();
	~Block();

	bool initialize(const std::string& _id, glm::vec3 _center, glm::vec2 _size, float _rotation, 
					GraphicsComponent::ptr _graphicsComponent);
	bool canCollide;

	typedef std::shared_ptr<Block> ptr;
private:
	int health;

	virtual void	update(double _dt);
};

#endif /* BLOCK_H_ */
