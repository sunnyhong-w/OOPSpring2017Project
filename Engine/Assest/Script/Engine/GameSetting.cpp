#include "StdAfx.h"
#include <fstream>
#include "Engine.h"
#include "GameSetting.h"

/////////////////////////////////////////////////////////////
//Compile Window Selection                                 //
/////////////////////////////////////////////////////////////
string GameSetting::WindowName = "Hop";
/////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////
//Setting
json GameSetting::setting;

void GameSetting::InitSetting()
{
    fstream load;
    load.open(".\\Assest\\setting.dat", ios::in);
    if (load)
        setting << load;
    load.close();
}

int GameSetting::GetSizeX()
{
    return setting["window"][WindowName]["x"];
}

int GameSetting::GetSizeY()
{
    return setting["window"][WindowName]["y"];
}

float GameSetting::GetMusicVolume()
{
    return setting["audio"]["Music"];
}

float GameSetting::GetSoundVolume()
{
    return setting["audio"]["Sound"];
}

void GameSetting::SaveSetting()
{
    fstream save;
    save.open(R"(.\Assest\setting.dat)", ios::out);
    save << setting.dump(4);
    save.close();
}