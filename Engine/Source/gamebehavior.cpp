#include "stdafx.h"
#include <ddraw.h>
#include "gamebehavior.h"
#include "gameobject.h"

game_engine::GameBehaviour::GameBehaviour(GameObject * gobj) : Component(gobj, true)
{
    gobj->gamebehaviorSet.insert(this);
}

game_engine::GameBehaviour::~GameBehaviour()
{
    this->gameObject->gamebehaviorSet.erase(this->gameObject->gamebehaviorSet.find(this));
}
