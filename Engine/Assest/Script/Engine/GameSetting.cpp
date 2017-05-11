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
int GameSetting::sizeX;
int GameSetting::sizeY;

void GameSetting::InitSetting()
{
    fstream load;
    load.open(".\\Assest\\setting.dat", ios::in);
    if (load)
        setting << load;
    load.close();

    sizeX = setting["window"][WindowName]["x"];
    sizeY = setting["window"][WindowName]["y"];
}

int GameSetting::GetSizeX()
{
    return sizeX;
}

int GameSetting::GetSizeY()
{
    return sizeY;
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