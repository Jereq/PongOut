#include "stdafx.h"
#include "Referee.h"
#include "TMLReader.h"
#include "CreateGameResponse.h"


Referee::Referee(void)
{
}


Referee::~Referee(void)
{
}

void Referee::init( User::ptr _u0, User::ptr _u1, CommonTypes::GameInitInfo _info, int _refID)
{
	u0 = _u0;
	u1 = _u1;
	info = _info;

	u0->setReffereeID(_refID);
	u1->setReffereeID(_refID);

	std::vector<CommonTypes::Block> mapData;

	TMLReader tml;
	tml.readFile("../test.tml");

	TMLNODE r = tml.getRoot();
	TMLNODE mapNode, blockNode;
	for( TMLNODE n : tml.getRoot().nodes )
	{
		if(n.name == "Map")
		{
			for(std::pair<std::string,std::string> m : n.members)
			{
				if(m.first == "id")
				{
					int id = atoi(m.second.c_str());
					if(id == _info.mapID)
					{
						mapNode = n;
					}
				}
			}
		}
		if(n.name == "Blocks")
		{
			blockNode = n;
		}
	}

	int id;
	std::string name;
	std::vector<std::string> rows;
	for(std::pair<std::string,std::string> m : mapNode.members)
	{
		if(m.first == "id")
		{
			id = atoi(m.second.c_str());
		}
		else if(m.first == "name")
		{
			name = m.second;
		}
		else if(m.first == "row")
		{
			rows.push_back(m.second);
		}
	}

	int offX = 0;
	int offY = 0;
	int blockUniqeID = 0;

	for(std::string row : rows )
	{
		for(unsigned int i = 0; i < row.size(); i++)
		{
			std::string val = row.substr(i,1);
			if(val == "0")
				continue;

			CommonTypes::Block bd;
			bd.health = 0;
			std::vector<std::string> textures;
			for(TMLNODE n : blockNode.nodes)
			{
				for(std::pair<std::string,std::string> m : n.members)
				{
					if(m.first == "id")
					{
						std::string id = m.second;
						if(id == val)
						{
							float x,y;
							x = 880.0f / 2.0f + 64.0f * i;
							y = 800.0f / 2.0f + offY * 32.0f;

							bd.x = x;
							bd.y = y;
							bd.z = 0;
							bd.type = stoi(val);
							bd.id = blockUniqeID;
							blockUniqeID++;
						}
					}
					if(m.first == "texture")
					{
						textures.push_back(m.second);
					}
				}
			}
			bd.health = textures.size();
			bd.textures = textures;
			mapData.push_back(bd);
		}

		offY++;
	}	

	CreateGameResponse::ptr cgrp = CreateGameResponse::ptr(new CreateGameResponse());

	cgrp->setResponse(mapData, _info);

	u0->addMsgToMsgQueue(cgrp);
	u1->addMsgToMsgQueue(cgrp);

	int i = 0;
}
