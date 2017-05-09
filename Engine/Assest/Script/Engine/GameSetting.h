#pragma once
#include"json_include.h"

class GameSetting {
public:
    static json setting;
    static string WindowName;
    static void InitSetting();
    static int GetSizeX();
    static int GetSizeY();
    static int GetMusicVolume();
    static int GetSoundVolume();
    static void SaveSetting();
};
