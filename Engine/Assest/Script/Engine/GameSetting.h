#pragma once
#include"json_include.h"

class GameSetting {
public:
    static json setting;
    static string WindowName;
    static void InitSetting();
    static int GetSizeX();
    static int GetSizeY();
    static float GetMusicVolume();
    static float GetSoundVolume();
    static void SaveSetting();
};
