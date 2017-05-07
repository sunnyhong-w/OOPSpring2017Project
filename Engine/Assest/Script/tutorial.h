#pragma once
#include"gamebehavior.h"

using namespace game_engine;

class Tutorial : public GameBehaviour
{
    public:
		Tutorial(GameObject* gobj) : GameBehaviour(gobj) { }
        void Start() override;
        void Update() override;
    private:
};