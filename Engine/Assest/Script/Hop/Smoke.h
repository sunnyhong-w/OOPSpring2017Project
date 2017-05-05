#pragma once
#include "gamebehavior.h"

using namespace game_engine;

class Smoke : public GameBehaviour
{
    public:
		Smoke(GameObject* gobj) : GameBehaviour(gobj) {};
		void OnAnimationEnd(AnimationData ad) override;
		void Start() override;
		void Update() override;
    protected:
    private:
		float frame;
};