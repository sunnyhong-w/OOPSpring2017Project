#pragma once
#include "gamebehavior.h"

using namespace game_engine;

class MapEdge : public GameBehaviour
{
    public:
    	MapEdge(GameObject* gobj) : GameBehaviour(gobj){};
        void OnCollisionStay(Collider* c) override;
        Vector2 targetPosition;
};