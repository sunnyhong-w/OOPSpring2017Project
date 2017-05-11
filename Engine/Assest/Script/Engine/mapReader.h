#pragma once
#include "GameStatus.h"
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
    string tileSetName;
	vector<Tile> tiles;
};


struct TileObject
{
	TileObject(string imgname, json inproperties, vector<CollisionInfo> incollision);
	string image;
	json properties;
    vector<CollisionInfo> collision;
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
	Vector2I GetMapSize();
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
    static vector<MapReader*> readerList;

    ~MapReader() {};
	MapReader(GameObject* gobj) : GameBehaviour(gobj) {  }
	void ParseJSON(json j) override;
    void ParseProperties(GameObject* gobj, string filename, json prop);
	void LoadMap(string str);
    void Start() override;
	void Draw(Vector2I campos) override;
    void LateUpdate() override;
	TileMap tileMap;

private:
    GameObject* GenerateTile(string fname, int tindex);
	void CreateMapEdge(Transform* parent, Vector2 mapSize, string filename, Vector2I tileSize);
	void CreateEdgeObj(Transform* parent, string filename, string way, string type, Vector2 position, Vector2I colliderSize);

    Vector2 slicer;
};