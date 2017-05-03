#pragma once
#include "gamebehavior.h"

using namespace game_engine;

class BoxParent : public GameBehaviour
{
    public:
		BoxParent(GameObject* gobj) : GameBehaviour(gobj)
		{
            RegisterEvent(BroadcastEvent::ButtonPressed);
            RegisterEvent(BroadcastEvent::ButtonRelease);
		}

        void Start() override;
        void Update() override;
        void CheckData();
        void OnRecivedBroadcast(BroadcastMessageData bmd) override;
    private:
        string boxdata[4][4];

        bool isSended;
        bool button1;
        bool button2;
};