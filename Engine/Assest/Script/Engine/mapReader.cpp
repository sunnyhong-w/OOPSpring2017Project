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
			json mapData = json::parse(buffer);

			string filename = mapData[""];

			for (json tileset : j["tilesets"])
			{

			}
		}
		else
		{
			string str = "ERROR : Map NOT FOUND when parse JSON :\n";
			str += "File : " + path;
			GAME_ASSERT(false, str.c_str());
		}
	}
}
