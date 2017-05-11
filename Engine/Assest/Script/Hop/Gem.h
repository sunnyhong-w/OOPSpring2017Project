#pragma once
#include "gamebehavior.h"

using namespace game_engine;

class Gem : public GameBehaviour
{
    public:
		Gem(GameObject* gobj) : GameBehaviour(gobj) {

		}
		void Start() override;
		void Update() override;
		void OnCollisionEnter(Collider* c) override;
		void OnRecivedBroadcast(BroadcastMessageData bmd) override;
    private:
        string name;
		Vector2 currentVelocity;
		Vector2 radius;
};