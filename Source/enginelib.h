/*  enginelib.h: 本檔案儲存遊戲引擎所使用的各式常見Class的標頭檔
Copyright (C) 2017 Guo Xiang, Hong <sunnyhong.w@gmail.com>

HISTORY :
2017-02-09 v0.1.0
*/

#pragma once

namespace game_engine
{

struct Vector2I;

struct Vector2
{
    public:
        Vector2();
        Vector2(bool setNull);
        Vector2(float x, float y);
        float x;
        float y;

        const static Vector2 up;
        const static Vector2 down;
        const static Vector2 left;
        const static Vector2 right;
        const static Vector2 one;
        const static Vector2 zero;
        const static Vector2 null;
        Vector2I GetV2I();
        bool isNull();
    private:
        bool nullVector = false;
};

struct Vector2I
{
    public:
        Vector2I();
        Vector2I(int x, int y);
        Vector2I(Vector2* v2);
        int x;
        int y;
};

}