#include "stdafx.h"
#include"enginelib.h"
#include"easing.h"

namespace game_engine
{

/////////////////////////////////////////////////////////////
// Vector2
/////////////////////////////////////////////////////////////

const Vector2 Vector2::up     = Vector2( 0, -1);
const Vector2 Vector2::down   = Vector2( 0,  1);
const Vector2 Vector2::left   = Vector2(-1,  0);
const Vector2 Vector2::right  = Vector2( 1,  0);
const Vector2 Vector2::one    = Vector2( 1,  1);
const Vector2 Vector2::zero   = Vector2( 0,  0);
const Vector2 Vector2::null   = Vector2(true);

const Vector2I Vector2I::up   = Vector2I(0, -1);
const Vector2I Vector2I::down = Vector2I(0, 1);
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
    return Vector2(this->x + adder.x, this->y + adder.y);
}

Vector2 Vector2::SmoothDamp(Vector2 currentPosition, Vector2 target, Vector2 &currentVelocity, float smoothTime, float maxSpeed, float deltaTime)
{
    float dt = deltaTime != 0 ? deltaTime : Time::deltaTime;

    Damping::SmoothDamp(currentPosition.x, currentVelocity.x, target.x, smoothTime, dt);
    Damping::SmoothDamp(currentPosition.y, currentVelocity.y, target.y, smoothTime, dt);

    if (currentVelocity.x > maxSpeed && maxSpeed != 0)
        currentVelocity.x = maxSpeed;
    if (currentVelocity.y > maxSpeed && maxSpeed != 0)
        currentVelocity.y = maxSpeed;

    return currentPosition;
}

Vector2 Vector2::BounceDamp(Vector2 currentPosition, Vector2 target, Vector2 & currentVelocity, float smoothTime, float maxSpeed, float deltaTime)
{
    float dt = deltaTime != 0 ? deltaTime : Time::deltaTime;

    Damping::BounceDamp(currentPosition.x, currentVelocity.x, target.x, smoothTime, dt);
    Damping::BounceDamp(currentPosition.y, currentVelocity.y, target.y, smoothTime, dt);

    if (currentVelocity.x > maxSpeed && maxSpeed != 0)
        currentVelocity.x = maxSpeed;
    if (currentVelocity.y > maxSpeed && maxSpeed != 0)
        currentVelocity.y = maxSpeed;

    return currentPosition;
}

Vector2 Vector2::Damp(Vector2 currentPosition, Vector2 target, Vector2 & currentVelocity, float pd, float f, float smoothTime, float maxSpeed, float deltaTime)
{
	float dt = deltaTime != 0 ? deltaTime : Time::deltaTime;

	Damping::Damp(currentPosition.x, currentVelocity.x, target.x, smoothTime, pd, f, dt);
    Damping::Damp(currentPosition.y, currentVelocity.y, target.y, smoothTime, pd, f, dt);

	if (currentVelocity.x > maxSpeed && maxSpeed != 0)
		currentVelocity.x = maxSpeed;
	if (currentVelocity.y > maxSpeed && maxSpeed != 0)
		currentVelocity.y = maxSpeed;

	return currentPosition;
}

float clamp(float in, float min, float max)
{
    return (in > max ? max : (in < min ? min : in));
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
	return hasNull ? left.isNull() != right.isNull() : left.x != right.x || left.y != right.y;
}

Vector2 Vector2::abs()
{
    return Vector2(std::abs(this->x), std::abs(this->y));
}

Vector2 Vector2::round()
{
	return Vector2(roundf(x), roundf(y));
}

Vector2 Vector2::floor()
{
	return Vector2(floorf(x),floorf(y));
}

Vector2 Vector2::ceil()
{
	return Vector2(ceilf(x),ceilf(y));
}

Vector2 Vector2::ceilSpecial()
{
	return Vector2((x > 0 ? ceilf(x) : floorf(x)), (y > 0 ? ceilf(y) : floorf(y)));
}

Vector2 Vector2::floorSpecial()
{
	return Vector2((x < 0 ? ceilf(x) : floorf(x)), (y < 0 ? ceilf(y) : floorf(y)));
}

Vector2 Vector2::side()
{
	Vector2 ret = Vector2::zero;

	if (x > 0)
		ret.x = 1;
	else if(x < 0)
		ret.x = -1;
	
	if (y > 0)
		ret.y = 1;
	else if (y < 0)
		ret.y = -1;

	return ret;
}

Vector2 Vector2::sliceRound(Vector2 slice)
{
    return ((*this) /slice).round() * slice;
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
	return hasNull ? left.isNull() != right.isNull() : left.x != right.x || left.y != left.y;
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

void from_json(const json & j, Rect & v)
{
    if (j.find("x") != j.end())
        v.x = j["x"];

    if (j.find("y") != j.end())
        v.y = j["y"];

    if (j.find("w") != j.end())
        v.w = j["h"];

    if (j.find("w") != j.end())
        v.h = j["h"];
}

void to_json(json & j, const Rect & v)
{
    j = json{ { "x", v.x },{ "y", v.y },{ "w", v.w },{ "h", v.h } };
}

Rect::Rect()
{
    this->x = 0;
    this->y = 0;
    this->w = 0;
    this->h = 0;
}             

Rect::Rect(int x, int y, int w, int h)
{
    this->x = x;
    this->y = y;
    this->w = w;
    this->h = h;
}

AnimationSetting::AnimationSetting()
{
    this->filename = "";
    this->duration = -1;
    this->rotated = false;
    this->trimmed = false;
    this->frame = Rect(0, 0, 0, 0);
    this->sourceSize = Vector2I(0, 0);
    this->spriteSourceSize = Rect(0, 0, 0, 0);
}

void from_json(const json & j, AnimationSetting & as)
{
    as.filename = j["filename"].get<string>();
    as.duration = j["duration"];
    as.rotated = j["rotated"];
    as.trimmed = j["trimmed"];
    as.frame = j["frame"];
    as.sourceSize = j["sourceSize"];
    as.spriteSourceSize = j["spriteSourceSize"];
}

void to_json(json & j, const AnimationSetting & as)
{
    j = json{
        { "filename" , as.filename },
        { "duration" , as.duration },
        { "rotated" , as.rotated },
        { "trimmed" , as.trimmed },
        { "fram" , as.frame },
        { "sourceSiz" , as.sourceSize },
        { "spriteSourceSize" , as.spriteSourceSize }
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

void from_json(const json &j, Layer &l)
{
    l = (Layer)j.get<int>();
}

void from_json(const json &j, Tag &t)
{
    t = (Tag)j.get<int>();
}

void from_json(const json &j, SortingLayer &rd)
{
    rd = (SortingLayer)j.get<int>();
}

void from_json(const json & j, BroadcastEvent & be)
{
	be = (BroadcastEvent)j.get<int>();
}

void to_json(json & j, const BroadcastEvent & be)
{
	j = (int)be;
}

void from_json(const json & j, BroadcastMessageData & bmd)
{
	bmd.event = j["event"];
	bmd.data = j["data"];
	bmd.position = j["position"];
	bmd.size = j["size"];
	bmd.sender = j["sender"].get<string>();
}

void to_json(json & j, const BroadcastMessageData & bmd)
{
	j["event"] = bmd.event;
	j["data"] = bmd.data;
	j["position"] = bmd.position;
	j["size"] = bmd.size;
	j["sender"] = bmd.sender;
}

}