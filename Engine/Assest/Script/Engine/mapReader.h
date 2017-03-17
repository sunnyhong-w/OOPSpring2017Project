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
	int columns;
	vector<Tile> tiles;
};

struct TileMap
{
	int width;
	int height;
	int tileHeight;
	int tileWidth;
	vector<TileSet> tileSetList;
	json layers;
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
	void LoadMap(string str);
	void Draw(Vector2I campos) override;

private:
	vector<vector<GameObject*>> tileList;
	TileMap tileMap;
	
};