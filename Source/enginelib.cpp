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

Vector2I Vector2::GetV2I()
{
    return Vector2I(this);
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

}