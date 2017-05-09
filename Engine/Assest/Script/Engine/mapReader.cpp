#include "StdAfx.h"
#include "Engine.h"
#include "mapReader.h"
#include<fstream>
using namespace std;

using namespace game_engine;

vector<MapReader*> MapReader::readerList;

void MapReader::LateUpdate()
{
    Vector2 pos = (GameScene::CameraPosition().GetV2() - this->transform->GetWorldPosition()) / (slicer * Vector2(tileMap.tileWidth, tileMap.tileHeight));
    Vector2 fpos = pos.floorSpecial();
    Vector2 cpos = pos.ceilSpecial();

    vector<Vector2I> posVector;

    posVector.push_back(fpos.GetV2I());
    if (pos.x != fpos.x)
        posVector.push_back(Vector2I(cpos.x, fpos.y));

    if (pos.y != fpos.y)
        posVector.push_back(Vector2I(fpos.x, cpos.y));

    if(pos.x != fpos.x && pos.y != fpos.y)
        posVector.push_back(cpos.GetV2I());

    string name = this->gameObject->GetName();

    for (auto ctrans : this->transform->GetChild())
    {
        string objname = ctrans->gameObject->GetName();

        if (objname == name + " Objects")
            continue;

        bool find = false;

        for (auto v : posVector)
        {
            if (objname == name + " Slice(" + to_string(v.x) + ", " + to_string(v.y) + ")")
            {
                ctrans->gameObject->SetEnable(true);
                find = true;
                continue;
            }
        }

        if(!find)
            ctrans->gameObject->SetEnable(false);
    }
}

void MapReader::ParseJSON(json j)
{

    if (j.find("slicer") != j.end())
    {
        slicer = j["slicer"];
    }

	if (j.find("load") != j.end())
	{
		LoadMap(j["load"].get<string>());
	}

}

