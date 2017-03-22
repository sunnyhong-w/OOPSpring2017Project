#include "StdAfx.h"
#include "Engine.h"
#include "mapReader.h"

#include<fstream>
using namespace std;

using namespace game_engine;

MapReader::~MapReader()
{

}

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
    auto objlist = transform->GetChild();
	for (auto childtransform : objlist)
		Destroy(childtransform->gameObject);

	//-----

	string path = R"(.\Assest\Map\)" + fname + ".json";

	fstream file(path);

	if (file.good())
	{
		stringstream buffer;
		buffer << file.rdbuf();
		json jsondat = json::parse(buffer);
		tileMap = jsondat;

		int zindex = 0;
		for (json j : tileMap.layers)
		{
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
							if (tmp.firstgid == -1 || tindex < tmp.firstgid || tindex >= tmp.firstgid + tmp.tilecount)
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

							Instantiate(gobj);
                            gobj->transform->SetParent(this->transform);
                            gobj->transform->SetPosition(pos.GetV2());
							gobj->transform->SetZIndex(zindex);
							gobj->SetLayer(Layer::Tile);
                            
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
			else if (j["type"].get<string>() == "objectgroup")
			{
				json objectJSONList = j["objects"];
				for (auto obj : objectJSONList)
				{
					int tindex = obj["gid"];

					for (ObjectSet tmp : tileMap.objectSetList)
					{
						if (tmp.firstgid == -1 || tindex < tmp.firstgid || tindex >= tmp.firstgid + tmp.tilecount)
							continue;

						GameObject *gobj = new GameObject();
						int tileindex = tindex - tmp.firstgid;
						SpriteRenderer* SR = gobj->AddComponentOnce<SpriteRenderer>();
						SR->LoadBitmapData(tmp.objects[tileindex].image);
						
						Instantiate(gobj);
						gobj->transform->SetParent(this->transform);
						gobj->transform->SetPosition(obj);
						gobj->transform->SetZIndex(zindex);
						gobj->SetLayer(Layer::Tile);

						break;

					}
				}
			}

			zindex++;
		}
	}
	else
	{
		string str = "ERROR : Map NOT FOUND when parse JSON :\n";
		str += "File : " + path;
		GAME_ASSERT(false, str.c_str());
	}

	file.close();
}

void MapReader::Draw(Vector2I campos)
{
	
}

void MapReader::Update()
{
	if (Input::GetKeyClick(VK_F8))
	{
		LoadMap("temp3");
	}

    if (Input::GetKeyClick(VK_F7))
    {
        vector<Transform*> objlist = transform->GetChild();
        for (Transform* gobj : objlist)
            Destroy(gobj->gameObject);
    }

    if (Input::GetKeyPressing(VK_F6))
        this->transform->Translate(Vector2::right * 5);
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
	for (json tileset : j["tilesets"])
	{
		if (tileset["columns"].get<int>() != 0)
			tm.tileSetList.push_back(tileset);
		else
			tm.objectSetList.push_back(tileset);

		
	}
}

void from_json(const json & j, ObjectSet & os)
{
	os.firstgid = j["firstgid"];
	os.tilecount = j["tilecount"];

	for (int i = 0; i < os.tilecount; i++)
	{
		string imgname = j["tiles"][to_string(i)]["image"].get<string>();
		imgname = imgname.substr(imgname.find_first_of("Bitmap") + 7);
		imgname = imgname.substr(0, imgname.find_last_of("."));

		json prop = (j.find("tileproperties") != j.end() ? j["tileproperties"][to_string(i)] : json());

		os.objects.push_back(TileObject(imgname, prop));
	}
}


void from_json(const json& j, TileSet& ts)
{
	ts.columns = j["columns"];
	string imgname = j["image"].get<string>();
	imgname = imgname.substr(imgname.find_first_of("Bitmap") + 7);
	imgname = imgname.substr(0, imgname.find_last_of("."));
	ts.image = imgname;
	ts.tileSize = Vector2I(j["tilewidth"], j["tileheight"]);
	ts.firstgid = j["firstgid"];
	ts.tilecount = j["tilecount"];

	if (j.find("tiles") != j.end())
	{
		for (int i = 0; i < ts.tilecount; i++)
			if (j["tiles"].find(to_string(i)) != j["tiles"].end())
				ts.tiles.push_back(j["tiles"][to_string(i)]["objectgroup"]);
	}
	else
	{
		GAME_ASSERT(false, ("collider not found in tile map,image : " + imgname).c_str());
	}
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

TileObject::TileObject(string imgname, json inproperties)
{
	image = imgname;
	properties = inproperties;
}
