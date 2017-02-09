/*  enginelib.h: 本檔案儲存遊戲引擎所使用的各式常見Class的標頭檔
Copyright (C) 2017 Guo Xiang, Hong <sunnyhong.w@gmail.com>

HISTORY :
2017-02-09 v0.1.0
*/

#pragma once

namespace game_engine {

///////////////////////////////////
//Render Z-index position define //
///////////////////////////////////
enum RenderDepth { BACKGROUND = -1000 , MAINGROUND = 0, FOREGROUND = 1000, GUI = 2000 };
///////////////////////////////////

struct Vector2
{
public:
    Vector2();
    Vector2(float x, float y);
    float x;
    float y;
    const static Vector2 up;
    const static Vector2 down;
    const static Vector2 left;
    const static Vector2 right;
    const static Vector2 one;
    const static Vector2 zero;
};

}