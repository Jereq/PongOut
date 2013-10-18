#pragma once

#include <glm/glm.hpp>
namespace CommonTypes
{
	struct Block
	{
		int id, type, health;
		float x, y, z;
		std::vector<std::string> textures;

		int getSize()
		{
			int res = 0;

			for (std::string s : textures)
			{
				res += sizeof(char) * s.length() + sizeof(uint16_t);
			}

			res += sizeof(int) * 3;
			res += sizeof(float) * 3;
			res += sizeof(uint16_t); //size of vector

			return res;
		}

	};

	struct GameInitInfo
	{
		int mapID, ballSpeed, paddleSpeed, suddenDeathTime;
		char fogOfWar, powerUps;
	};

	struct Paddle
	{
		glm::vec2 pos, vel;
		uint32_t id;
	};

	struct Ball
	{
		glm::vec2 pos, vel;
		uint32_t id;
	};

	struct PlayerMatchInfo
	{
		uint32_t userID, score;
		Paddle paddle;
		Ball ball;
	};
}