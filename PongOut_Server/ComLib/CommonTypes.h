

namespace CommonTypes
{
	struct Block
	{
		int id, health;
		float x, y, z;
		std::vector<std::string> textures;
	};

	struct GameInitInfo
	{
		int mapID, ballSpeed, paddleSpeed, suddenDeathTime;
		char fogOfWar, powerUps;
	};
}