void MapReader::ParseProperties(GameObject * gobj, string filename, json prop)
{
    for (json::iterator j = prop.begin(); j != prop.end(); j++)
    {
        if (j.key() == "ParseJSON")
        {
            gobj->ParseJSON(json::parse(j.value().get<string>().c_str()));
        }
        else if (j.key() == "include")
        {
            string prefrabName = j.value();
            json prefrab = GameObject::GetPrefrabs(prefrabName);
            if (prefrab == nullptr)
            {
                GameScene::IncludePrefrabs(filename + ".map", { {prefrabName , prefrabName} });
                prefrab = GameObject::GetPrefrabs(prefrabName);
            }

            bool doNOTDestoryOnChangeScene = prefrab.find("doNOTDestoryOnChangeScene") != prefrab.end() ? prefrab["doNOTDestoryOnChangeScene"] : false;
            gobj->ParseJSON(prefrab);
            gobj->doNOTDestoryOnChangeScene = doNOTDestoryOnChangeScene;

            if (prefrab.find("Child") != prefrab.end())
            {
                auto childList = GameScene::InstantiateGameObject(filename + ".map", prefrab["Child"]);

                for (auto childobj : childList)
                    childobj->transform->SetParent(gobj->transform);
            }
        }
        else if (j.key() == "GreenBox")
        {
            gobj->renderByBehavior = true;
            GreenBox* gb = gobj->AddComponentOnce<GreenBox>();
            string tmp = "";
            string jstr = j.value();
            for (char c : jstr)
            {
                if (c != '\n')
                    tmp += c;
            }

            gb->SetData(tmp);
        }
        else if (j.key() == "Transporter")
        {
            gobj->SetName(filename + j.value().get<string>() + "Transporter");
            gobj->AddComponentOnce<SpriteRenderer>()->SetEnable(false);
		}
		else if (j.key() == "Gem")
		{
			auto collectedGem = gobj->AddComponentOnce<CollectedGem>();
			collectedGem->name = j.value().get<string>();
		}
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

        this->gameObject->SetName(fname);

        Vector2I tileSlice = (Vector2(tileMap.width, tileMap.height) / slicer).ceil().GetV2I();
        for (int sliceY = 0; sliceY < tileSlice.y; sliceY++)
        {
            for (int sliceX = 0; sliceX < tileSlice.x; sliceX++)
            {
                GameObject* sliceParent = new GameObject();
                Instantiate(sliceParent);
                sliceParent->SetName(fname + " Slice(" + to_string(sliceX) + ", " + to_string(sliceY) + ")");
                sliceParent->transform->SetParent(this->transform);
                sliceParent->transform->SetPosition(Vector2(sliceX, sliceY) * slicer * Vector2(tileMap.tileWidth, tileMap.tileHeight) + Vector2(0, tileMap.tileHeight));
            }
        }

        GameObject* objParent = new GameObject();
        Instantiate(objParent);
        objParent->SetName(fname + " Objects");
        objParent->transform->SetParent(this->transform);
        objParent->transform->SetPosition(Vector2(0,0));

		CreateMapEdge(objParent->transform, Vector2(tileMap.width, tileMap.height), fname, Vector2I(tileMap.tileWidth, tileMap.tileHeight));

        int zindex = 0;
        for (json j : tileMap.layers)
        {
            if (j["type"].get<string>() == "tilelayer")
            {
                Vector2 pos = Vector2(0, tileMap.tileHeight);

                for (int sliceY = 0; sliceY < tileSlice.y; sliceY++)
                {
                    for (int sliceX = 0; sliceX < tileSlice.x; sliceX++)
                    {
                        GameObject* sliceParent = GameObject::findGameObjectByName(fname + " Slice(" + to_string(sliceX) + ", " + to_string(sliceY) + ")");

                        for (int y = 0; y < slicer.y; y++)
                        {
                            int ty = sliceY * slicer.y + y;
                            if (ty >= tileMap.height)
                                break;

                            for (int x = 0; x < slicer.x; x++)
                            {
                                int tx = sliceX * slicer.x + x;
                                if (tx >= tileMap.width)
                                    break;

                                GameObject* gobj = GenerateTile(fname, j["data"][ty * tileMap.width + tx]);

                                if (gobj != nullptr)
                                {
                                    gobj->transform->SetParent(sliceParent->transform);
                                    gobj->transform->SetPosition(Vector2(x, y) * Vector2(tileMap.tileWidth, tileMap.tileHeight));
                                    gobj->transform->SetZIndex(zindex);
                                }
                            }
                        }
                    }
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

                        ParseProperties(gobj, fname, tmp.objects[tileindex].properties);
                        ParseProperties(gobj, fname, obj["properties"]);

						if (gobj->animationController == nullptr)
						{
							SpriteRenderer* SR = gobj->AddComponentOnce<SpriteRenderer>();
							SR->LoadBitmapData(tmp.objects[tileindex].image);
						}

						gobj->spriteRenderer->SetAnchorRaito(Vector2::down);

                        if (tmp.objects[tileindex].collision.size() != 0)
                        {
                            Collider* cr = gobj->AddComponentOnce<Collider>();
                            cr->collisionInfo = tmp.objects[tileindex].collision[0]; //先當他只會有一個collider
                        }

                        Instantiate(gobj, objParent->transform, obj);
                        gobj->transform->SetZIndex(zindex);

                        if (obj["name"] != "")
                            gobj->SetName(obj["name"]);

                        break;
                    }
                }
            }

            zindex++;
        }
        this->transform->SetWorldZIndex(-1 * zindex);
    }
    else
    {
        string str = "ERROR : Map NOT FOUND when parse JSON :\n";
        str += "File : " + path;
        GAME_ASSERT(false, str.c_str());
    }

    file.close();
}

void MapReader::Start()
{
    readerList.push_back(this);
}

void MapReader::Draw(Vector2I campos)
{
	
}

GameObject* MapReader::GenerateTile(string fname, int tindex)
{
    if (tindex != 0)
    {
        for (TileSet tmp : tileMap.tileSetList)
        {
            if (tmp.firstgid == -1 || tindex < tmp.firstgid || tindex >= tmp.firstgid + tmp.tilecount)
                continue;

            int tileindex = tindex - tmp.firstgid;

            GameObject *gobj = new GameObject();

            ParseProperties(gobj, fname, tmp.tiles[tileindex].properties);

            gobj->ParseJSON(GameObject::GetPrefrabs("Engine\\TileRenderer"));
            SpriteRenderer* SR = gobj->AddComponentOnce<SpriteRenderer>();
            SR->LoadBitmapData(tmp.image);

            Vector2I srcpos(tileindex % tmp.columns, tileindex / tmp.columns);
            SR->SetSourcePos(srcpos * tmp.tileSize);
            SR->SetSize(tmp.tileSize);
            SR->SetAnchorRaito(Vector2::down);

            if (tmp.tiles[tileindex].object.size() != 0)
            {
                Collider* cr = gobj->AddComponentOnce<Collider>();
                cr->collisionInfo = tmp.tiles[tileindex].object[0]; //先當他只會有一個collider
            }

            Instantiate(gobj);
            gobj->SetLayer(Layer::Tile);

            return gobj;
        }
    }

    return nullptr;
}

