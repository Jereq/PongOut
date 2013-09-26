#include "Game.h"

#include "PongOutConfig.h"
//#include <chrono>
#include <iostream>
#include <stdio.h>
//#include <thread>
#include <vector>

Game::Game(ICoreSystem* _system)
	: system(_system)
{
	
}

bool cbIntersect(glm::vec3 _bp, float _br, glm::vec3 _rp, glm::vec2 _rb)
{
	float tx = _bp.x;
	float ty = _bp.y;
	float ox = _rp.x - _rb.x / 2.0;
	float oy = _rp.y - _rb.y / 2.0;

	if(tx < ox)
		tx = ox;
	if(tx > ox + _rb.x)
		tx = ox + _rb.x;
	if(ty < oy)
		ty = oy;
	if(ty > oy + _rb.y)
		ty = oy + _rb.y;

	bool result = ((_bp.x - tx) * (_bp.x - tx) + (_bp.y - ty) * (_bp.y - ty )) < _br * _br;
	return result;

}

void fbIntersect(glm::vec3& _bp, glm::vec2 _bd, glm::vec3& _bv)
{
	if(_bp.x + _bd.x / 2.0 > 1.0)
	{
		_bp.x = 1.0 - _bd.x / 2.0;
		_bv.x *= -1;
	}
	if(_bp.x - _bd.x / 2.0 < -1.0)
	{
		_bp.x = -1.0 + _bd.x / 2.0;
		_bv.x *= -1;
	}
	if(_bp.y + _bd.y / 2.0 > 1.0)
	{
		_bp.y = 1.0 - _bd.y / 2.0;
		_bv.y *= -1;
	}
	if(_bp.y - _bd.y / 2.0 < -1.0)
	{
		_bp.y = -1.0 + _bd.y / 2.0;
		_bv.y *= -1;
	}
}

void rbIntersect(glm::vec3& _bp, glm::vec2 _bd, glm::vec3& _bv, std::vector<glm::vec3>& _r, glm::vec2 _rd)
{
		for(int i = 0; i < _r.size(); i++)
		{
			if(cbIntersect(_bp + _bv, _bd.x, _r[i], _rd))
			{
				_bv.y *= -1;

				std::vector<glm::vec3>::iterator it = _r.begin() + i;
				_r.erase(it, it+1);

				break;
			}
		}
}

void Game::run()
{
	std::cout << "PongOut " << PongOut_VERSION_MAJOR << "." << PongOut_VERSION_MINOR << "." << PongOut_VERSION_PATCH << std::endl;

	
	IGraphics* graphics = system->getGraphics();
	if (!graphics->init())
	{
		printf("Failed to initialize renderer\n");
		return;
	}

	graphics->loadResources(system->getRootDir() / "resources");

	double previousTime = system->getTime();
	double currentTime = previousTime;
	double deltaTime = 0.f;

	system->getSounds()->load("Bubble3");

	std::cout << "Starting to run" << std::endl;

	glm::vec3 testVector = glm::vec3(0,-1 + 0.06125,0);
	glm::vec2 testDim = glm::vec2(0.25, 0.125);

	glm::vec3 bVector = glm::vec3(0.0,-0.75,0);
	glm::vec3 b2Vector = glm::vec3(0.0,0.75,0);
	glm::vec2 bDim = glm::vec2(0.06125 * 0.6,0.06125);
	glm::vec3 bVelocity = glm::vec3(0.0125, 0.0125, 0);
	glm::vec3 rVector = glm::vec3(-1.0,0,0);
	glm::vec2 rDim = glm::vec2(0.125, 0.125);

	glm::vec3 fakeVector = glm::vec3(0,1 - 0.06125,0);
	glm::vec3 b2Velocity = -bVelocity;
	std::vector<glm::vec3> blocks;
	for(int y=0; y < 3; y++)
	{
		for(int x=0; x < 13; x++ )
		{
			blocks.push_back( glm::vec3(x*rDim.x-0.75, y*rDim.y, 0)); 
		}
	}
	bool rExist = true;

	while (!system->windowIsClosing())
	{
		system->pollEvents();
		float dt = system->getTime();
		float bspeed = 0.075f;
		float pspeed = 0.001f;
		system->getSounds()->play("Bubble3");

		if( system->isKeyPress('D') )
			testVector = testVector + glm::vec3(1,0,0) * pspeed * dt;
		else if(system->isKeyPress('A'))
			testVector = testVector - glm::vec3(1,0,0) * pspeed * dt;

		if(testVector.x + testDim.x / 2.0 > 1.0)
			testVector.x = 1.0 - testDim.x / 2.0;
		else if(testVector.x - testDim.x / 2.0 < -1.0)
			testVector.x = -1.0 + testDim.x / 2.0;

		bVector = bVector + bVelocity * bspeed * dt;
		b2Vector = b2Vector + b2Velocity * bspeed * dt;

		fbIntersect(bVector, bDim, bVelocity);
		fbIntersect(b2Vector, bDim, b2Velocity);

		if(fakeVector.x < bVector.x)
			fakeVector += glm::vec3(1,0,0) * pspeed * dt;
		else if(fakeVector.x > bVector.x)
			fakeVector -= glm::vec3(1,0,0) * pspeed * dt;

		if( cbIntersect(bVector + bVelocity, bDim.x, testVector, testDim) )
		{
			bVelocity.y *= -1;
		}
		else if(cbIntersect(bVector + bVelocity, bDim.x, fakeVector, testDim) )
		{
			bVelocity.y *= -1;
		}
		if( cbIntersect(b2Vector + b2Velocity, bDim.x, testVector, testDim) )
		{
			b2Velocity.y *= -1;
		}
		else if(cbIntersect(b2Vector + b2Velocity, bDim.x, fakeVector, testDim) )
		{
			b2Velocity.y *= -1;
		}

		rbIntersect(bVector, bDim, bVelocity, blocks, rDim);
		rbIntersect(b2Vector, bDim, b2Velocity, blocks, rDim);

		static float x,y;
		x+=-0.001f;
		y+=-0.001f;

		graphics->addRectangle(testVector, glm::vec2(0.25f, 0.125f), 0.f, "blocks/dark_magenta_01");
		graphics->addRectangle(fakeVector, glm::vec2(0.25f, 0.125f), 0.f,"blocks/pink_01");

		for(int i = 0; i < blocks.size(); i++)
			graphics->addRectangle(blocks[i], rDim, 10.f, "blocks/orange_01");

		graphics->addRectangle(glm::vec3(0, 0, 1.f), glm::vec2(2,2), 10.f, "blocks/background_prototype");
		graphics->addRectangle(bVector, bDim, 1.0f, "player/ball_prototype");
		graphics->addRectangle(b2Vector, bDim, 1.0f, "player/ball_prototype");
		const static float FRAMES_PER_SECOND = 60.f;
		const static float FRAME_TIME = 1.f / FRAMES_PER_SECOND;

		previousTime = currentTime;
		currentTime = system->getTime();
		deltaTime = currentTime - previousTime;

		//if (deltaTime < FRAME_TIME)
		//{
		//	//std::chrono::milliseconds dura((unsigned int)((FRAME_TIME - deltaTime) * 1000.f));
		//	//std::this_thread::sleep_for(dura);
		//}

		graphics->drawFrame();
	}

	graphics->destroy();
}
