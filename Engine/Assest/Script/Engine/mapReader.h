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


struct TileObject
{
	TileObject(string imgname, json inproperties);
	string image;
	json properties;
};

struct ObjectSet
{
	int firstgid;
	int tilecount;
	vector<TileObject> objects;
};

void from_json(const json& j, ObjectSet& ts);

struct TileMap
{
	int width;
	int height;
	int tileHeight;
	int tileWidth;
	vector<TileSet> tileSetList;
	vector<ObjectSet> objectSetList;
	json layers;
};

void from_json(const json& j, TileSet& ts);

struct Tile
{
	Tile() {};
	json properties;
    json colliderProperties;
	vector<CollisionInfo> object;
};

class MapReader : public GameBehaviour
{
public:
	~MapReader();
	MapReader(GameObject* gobj) : GameBehaviour(gobj) {}
	void ParseJSON(json j) override;
    void ParseProperties(GameObject* gobj, string filename, json prop);
	void LoadMap(string str);
	void Draw(Vector2I campos) override;
	void Update() override;

private:
	TileMap tileMap;
};