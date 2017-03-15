#include "stdafx.h"
#include"enginelib.h"
#include"easing.h"

namespace game_engine
{

/////////////////////////////////////////////////////////////
// Vector2
/////////////////////////////////////////////////////////////

const Vector2 Vector2::up     = Vector2( 0,  1);
const Vector2 Vector2::down   = Vector2( 0, -1);
const Vector2 Vector2::left   = Vector2(-1,  0);
const Vector2 Vector2::right  = Vector2( 1,  0);
const Vector2 Vector2::one    = Vector2( 1,  1);
const Vector2 Vector2::zero   = Vector2( 0,  0);
const Vector2 Vector2::null   = Vector2(true);

const Vector2I Vector2I::up   = Vector2I(0, 1);
const Vector2I Vector2I::down = Vector2I(0, -1);
const Vector2I Vector2I::left = Vector2I(-1, 0);
const Vector2I Vector2I::right = Vector2I( 1, 0);
const Vector2I Vector2I::one  = Vector2I(1, 1);
const Vector2I Vector2I::zero = Vector2I(0, 0);
const Vector2I Vector2I::null = Vector2I(true);

Vector2::Vector2()
{
    this->x = 0;
    this->y = 0;
}

Vector2::Vector2(bool setNull) : nullVector(setNull)
{}

Vector2::Vector2(float x, float y)
{
    this->x = x;
    this->y = y;
}

Vector2 Vector2::operator-(Vector2 subtraction)
{
    return Vector2(this->x - subtraction.x, this->y - subtraction.y);
}

Vector2 Vector2::operator+(Vector2 adder)
{
    return Vector2(this->x + adder.x, this->y + adder.y);
}

Vector2 Vector2::operator-(Vector2I subtraction)
{
    return Vector2(this->x - subtraction.x, this->y - subtraction.y);
}

Vector2 Vector2::operator+(Vector2I adder)
{
    return Vector2(this->x - adder.x, this->y - adder.y);
}

Vector2 Vector2::SmoothDamp(Vector2 current, Vector2 target, Vector2 &currentVelocity, float smoothTime, float maxSpeed, float deltaTime)
{
	return Vector2::SmoothDampPt(current, target, currentVelocity, 0.003f, 4, smoothTime, maxSpeed, deltaTime);
}

Vector2 Vector2::SmoothDampPt(Vector2 current, Vector2 target, Vector2 & currentVelocity, float pd, float f, float smoothTime, float maxSpeed, float deltaTime)
{
	float dt = deltaTime != 0 ? deltaTime : Time::deltaTime;

	Damp(current.x, currentVelocity.x, target.x, smoothTime, pd, f, dt);
	Damp(current.y, currentVelocity.y, target.y, smoothTime, pd, f, dt);
	if (currentVelocity.x > maxSpeed && maxSpeed != 0)
		currentVelocity.x = maxSpeed;
	if (currentVelocity.y > maxSpeed && maxSpeed != 0)
		currentVelocity.y = maxSpeed;
	return current;
}

Vector2 operator*(Vector2 multiplied, Vector2 multiplier)
{
    return Vector2(multiplied.x * multiplier.x, multiplied.y * multiplier.y);
}

Vector2 operator*(Vector2 multiplied, double multiplier)
{
    return Vector2(multiplied.x * multiplier, multiplied.y * multiplier);
}

Vector2 operator*(double multiplied, Vector2 multiplier)
{
    return Vector2(multiplied * multiplier.x, multiplied * multiplier.y);
}

Vector2 operator/(Vector2 multiplied, Vector2 multiplier)
{
    return Vector2(multiplied.x / multiplier.x, multiplied.y / multiplier.y);
}
Vector2 operator/(Vector2 multiplied, double multiplier)
{
    return Vector2(multiplied.x / multiplier, multiplied.y / multiplier);
}

Vector2 operator/(double multiplied, Vector2 multiplier)
{
    return Vector2(multiplied / multiplier.x, multiplied / multiplier.y);
}

bool operator<(Vector2 left, Vector2 right)
{
    bool hasNull = left.isNull() || right.isNull();
    return (!hasNull) && left.x < right.x && left.y < right.y;
}

bool operator>(Vector2 left, Vector2 right)
{
	bool hasNull = left.isNull() || right.isNull();
	return (!hasNull) && left.x > right.x && left.y > right.y;
}

bool operator<=(Vector2 left, Vector2 right)
{
	bool hasNull = left.isNull() || right.isNull();
	return (!hasNull) && left.x <= right.x && left.y <= right.y;
}

bool operator>=(Vector2 left, Vector2 right)
{
	bool hasNull = left.isNull() || right.isNull();
	return (!hasNull) && left.x >= right.x && left.y >= right.y;
}

bool operator==(Vector2 left, Vector2 right)
{
    bool hasNull = left.isNull() || right.isNull();
	return hasNull ? left.isNull() && right.isNull() : left.x == right.x && left.y == right.y;
}

bool operator!=(Vector2 left, Vector2 right)
{
	bool hasNull = left.isNull() || right.isNull();
	return hasNull ? left.isNull() != right.isNull() : left.x != right.x && left.y != right.y;
}

Vector2 Vector2::abs()
{
    return Vector2(std::abs(this->x), std::abs(this->y));
}

Vector2I Vector2::GetV2I()
{
    return Vector2I(this);
}

string Vector2::toString()
{
    return '(' + to_string(this->x) + ',' + to_string(this->y) + ')';
}

bool Vector2::isNull()
{
    return nullVector;
}

void from_json(const json& j, Vector2& v)
{
    if (j.find("x") != j.end())
        v.x = j["x"];

    if (j.find("y") != j.end())
        v.y = j["y"];
}

void to_json(json& j, const Vector2& v) {
    j = json{ { "x", v.x },{ "y", v.y }};
}

///////

Vector2I::Vector2I()
{
    this->x = 0;
    this->y = 0;
}

Vector2I::Vector2I(bool setNull) : nullVector(setNull)
{}

Vector2I::Vector2I(int x, int y)
{
    this->x = x;
    this->y = y;
}

Vector2I::Vector2I(Vector2* v2)
{
    this->x = (int)v2->x;
    this->y = (int)v2->y;
}

Vector2I Vector2I::operator-(Vector2I subtraction)
{
    return Vector2I(this->x - subtraction.x, this->y - subtraction.y);
}

Vector2I Vector2I::operator+(Vector2I adder)
{
    return Vector2I(this->x + adder.x, this->y + adder.y);
}

Vector2 Vector2I::GetV2()
{
    return Vector2(this->x, this->y);
}

Vector2I Vector2I::abs()
{
    return Vector2I(std::abs(this->x), std::abs(this->y));
}

bool Vector2I::isNull()
{
    return nullVector;
}

Vector2I operator*(Vector2I multiplied, Vector2I multiplier)
{
    return Vector2I(multiplied.x * multiplier.x, multiplied.y * multiplier.y);
}

Vector2I operator*(Vector2I multiplied, int multiplier)
{
    return Vector2I(multiplied.x * multiplier, multiplied.y * multiplier);
}

Vector2I operator*(int multiplied, Vector2I multiplier)
{
    return Vector2I(multiplied * multiplier.x, multiplied * multiplier.y);
}

Vector2I operator/(Vector2I multiplied, Vector2I multiplier)
{
    return Vector2I(multiplied.x / multiplier.x, multiplied.y / multiplier.y);
}

Vector2I operator/(Vector2I multiplied, int multiplier)
{
    return Vector2I(multiplied.x / multiplier, multiplied.y / multiplier);
}

Vector2I operator/(int multiplied, Vector2I multiplier)
{
    return Vector2I(multiplied / multiplier.x, multiplied / multiplier.y);
}

bool operator<(Vector2I left, Vector2I right)
{
    bool hasNull = left.isNull() || right.isNull();
    return (!hasNull) && left.x < right.x && left.y < right.y;
}

bool operator>(Vector2I left, Vector2I right)
{
	bool hasNull = left.isNull() || right.isNull();
	return (!hasNull) && left.x > right.x && left.y > right.y;
}

bool operator<=(Vector2I left, Vector2I right)
{
	bool hasNull = left.isNull() || right.isNull();
	return (!hasNull) && left.x <= right.x && left.y <= right.y;
}

bool operator>=(Vector2I left, Vector2I right)
{
	bool hasNull = left.isNull() || right.isNull();
	return (!hasNull) && left.x >= right.x && left.y >= right.y;
}

bool operator==(Vector2I left, Vector2I right)
{
	bool hasNull = left.isNull() || right.isNull();
	return hasNull ? left.isNull() && right.isNull() : left.x == right.x && left.y == left.y;
}

bool operator!=(Vector2I left, Vector2I right)
{
	bool hasNull = left.isNull() || right.isNull();
	return hasNull ? left.isNull() != right.isNull() : left.x != right.x && left.y != left.y;
}

void from_json(const json& j, Vector2I& v)
{
    if (j.find("x") != j.end())
        v.x = j["x"];

    if (j.find("y") != j.end())
        v.y = j["y"];
}

void to_json(json& j, const Vector2I& v) {
    j = json{ { "x", v.x },{ "y", v.y } };
}



AnimationSetting::AnimationSetting()
{
    this->anchor = Vector2::null;
    this->size = Vector2I::null;
    this->position = Vector2I::null;
    this->filename = "";
    this->duration = -1;
}

bool AnimationSetting::Build(AnimationSetting newSetting)
{
    //Build Data With newSetting
    if (newSetting.anchor != Vector2::null)
        this->anchor = newSetting.anchor;
    if(newSetting.duration != -1)
        this->duration = newSetting.duration;
    if (newSetting.filename != "")
        this->filename = newSetting.filename;
    if (newSetting.position != Vector2I::null)
        this->position = newSetting.position;
    if (newSetting.size != Vector2I::null)
        this->size = newSetting.size;

    //Fix Data without setting
    bool safe = this->anchor == Vector2::null
        || this->duration == -1
        || this->position == Vector2I::null
        || this->size == Vector2I::null
        || this->filename == "";

    return safe;
}

void from_json(const json & j, AnimationSetting & as)
{
    if (j.find("anchor") != j.end())
        as.anchor = j["anchor"];
    if (j.find("size") != j.end())
        as.size = j["size"];
    if (j.find("position") != j.end())
        as.position = j["position"];
    if (j.find("filename") != j.end())
        as.filename = j["filename"].get<string>();
    if (j.find("duration") != j.end())
        as.duration = j["duration"];
}

void to_json(json & j, const AnimationSetting & as)
{
    j = json{
        { "anchor"   ,as.anchor   },
        { "size"     ,as.size     },
        { "position" ,as.position },
        { "filename" ,as.filename },
        { "duration" ,as.duration }
    };
}

float Time::deltaTime = 0;
DWORD Time::timeStamp = 0;

void Time::Update()
{
	DWORD ts = clock();
	Time::deltaTime = (double)(ts - Time::timeStamp) / 1000;
	Time::timeStamp = ts;
}

}