void MapReader::CreateMapEdge(Transform * parent, Vector2 mapSize, string filename, Vector2I tileSize)
{
	string type[2] = { "Map" , "Camera" };
	
	Vector2 opos = (tileSize * -1).GetV2();
	Vector2I HCollider((mapSize.x + 2) * tileSize.x, tileSize.y);
	Vector2I VCollider(tileSize.x, (mapSize.y + 2) * tileSize.y);

	string way[4] = { "Top", "Bottom", "Left", "Right"};
	Vector2I colliderSize[4] = { HCollider,HCollider,VCollider,VCollider };
	Vector2 position[4] = { opos,Vector2(tileSize.x * -1 , 0) + Vector2::down * mapSize * tileSize.GetV2(), 
							opos,Vector2(0 , tileSize.y * -1) + Vector2::right * mapSize * tileSize.GetV2() };

	for (int i = 0; i < 4; i++)
	{
		for (auto str : type)
		{
			CreateEdgeObj(parent, filename, way[i], str, position[i], colliderSize[i]);
		}
	}
}

void MapReader::CreateEdgeObj(Transform * parent, string filename, string way, string type, Vector2 position, Vector2I colliderSize)
{
	GameObject* obj = new GameObject();
	Instantiate(obj);
	obj->SetName(filename + way + type + "Edge");
	obj->transform->SetParent(parent);
	obj->transform->SetPosition(position);

	auto collider = obj->AddComponentOnce<Collider>();
	CollisionInfo ci;
	ci.size = colliderSize;
	collider->collisionInfo = ci;
	
	if (type == "Camera")
		obj->SetLayer(Layer::CameraEdge);
    else if (type == "Map")
    {
        obj->AddComponentOnce<MapEdge>();
        obj->SetLayer(Layer::Tile);
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

        json prop;
        if (j.find("tileproperties") != j.end())
        {
            if (j["tileproperties"].find(to_string(i)) != j["tileproperties"].end())
            {
                prop = j["tileproperties"][to_string(i)];
            }
        }

        vector<CollisionInfo> co;
        if (j["tiles"][to_string(i)].find("objectgroup") != j["tiles"][to_string(i)].end())
        {
            if (j["tiles"][to_string(i)]["objectgroup"].find("objects") != j["tiles"][to_string(i)]["objectgroup"].end())
            {
                for (json jobj : j["tiles"][to_string(i)]["objectgroup"]["objects"])
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

                    co.push_back(ci);
                }
            }
        }

		os.objects.push_back(TileObject(imgname, prop, co));
	}
}

void from_json(const json& j, TileSet& ts)
{
	ts.columns = j["columns"];
	string imgname = j["image"].get<string>();
	imgname = imgname.substr(imgname.find_first_of("Bitmap") + 7);
	imgname = imgname.substr(0, imgname.find_last_of("."));
	ts.image = imgname;
    ts.tileSetName = j["name"].get<string>();
	ts.tileSize = Vector2I(j["tilewidth"], j["tileheight"]);
	ts.firstgid = j["firstgid"];
	ts.tilecount = j["tilecount"];

	if (j.find("tiles") != j.end())
	{
        for (int i = 0; i < ts.tilecount; i++)
        {
            if (j["tiles"].find(to_string(i)) != j["tiles"].end())
            {
                Tile t = j["tiles"][to_string(i)]["objectgroup"];

                json prop;
                if (j.find("tileproperties") != j.end())
                {
                    if (j["tileproperties"].find(to_string(i)) != j["tileproperties"].end())
                    {
                        prop = j["tileproperties"][to_string(i)];
                    }
                }

                t.properties = prop;

                ts.tiles.push_back(t);
            }
            else
            {
                ts.tiles.push_back(Tile());
            }
        }
	}
	else
	{
        for (int i = 0; i < ts.tilecount; i++)
        {
            ts.tiles.push_back(Tile());
        }
	}
}

void from_json(const json& j, Tile& to)
{
	if(j.find("properties") != j.end())
		to.colliderProperties = j["properties"];

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

TileObject::TileObject(string imgname, json inproperties, vector<CollisionInfo> incollision)
{
	this->image = imgname;
    this->properties = inproperties;
    this->collision = incollision;
}

Vector2I TileMap::GetMapSize()
{
	return Vector2I(width*tileWidth,height*tileHeight);
}
