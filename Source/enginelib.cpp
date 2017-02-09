#include"enginelib.h"

namespace game_engine{

/////////////////////////////////////////////////////////////
// Vector2
/////////////////////////////////////////////////////////////

const Vector2 Vector2::up     = Vector2( 0,  1);
const Vector2 Vector2::down   = Vector2( 0, -1);
const Vector2 Vector2::left   = Vector2( 1,  0);
const Vector2 Vector2::right  = Vector2(-1,  0);
const Vector2 Vector2::one    = Vector2( 1,  1);
const Vector2 Vector2::zero   = Vector2( 0,  0);

Vector2::Vector2()
{
    this->x = 0;
    this->y = 0;
}

Vector2::Vector2(float x, float y)
{
    this->x = x;
    this->y = y;
}

}