#pragma once
#include "gamebehavior.h"

using namespace game_engine;

class BoxParent : public GameBehaviour
{
    public:
		BoxParent(GameObject* gobj) : GameBehaviour(gobj)
		{
			eventListener[BoardcastEvent::ButtonPressed] = true;
			eventListener[BoardcastEvent::ButtonRelease] = true;
		}

        void Start() override;
        void Update() override;
        void CheckData();
        void OnRecivedBoardcast(BoardcastMessageData bmd) override;
    private:
        string boxdata[4][4];

        bool isSended;
        bool button1;
        bool button2;
};