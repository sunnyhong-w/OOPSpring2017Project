#pragma once
#include "gamebehavior.h"

using namespace game_engine;

class MapEdge : public GameBehaviour
{
    public:
    	MapEdge(GameObject* gobj) : GameBehaviour(gobj)
		{
			target = nullptr;
		};
		void Update() override;
        void OnCollisionStay(Collider* c) override;
        Vector2 targetPosition;
		string targetRoom;
		GameObject *target;
};