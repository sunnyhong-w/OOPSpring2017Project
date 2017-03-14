#pragma once
#include"gamebehavior.h"

using namespace game_engine;

class Tutorial : public GameBehaviour
{
    public:
		Tutorial(GameObject* gobj) : GameBehaviour(gobj) { tmp = true; }
        void ParseJSON(json j) override;
        void Start() override;
        void Update() override;
        void OnRecivedBoardcast(json j) override;

        int speed;
    private:
		bool tmp = true;
        double time;
        Vector2I textPos = Vector2I::zero;
		Vector2I winpos = Vector2I::zero;
};