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

    button1 = false;
    button2 = false;
    isSended = false;
}

void BoxParent::Update()
{
    if (!isSended && button1 && button2)
    {
        json data;
        GameScene::Boardcast(BoardcastEvent::RedRoomFinish, data, "Hop");
        isSended = true;
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
		GameScene::Boardcast(BoardcastEvent::UpdateBoxPosition, sendData);

        for (int x = 0; x < 4; x++)
        {
            for (int y = 0; y < 4; y++)
            {
                boxdata[x][y] = tempData[x][y];
            }
        }
    }
}

void BoxParent::OnRecivedBoardcast(BoardcastMessageData bmd)
{
    if (bmd.event == BoardcastEvent::ButtonPressed)
    {
        if (bmd.data["name"] == "Button1")
            button1 = true;
        else if (bmd.data["name"] == "Button2")
            button2 = true;
    }
    else if (bmd.event == BoardcastEvent::ButtonRelease)
    {
        if (bmd.data["name"] == "Button1")
            button1 = false;
        else if (bmd.data["name"] == "Button2")
            button2 = false;
    }
}
