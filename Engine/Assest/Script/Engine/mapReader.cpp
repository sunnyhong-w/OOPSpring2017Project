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
		LoadMap(j["load"].get<string>());
	}
}

void MapReader::LoadMap(string fname)
{
	for (auto v : tileList)
	{
		for (GameObject* gobj : v)
		{
			delete gobj;
		}

		v.clear();
	}

	tileList.clear();

	//-----

	string path = R"(.\Assest\Map\)" + fname + ".json";

	fstream file(path);

	if (file.good())
	{
		stringstream buffer;
		buffer << file.rdbuf();
		json jsondat = json::parse(buffer);
		tileMap = jsondat;

		for (json j : tileMap.layers)
		{
			vector<GameObject*> vec;

			if (j["type"].get<string>() == "tilelayer")
			{
				Vector2I pos = Vector2I::zero;
				int count = 0;

				for (int i = 0; i < tileMap.width * tileMap.height; i++)
				{
					int tindex = j["data"][i];

					if (tindex != 0)
					{
						for (TileSet tmp : tileMap.tileSetList)
						{
							if (tindex < tmp.firstgid || tindex > tmp.firstgid + tmp.tilecount)
								continue;

							GameObject *gobj = new GameObject();
							gobj->ParseJSON(GameObject::GetPrefrabs("Engine\\TileRenderer"));
							SpriteRenderer* SR = gobj->GetComponent<SpriteRenderer>();
							SR->LoadBitmapData(tmp.image);
							int tileindex = tindex - tmp.firstgid;

							Vector2I srcpos(tileindex% tmp.columns, tileindex / tmp.columns);
							SR->SetSourcePos(srcpos * tmp.tileSize);
							SR->SetSize(tmp.tileSize);

							if (tmp.tiles[tileindex].object.size() != 0)
							{
								Collider* cr = gobj->AddComponentOnce<Collider>();
								cr->collisionInfo = tmp.tiles[tileindex].object[0]; //先當他只會有一個collider
							}

							gobj->transform->position = pos.GetV2();

							vec.push_back(gobj);

							break;
						}
					}

					pos.x += tileMap.tileWidth;

					if (i % tileMap.width == tileMap.width - 1)
					{
						pos.x = 0;
						pos.y += tileMap.tileHeight;
					}

					count++;
				}

			}

			tileList.push_back(vec);

		}
	}
	else
	{
		string str = "ERROR : Map NOT FOUND when parse JSON :\n";
		str += "File : " + path;
		GAME_ASSERT(false, str.c_str());
	}
}

void MapReader::Draw(Vector2I campos)
{
	for (auto v : tileList)
	{
		for (GameObject* gobj : v)
		{
			gobj->Draw(campos);
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
	tm.tileWidth = j["tilewidth"];
	tm.tileHeight = j["tileheight"];
	tm.layers = j["layers"];
	for (TileSet ts : j["tilesets"])
		tm.tileSetList.push_back(ts);
}

void from_json(const json& j, TileSet& ts)
{
	string imgname = j["image"].get<string>();
	imgname = imgname.substr(10);
	imgname = imgname.substr(0,imgname.find_last_of("."));
	ts.image = imgname;
	ts.tileSize = Vector2I(j["tilewidth"], j["tileheight"]);
	ts.firstgid = j["firstgid"];
	ts.tilecount = j["tilecount"];
	ts.columns = j["columns"];
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