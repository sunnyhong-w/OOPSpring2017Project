#include "stdafx.h"
#include"enginelib.h"

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
    return (right < left);
}

bool operator<=(Vector2 left, Vector2 right)
{
    return !(left > right);
}

bool operator>=(Vector2 left, Vector2 right)
{
    return !(left < right);
}

bool operator==(Vector2 left, Vector2 right)
{
    bool hasNull = left.isNull() || right.isNull();
    return (left.isNull() && right.isNull()) || (!hasNull && left.x == right.x && left.y == left.y);
}

bool operator!=(Vector2 left, Vector2 right)
{
    return !(left == right);
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
    return (right < left);
}

bool operator<=(Vector2I left, Vector2I right)
{
    return (left > right);
}

bool operator>=(Vector2I left, Vector2I right)
{
    return (left < right);
}

bool operator==(Vector2I left, Vector2I right)
{
    bool hasNull = left.isNull() || right.isNull();
    return (left.isNull() && right.isNull()) || (!hasNull && left.x == right.x && left.y == left.y);
}

bool operator!=(Vector2I left, Vector2I right)
{
    return !(left == right);
}

void from_json(const json& j, Vector2I& v)
{
    if (j.find("x") != j.end())
        v.x = j["x"];

    if (j.find("y") != j.end())
        v.y = j["y"];
}


}