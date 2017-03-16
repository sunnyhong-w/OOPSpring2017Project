/*  enginelib.h: 本檔案儲存遊戲引擎所使用的各式常見Class的標頭檔
Copyright (C) 2017 Guo Xiang, Hong <sunnyhong.w@gmail.com>

HISTORY :
2017-02-09 v0.1.0
*/

#pragma once
#include"_setting.h"
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
        Vector2I GetV2I();
        string toString();
        bool isNull();
        Vector2 abs();
		Vector2 round();

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
        Vector2 operator -(Vector2I subtraction);
        Vector2 operator +(Vector2I adder);
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

		static Vector2 SmoothDamp(Vector2 current,Vector2 target, Vector2 &currentVelocity,
								  float smoothTime, float maxSpeed = 0, float deltaTime = 0);

		static Vector2 SmoothDampEX(Vector2 current, Vector2 target, Vector2 &currentVelocity,
						            float pd, float f, float smoothTime, float maxSpeed = 0, float deltaTime = 0);

    private:
        bool nullVector = false;
};

void from_json(const json& j, Vector2& v);
void to_json(json& j, const Vector2& v);

struct Vector2I
{
    public:
        const static Vector2I up;
        const static Vector2I down;
        const static Vector2I left;
        const static Vector2I right;
        const static Vector2I one;
        const static Vector2I zero;
        const static Vector2I null;
        Vector2I();
        Vector2I(bool setNull);
        Vector2I(int x, int y);
        Vector2I(Vector2* v2);
        bool isNull();
        Vector2I abs();
        Vector2 GetV2();
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
        
        int x;
        int y;

    private:
        bool nullVector = false;
};

void from_json(const json& j, Vector2I& v);
void to_json(json& j, const Vector2I& v);

struct AnimationSetting
{
public:
    AnimationSetting();
    bool Build(AnimationSetting newSetting);
    string filename;
    Vector2I position;
    Vector2I size;
    Vector2 anchor;
    int duration;
};

void from_json(const json& j, AnimationSetting& v);
void to_json(json& j, const AnimationSetting& v);

class Time
{
public:
	static float deltaTime;
	static void Update();
private:
	static DWORD timeStamp;
};

}