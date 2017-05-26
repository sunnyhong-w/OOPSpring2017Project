#pragma once
#include "gamebehavior.h"

using namespace game_engine;

class YellowBox : public GameBehaviour
{
    public:
		YellowBox(GameObject* gobj) : GameBehaviour(gobj) {}
		void Start() override;
        void LateUpdate() override;
		void OnRecivedBroadcast(BroadcastMessageData bmd) override;
    private:
        Vector2I SlidePos;
};