#pragma once
#include "gamebehavior.h"

using namespace game_engine;

class CollectedGem : public GameBehaviour
{
    public:
		CollectedGem(GameObject* gobj) : GameBehaviour(gobj) {}
		void Start() override;
		void Update() override;
		string name;
    protected:
    private:
};