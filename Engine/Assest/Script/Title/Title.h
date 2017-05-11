#pragma once
#include "gamebehavior.h"

using namespace game_engine;

class Title : public GameBehaviour
{
    public:
		Title(GameObject* gobj) : GameBehaviour(gobj) {}
		void Start() override;
		void Update() override;
    protected:
    private:
		Vector2 originScale;
		double time;
};