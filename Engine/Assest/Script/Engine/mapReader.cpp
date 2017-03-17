#include "StdAfx.h"
#include "Engine.h"
#include "mapReader.h"

#include<fstream>
using namespace std;

using namespace game_engine;

void MapReader::ParseJSON(json j)
{
	json emptyjson;
	SpriteRenderer* SR = this->gameObject->AddComponentOnce<SpriteRenderer>();
	SR->ParseJSON(emptyjson);

	if (j.find("load") != j.end())
	{
		string path = R"(.\Assest\Map\)" + j["load"].get<string>()+ ".json";

		fstream file(path);

		if (file.good())
		{
			stringstream buffer;
			buffer << file.rdbuf();
			json jsondat = json::parse(buffer);
			TileMap tileMap = jsondat;
			
		}
		else
		{
			string str = "ERROR : Map NOT FOUND when parse JSON :\n";
			str += "File : " + path;
			GAME_ASSERT(false, str.c_str());
		}
	}
}

TileSet::TileSet()
{
	image = "";
	tileSize = Vector2I::null;
	firstgid = -1;
}

void from_json(const json& j, TileMap& tm)
{
	tm.width = j["width"];
	tm.height = j["height"];

	for (TileSet ts : j["tilesets"])
		tm.tileSetList.push_back(ts);
}

void from_json(const json& j, TileSet& ts)
{
	string imgname = j["image"].get<string>();
	imgname = imgname.substr(10);
	ts.image = imgname;
	ts.tileSize = Vector2I(j["tilewidth"], j["tileheight"]);
	ts.firstgid = j["firstgid"];
	ts.tilecount = j["tilecount"];
	for (int i = 0; i < ts.tilecount; i++)
		ts.tiles.push_back(j["tiles"][to_string(i)]["objectgroup"]);
}

void from_json(const json& j, Tile& to)
{
	if(j.find("properties") != j.end())
		to.properties = j["properties"];

	if (j.find("objects") != j.end())
	{
		for (json jobj : j["objects"])
		{
			Vector2I max = Vector2I::zero;
			Vector2I min = Vector2I::one * 9999;
			for (Vector2I v : jobj["polygon"])
			{
				if (v.x > max.x)
					max.x = v.x;
				if (v.y > max.y)
					max.y = v.y;
				if (v.x < min.x)
					min.x = v.x;
				if (v.y < min.y)
					min.y = v.y;
			}

			CollisionInfo ci;
			ci.size = max - min;
			ci.offset = min;

			to.object.push_back(ci);
		}
	}
}