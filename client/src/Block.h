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

static const glm::vec2 BLOCKSIZE = glm::vec2(64,32);

struct BlockData
{
	int typeId;
	int health;
	glm::vec3 center;
};

using std::string;

class Block : public GameObject
{
public:
	Block();
	~Block();

	bool initialize(const BlockData& _blockData, float _rotation, const std::vector<std::string>& _textures, GraphicsComponent::ptr _graphicsComponent);
	bool canCollide;
	virtual void	update(double _dt);
	typedef std::shared_ptr<Block> ptr;
private:
	int health;
	std::vector<std::string> textures;

	friend class PhysicsComponent;
};

#endif /* BLOCK_H_ */
