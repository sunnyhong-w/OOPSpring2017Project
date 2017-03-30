#pragma once
#include "gamebehavior.h"

using namespace game_engine;

class GreenBox : public GameBehaviour
{
    public:
    	GreenBox(GameObject* gobj) : GameBehaviour(gobj)
        {
            eventListener[BoardcastEvent::UpdateBoxPosition] = true;
        };

        void OnRecivedBoardcast(BoardcastMessageData bmd) override;
        void Start() override;
        void Update() override;

        void SetData(string dt);

    private:
        void BoxON();
        void BoxOff();

        int tick = 0;
        int BoxNum = 0;
        char target[4][4];
};