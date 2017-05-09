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

    button1 = 0;
    button2 = 0;
    isSended = false;

    AudioPlayer::GetSource("RSIC")->Play();
}

void BoxParent::Update()
{
    if (!isSended && button1 > 0 && button2 > 0)
    {
        json data;
        GameScene::Broadcast(BroadcastEvent::RedRoomFinish, data, "Hop");
        isSended = true;
    }

    if (Input::GetKeyPressing(VK_F5))
        AudioPlayer::SetMusicVolume(AudioPlayer::GetMusicVolume() - 0.01f);

    if (Input::GetKeyPressing(VK_F6))
        AudioPlayer::SetMusicVolume(AudioPlayer::GetMusicVolume() + 0.01f);

    if (Input::GetKeyPressing(VK_F7))
        AudioPlayer::SetSoundVolume(AudioPlayer::GetSoundVolume() - 0.01f);

    if (Input::GetKeyPressing(VK_F8))
        AudioPlayer::SetSoundVolume(AudioPlayer::GetSoundVolume() + 0.01f);
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
		GameScene::Broadcast(BroadcastEvent::UpdateBoxPosition, sendData);

        for (int x = 0; x < 4; x++)
        {
            for (int y = 0; y < 4; y++)
            {
                boxdata[x][y] = tempData[x][y];
            }
        }
    }
}

void BoxParent::OnRecivedBroadcast(BroadcastMessageData bmd)
{
    if (bmd.event == BroadcastEvent::ButtonPressed)
    {
        if (bmd.data["name"] == "Button1")
            button1++;
        else if (bmd.data["name"] == "Button2")
            button2++;
    }
    else if (bmd.event == BroadcastEvent::ButtonRelease)
    {
        if (bmd.data["name"] == "Button1")
            button1--;
        else if (bmd.data["name"] == "Button2")
            button2--;
    }
}
