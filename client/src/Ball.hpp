#ifndef BALL_HPP_
#define BALL_HPP_

#include <string>
#include <vector>

#include "ICoreSystem.h"
#include "HitBox.h"

class Ball
{
private:
	float radius;
	std::string textureName;
	glm::vec2 velocity;
	glm::vec3 position;

	HitBox* hitBox;

public:
	Ball(glm::vec3 _position, float _radius, string _texturePath);
	~Ball();

	glm::vec3 getPos();
	void setPos(glm::vec3 _position);
	
	glm::vec2 getVelocity();
	void setVelocity(glm::vec2 _velocity);

	float getRadius();
};

#endif