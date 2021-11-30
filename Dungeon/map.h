#ifndef __MAP_H__
#define __MAP_H__

#include "entity.h"
#include <vector>

class BackBuffer;
class MapObject;

class Map
{
public:
	Map(int width, int height);
	~Map();

	void InitializeNewMap();

	void Draw(BackBuffer* backBuffer);

	int GetMapLevel();
	void SetMapLevel(int level);
	void IncreaseLevel();

	static void InitialiseSprites(BackBuffer* backBuffer);
	static void DestroySprites();
private:
	Map();
	Map(const Map& map);
	void InitializeMap();
	void GenerateMap();
	void DestroyMap();

private:
	static Sprite* sm_pWallTopLeftSprite;
	static Sprite* sm_pWallTopMidSprite;
	static Sprite* sm_pWallTopRightSprite;

	static Sprite* sm_pWallSideMidLeftSprite;
	static Sprite* sm_pWallSideMidRightSprite;

	static Sprite* sm_pWallSideFrontLeftSprite;
	static Sprite* sm_pWallSideFrontRightSprite;
	static Sprite* sm_pWallMidSprite;

	static Sprite* sm_pFloorSprites[];

	int m_width;
	int m_height;

	int m_mapLevel;

	std::vector<std::vector<MapObject*>> bgObjects;
};

#endif // !__MAP_H__