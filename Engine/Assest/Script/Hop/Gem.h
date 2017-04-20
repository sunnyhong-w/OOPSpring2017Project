#pragma once
#include "gamebehavior.h"

using namespace game_engine;

class Gem : public GameBehaviour
{
    public:
		Gem(GameObject* gobj) : GameBehaviour(gobj) {
			GameStatus::status[to_string((int)StatusName::Gem)] = 0;
		}
		void OnCollisionEnter(Collider* c) override;
    private:
};