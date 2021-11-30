#define RANDOM(min, max) rand() % (max - min + 1) + min

#include "map.h"
#include "backbuffer.h"
#include "mapobject.h"
#include "sprite.h"

Sprite* Map::sm_pWallTopLeftSprite;
Sprite* Map::sm_pWallTopMidSprite;
Sprite* Map::sm_pWallTopRightSprite;

Sprite* Map::sm_pWallSideMidLeftSprite;
Sprite* Map::sm_pWallSideMidRightSprite;

Sprite* Map::sm_pWallSideFrontLeftSprite;
Sprite* Map::sm_pWallSideFrontRightSprite;
Sprite* Map::sm_pWallMidSprite;

Sprite* Map::sm_pFloorSprites[8];

Map::Map(int width, int height)
: m_width(0)
, m_height(0)
, m_mapLevel(1)
{
	if (width > 0)
	{
		this->m_width = width;
	}

	if (height > 0)
	{
		this->m_height = height;
	}
}

Map::~Map()
{
	DestroyMap();
}

void Map::InitializeNewMap()
{
	DestroyMap();
	InitializeMap();
	GenerateMap();
}

void Map::InitializeMap()
{
	for (int height = 0; height < this->m_height; ++height)
	{
		std::vector<MapObject*> objects;

		for (int width = 0; width < this->m_width; ++width)
		{
			objects.push_back(new MapObject());
		}

		bgObjects.push_back(objects);
	}
}

void Map::GenerateMap()
{
	for (int height = 0; height < this->m_height; ++height)
	{
		for (int width = 0; width < this->m_width; ++width)
		{
			if (width == 0)
			{
				if (height == 0)
				{
					bgObjects[height][width]->Initialise(sm_pWallTopLeftSprite);
				}
				else if (height == this->m_height - 1)
				{
					bgObjects[height][width]->Initialise(sm_pWallSideFrontLeftSprite);
				}
				else
				{
					bgObjects[height][width]->Initialise(sm_pWallSideMidLeftSprite);
				}
			}
			else if (width == this->m_width - 1)
			{
				if (height == 0)
				{
					bgObjects[height][width]->Initialise(sm_pWallTopRightSprite);
				}
				else if (height == this->m_height - 1)
				{
					bgObjects[height][width]->Initialise(sm_pWallSideFrontRightSprite);
				}
				else
				{
					bgObjects[height][width]->Initialise(sm_pWallSideMidRightSprite);
				}
			}
			else
			{
				if (height == 0)
				{
					bgObjects[height][width]->Initialise(sm_pWallTopMidSprite);
				}
				else if (height == 1 || height == this->m_height - 1)
				{
					bgObjects[height][width]->Initialise(sm_pWallMidSprite);
				}
				else if (height == this->m_height - 2)
				{
					bgObjects[height][width]->Initialise(sm_pWallTopMidSprite);
				}
				else
				{
					bgObjects[height][width]->Initialise(sm_pFloorSprites[RANDOM(0, 7)]);
				}
			}

			bgObjects[height][width]->SetPosition(width * 32.f, height * 32.f);
		}
	}
}

void Map::DestroyMap()
{
	for each (std::vector<MapObject*> objects in bgObjects)
	{
		for each (MapObject* object in objects)
		{
			delete object;
			object = 0;
		}

		objects.clear();
		objects.shrink_to_fit();
	}

	bgObjects.clear();
	bgObjects.shrink_to_fit();
}

void Map::Draw(BackBuffer * backBuffer)
{
	for (int height = 0; height < this->m_height; ++height)
	{
		for (int width = 0; width < this->m_width; ++width)
		{
			bgObjects[height][width]->Draw(*backBuffer);
		}
	}
}

int Map::GetMapLevel()
{
	return this->m_mapLevel;
}

void Map::SetMapLevel(int level)
{
	if (level > 0)
	{
		this->m_mapLevel = level;
	}
}

void Map::IncreaseLevel()
{
	this->m_mapLevel++;
}

void Map::InitialiseSprites(BackBuffer* backBuffer)
{
	sm_pWallTopLeftSprite = backBuffer->CreateSprite("assets\\imgs\\dungeon\\frames\\wall_side_top_left.png", 2);
	sm_pWallTopMidSprite = backBuffer->CreateSprite("assets\\imgs\\dungeon\\frames\\wall_top_mid.png", 2);
	sm_pWallTopRightSprite = backBuffer->CreateSprite("assets\\imgs\\dungeon\\frames\\wall_side_top_right.png", 2);

	sm_pWallSideMidLeftSprite = backBuffer->CreateSprite("assets\\imgs\\dungeon\\frames\\wall_side_mid_left.png", 2);
	sm_pWallSideMidRightSprite = backBuffer->CreateSprite("assets\\imgs\\dungeon\\frames\\wall_side_mid_right.png", 2);

	sm_pWallSideFrontLeftSprite = backBuffer->CreateSprite("assets\\imgs\\dungeon\\frames\\wall_side_front_left.png", 2);
	sm_pWallSideFrontRightSprite = backBuffer->CreateSprite("assets\\imgs\\dungeon\\frames\\wall_side_front_right.png", 2);
	sm_pWallMidSprite = backBuffer->CreateSprite("assets\\imgs\\dungeon\\frames\\wall_mid.png", 2);

	for (int i = 0; i < 8; ++i)
	{
		char path[40];
		sprintf_s(path, "assets\\imgs\\dungeon\\frames\\floor_%i.png", i + 1);
		sm_pFloorSprites[i] = backBuffer->CreateSprite(path, 2);
	}
}

void Map::DestroySprites()
{
	delete sm_pWallTopLeftSprite;
	sm_pWallTopLeftSprite = 0;
	delete sm_pWallTopMidSprite;
	sm_pWallTopMidSprite = 0;
	delete sm_pWallTopRightSprite;
	sm_pWallTopRightSprite = 0;

	delete sm_pWallSideMidLeftSprite;
	sm_pWallSideMidLeftSprite = 0;
	delete sm_pWallSideMidRightSprite;
	sm_pWallSideMidRightSprite = 0;

	delete sm_pWallSideFrontLeftSprite;
	sm_pWallSideFrontLeftSprite = 0;
	delete sm_pWallSideFrontRightSprite;
	sm_pWallSideFrontRightSprite = 0;
	delete sm_pWallMidSprite;
	sm_pWallMidSprite = 0;

	for (int i = 0; i < 8; ++i)
	{
		delete sm_pFloorSprites[i];
		sm_pFloorSprites[i] = 0;
	}
}
