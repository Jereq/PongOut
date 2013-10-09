#pragma once
#include <iostream>
#include <string>

#include "ICoreSystem.h"

class HitBox
{
public:
	HitBox(glm::vec3& _position, glm::vec2& _size);
	~HitBox();
	bool intersectSphere(std::string& _side, glm::vec3* _ballPos, float* _radius);
	bool intersectBox(HitBox* _hitBox);
	void calcBounds();
	
	glm::vec3 getTopLeft();
	glm::vec3 getTopRight();
	glm::vec3 getBottomLeft();
	glm::vec3 getBottomRight();

private:
	glm::vec3&		position;
	glm::vec2&		size;
	
	glm::vec3		topLeft,
					topRight,
					bottomLeft,
					bottomRight;
};