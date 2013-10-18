/*
 * Block.h
 *
 *
 *
 */

#ifndef BLOCKC_H_
#define BLOCKC_H_

#include <iostream>
#include <string>

#include "glm/glm.hpp"
#include "GameObject.h"
#include "CommonTypes.h"

static const glm::vec2 BLOCKSIZE = glm::vec2(64,32);


class BlockC : public GameObject
{
public:
	BlockC();
	~BlockC();

	bool initialize(const CommonTypes::Block& _blockData, float _rotation, GraphicsComponent::ptr _graphicsComponent);
	void setCollided(const int _health);
	virtual void	update(double _dt);
	void			setInPlay(bool _inPlayState)override;
	typedef std::shared_ptr<BlockC> ptr;
private:
	int health;
	std::vector<std::string> textures;

	friend class PhysicsComponent;
};

#endif /* BLOCK_H_ */
