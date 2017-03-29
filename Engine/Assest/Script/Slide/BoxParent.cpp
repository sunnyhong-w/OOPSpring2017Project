#include "stdafx.h"
#include "Engine.h"
#include "BoxParent.h" //Include you header

void BoxParent::Start()
{
    for (int x = 0; x < 4; x++)
    {
        for (int y = 0; y < 4; y++)
        {
            boxdata[x][y] = "";
        }
    }
}

void BoxParent::CheckData()
{
    Vector2 slice(64, 64);
    json sendData;
    string tempData[4][4];

    for (auto tChild : this->transform->GetChild())
    {
        Slide* slide = tChild->gameObject->GetComponent<Slide>();
        string name = tChild->gameObject->GetName();
        Vector2 tpos = (tChild->GetPostion() / slice).round();

        for (auto sbs : slide->boxdata)
        {
            Vector2I tmppos = (tpos + sbs.position).GetV2I();

            tempData[tmppos.x][tmppos.y] = name;
        }

        sendData[name]["data"] = slide->boxdata;
        sendData[name]["position"] = tpos.GetV2I();
    }

    //Compare

    bool Same = true;

    for (int x = 0; x < 4 && Same; x++)
    {
        for (int y = 0; y < 4 && Same; y++)
        {
            Same = tempData[x][y] == boxdata[x][y];
        }
    }

    if (!Same)
    {
        json send;
        send["data"] = sendData;
        send["event"] = "UpdateBoxPostion";

        game_framework::CGame::Instance()->BoardcastMessage(send);

        for (int x = 0; x < 4; x++)
        {
            for (int y = 0; y < 4; y++)
            {
                boxdata[x][y] = tempData[x][y];
            }
        }
    }
}

void BoxParent::OnRecivedBoardcast(json j)
{
    //AfxMessageBox(j.dump(4).c_str());
}
