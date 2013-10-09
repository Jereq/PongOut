#pragma once

#include "ICoreSystem.h"
#include "Ball.hpp"
#include <iostream>
#include <array>

class Collision
{
public:
	Collision(std::vector<Ball> &_ballList);
	~Collision();

private:
	
	std::vector<Ball>						&ballList;
	std::vector<std::array<Ball, 2>>		collisionVector;	

public:

	void intersectSphere();
	void ballOnRectBounce();
};