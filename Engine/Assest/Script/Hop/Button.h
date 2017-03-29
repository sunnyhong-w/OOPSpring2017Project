#pragma once
#include "gamebehavior.h"

using namespace game_engine;

class Button : public GameBehaviour
{
    public:
		Button(GameObject* gobj) : GameBehaviour(gobj) {}
		void Start() override;
		void Update() override;
		void OnCollisionEnter(Collider* c) override;
		void OnCollisionStay(Collider* c) override;

		void OnCollisionExit(Collider* c) override;
    private:
		int ticker;
		bool isPressing;
};