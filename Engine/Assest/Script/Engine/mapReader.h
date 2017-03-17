#pragma once

#include "gamebehavior.h"

using namespace game_engine;
struct TileObject;
struct TileMapStruct;

struct TileMapStruct
{
	TileMapStruct();
	string image;
	Vector2I tileSize;
	int firstgid;
	vector<TileObject> tiles;
};

struct TileObject
{
	json properties;
};

class MapReader : public GameBehaviour
{
public:
	MapReader(GameObject* gobj) : GameBehaviour(gobj) {}
	void ParseJSON(json j) override;

private:
	vector<GameObject*> tileList;
	vector<json> tilePrefrabs;
	
};