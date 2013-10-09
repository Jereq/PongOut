#include "Collision.h"


Collision::Collision(std::vector<Ball> &_ballList)
	: ballList(_ballList)
{
}

Collision::~Collision()
{
}

void Collision::intersectSphere()
{
	for (unsigned int i=0; i<ballList.size(); i++)
	{
		for (unsigned int j = i+1; j<ballList.size(); i++)
		{
			glm::vec3 vectorBetween = ballList[j].getPos() - ballList[i].getPos();
			vectorBetween = vectorBetween * vectorBetween;
			float distanceBetween = glm::dot(vectorBetween, vectorBetween);

			if (ballList[i].getRadius() + ballList[j].getRadius() < distanceBetween)
			{
				std::array<Ball, 2> balls = {ballList[i], ballList[j]};
				collisionVector.push_back(balls);
			}
		}
	}
}

void Collision::ballOnRectBounce()
{

}