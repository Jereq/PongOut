#pragma once

#include <glm/glm.hpp>
namespace CommonTypes
{
	struct Block
	{
		int id, type, health;
		glm::vec2 pos;
		char inPlay;
		std::vector<std::string> textures;

		int getSize()
		{
			int res = 0;

			for (std::string s : textures)
			{
				res += sizeof(char) * s.length() + sizeof(uint16_t);
			}

			res += sizeof(int) * 3;
			res += sizeof(glm::vec2);
			res += sizeof(char);
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
		char inPlay;
	};

	struct Ball
	{
		glm::vec2 pos, vel;
		uint32_t id;
		float radius;
		char inPlay;
	};

	struct PlayerMatchInfo
	{
		uint32_t userID, score;
		Paddle paddle;
		Ball ball;
	};

	struct MatchResult
	{
		uint32_t winnerID, winnerScore, loserScore;
	};
}