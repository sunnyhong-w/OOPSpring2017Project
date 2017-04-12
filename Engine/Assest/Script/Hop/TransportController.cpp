#include "stdafx.h"
#include "Engine.h"
#include "TransportController.h" //Include you header

void TransportController::OnRecivedBoardcast(BoardcastMessageData bmd)
{
    set<string> mapNameSet;

    SlideBoxSetting boxMapping[4][4];
    string boxName[4][4];

    for (json::iterator it = bmd.data.begin(); it != bmd.data.end(); ++it)
    {
        string name = it.key();
        mapNameSet.insert(name);

        Vector2I pos = it.value()["position"];
        json boxdata = it.value()["data"];

        for (SlideBoxSetting sbs : boxdata)
        {
            Vector2I tpos = pos + sbs.position;
            boxMapping[tpos.x][tpos.y] = sbs;
            boxName[tpos.x][tpos.y] = name;
        }
    }

    string way[4] = { "Top", "Bottom", "Left", "Right" };
    for (auto str : mapNameSet)
    {
        if (edgeMapGameObject.find(str) == edgeMapGameObject.end())
        {
            for(auto sway : way)
            {
                GameObject* gobj = GameObject::findGameObjectByName(str + sway + "Map" + "Edge");
                if (gobj != nullptr)
                {
                    edgeMapGameObject[str][sway] = gobj;
                }
            }
        }
    }

    for (auto mappair : edgeMapGameObject)
    {
        for (auto waypair : mappair.second)
        {
            GameObject* gobj = waypair.second;
            gobj->SetLayer(Layer::Tile);
        }
    }

    for (int y = 0; y < 4; y++)
    {
        for (int x = 0; x < 4; x++)
        {
            if (y + 1 < 4 && boxMapping[x][y].state.bottom && boxMapping[x][y + 1].state.top)
            {
                edgeMapGameObject[boxName[x][y]]["Bottom"]->SetLayer(Layer::Default);
                edgeMapGameObject[boxName[x][y + 1]]["Top"]->SetLayer(Layer::Default);
            }

            if (x + 1 < 4 && boxMapping[x][y].state.right && boxMapping[x + 1][y].state.left)
            {
                edgeMapGameObject[boxName[x][y]]["Right"]->SetLayer(Layer::Default);
                edgeMapGameObject[boxName[x + 1][y]]["Left"]->SetLayer(Layer::Default);
            }
        }
    }
}