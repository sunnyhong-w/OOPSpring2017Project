#pragma once
#include "gamebehavior.h"

using namespace game_engine;

class YellowBox : public GameBehaviour
{
    public:
		YellowBox(GameObject* gobj) : GameBehaviour(gobj) {}
		void Start() override;
		void Update() override;
		Vector2 GetWorldPos(Vector2 screenPos);
    protected:
    private:
};