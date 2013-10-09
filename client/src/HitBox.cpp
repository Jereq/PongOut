#include "HitBox.h"


HitBox::HitBox(glm::vec3& _position, glm::vec2& _size)
	: position(_position),
	size(_size)
{	
	position		= _position;
	size			= _size;

	topLeft			= position - size.x/2 - size.y/2;
	topRight		= position + size.x/2 - size.y/2;
	bottomLeft		= position - size.x/2 + size.y/2;
	bottomRight		= position + size.x/2 + size.y/2;
}

HitBox::~HitBox()
{
}

void HitBox::calcBounds()
{
	topLeft			= position - size.x/2 - size.y/2;
	topRight		= position + size.x/2 - size.y/2;
	bottomLeft		= position - size.x/2 + size.y/2;
	bottomRight		= position + size.x/2 + size.y/2;
}

glm::vec3 HitBox::getTopLeft()
{
	return topLeft;
}

glm::vec3 HitBox::getTopRight()
{
	return topRight;
}

glm::vec3 HitBox::getBottomLeft()
{
	return bottomLeft;
}

glm::vec3 HitBox::getBottomRight()
{
	return bottomRight;
}

bool HitBox::intersectSphere(std::string& _side, glm::vec3* _ballPos, float* _radius)
{
	glm::vec3 distanceVec = *_ballPos - position;
	
	if(glm::length(distanceVec) < *_radius)
	{
		if(topLeft.y < _ballPos->y && bottomLeft.y > _ballPos->y && topLeft.x < _ballPos->x)
		{
			_side = "left";
			return true;
		}
		
		else if (topLeft.x < _ballPos->x && topRight.x > _ballPos->x && topLeft.y > _ballPos->y)
		{
			_side = "top";
			return true;
		}
		
		else if (bottomLeft.x < _ballPos->x && bottomRight.x > _ballPos->x && bottomLeft.y < _ballPos->y)
		{
			_side = "bottom";
			return true;
		}

		else if (topRight.y < _ballPos->y && bottomRight.y > _ballPos->y && topRight.x > _ballPos->x)
		{
			_side = "right";
			return true;
		}

		return false;
	}

	return false;
}

bool HitBox::intersectBox(HitBox* _hitBox)
{
	if(_hitBox->getTopLeft().x > topLeft.x && _hitBox->getTopRight().x < topRight.x)
	{
		if(_hitBox->getTopLeft().y > topLeft.y && _hitBox->getBottomLeft().y < bottomLeft.y)
		{
			return true;
		}
	}
	return false;
}