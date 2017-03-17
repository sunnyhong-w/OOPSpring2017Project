#pragma once

#include "gamebehavior.h"

using namespace game_engine;
struct Tile;
struct TileSet;
struct TileMap;

struct TileSet
{
	TileSet();
	string image;
	Vector2I tileSize;
	int firstgid;
	int tilecount;
	vector<Tile> tiles;
};

struct TileMap
{
	int width;
	int height;
	vector<TileSet> tileSetList;
};

void from_json(const json& j, TileSet& ts);

struct Tile
{
	Tile() {};
	json properties;
	vector<CollisionInfo> object;
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