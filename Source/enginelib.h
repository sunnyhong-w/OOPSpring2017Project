/*  enginelib.h: 本檔案儲存遊戲引擎所使用的各式常見Class的標頭檔
Copyright (C) 2017 Guo Xiang, Hong <sunnyhong.w@gmail.com>

HISTORY :
2017-02-09 v0.1.0
*/

#pragma once
#include <string>
using namespace std;
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
        Vector2 operator -(Vector2 subtraction);
        Vector2 operator +(Vector2 adder);
        friend Vector2 operator *(Vector2 multiplied, Vector2 multiplier);
        friend Vector2 operator *(Vector2 multiplied, double multiplier);
        friend Vector2 operator *(double multiplied, Vector2 multiplier);
        friend Vector2 operator /(Vector2 multiplied, Vector2 multiplier);
        friend Vector2 operator /(Vector2 multiplied, double multiplier);
        friend Vector2 operator /(double multiplied, Vector2 multiplier);
        friend bool operator <(Vector2 left, Vector2 right);
        friend bool operator >(Vector2 left, Vector2 right);
        friend bool operator <=(Vector2 left, Vector2 right);
        friend bool operator >=(Vector2 left, Vector2 right);
        friend bool operator ==(Vector2 left, Vector2 right);
        friend bool operator !=(Vector2 left, Vector2 right);
        //Vector2I operator =(Vector2 equ);
        Vector2I GetV2I();
        string toString();
        bool isNull();
    private:
        bool nullVector = false;
};

struct Vector2I
{
    public:
        Vector2I();
        Vector2I(bool setNull);
        Vector2I(int x, int y);
        Vector2I(Vector2* v2);
        Vector2I operator -(Vector2I subtraction);
        Vector2I operator +(Vector2I adder);
        friend Vector2I operator *(Vector2I multiplied, Vector2I multiplier);
        friend Vector2I operator *(Vector2I multiplied, int multiplier);
        friend Vector2I operator *(int multiplied, Vector2I multiplier);
        friend Vector2I operator /(Vector2I multiplied, Vector2I multiplier);
        friend Vector2I operator /(Vector2I multiplied, int multiplier);
        friend Vector2I operator /(int multiplied, Vector2I multiplier);
        friend bool operator <(Vector2I left, Vector2I right);
        friend bool operator >(Vector2I left, Vector2I right);
        friend bool operator <=(Vector2I left, Vector2I right);
        friend bool operator >=(Vector2I left, Vector2I right);
        friend bool operator ==(Vector2I left, Vector2I right);
        friend bool operator !=(Vector2I left, Vector2I right);
        Vector2I abs();
        int x;
        int y;
        bool isNull();
    private:
        bool nullVector = false;
};
}