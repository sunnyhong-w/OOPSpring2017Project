#include "stdafx.h"
#include"enginelib.h"

namespace game_engine
{

/////////////////////////////////////////////////////////////
// Vector2
/////////////////////////////////////////////////////////////

const Vector2 Vector2::up     = Vector2( 0,  1);
const Vector2 Vector2::down   = Vector2( 0, -1);
const Vector2 Vector2::left   = Vector2( 1,  0);
const Vector2 Vector2::right  = Vector2(-1,  0);
const Vector2 Vector2::one    = Vector2( 1,  1);
const Vector2 Vector2::zero   = Vector2( 0,  0);
const Vector2 Vector2::null   = Vector2(true);

Vector2::Vector2()
{
    this->x = 0;
    this->y = 0;
}

Vector2::Vector2(bool setNull) : nullVector(true)
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

Vector2I::Vector2I()
{
    this->x = 0;
    this->y = 0;
}

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

Vector2I Vector2I::operator+(Vector2 adder)
{
    return Vector2I(this->x + adder.x, this->y + adder.y);
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


}