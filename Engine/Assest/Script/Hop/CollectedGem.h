#pragma once
#include "gamebehavior.h"

using namespace game_engine;

class CollectedGem : public GameBehaviour
{
    public:
		static int count;
		CollectedGem(GameObject* gobj) : GameBehaviour(gobj) {}
		void Start() override;
		void Update() override;
		string name;
    protected:
    private:
		double time = 0.0;
};