#pragma once
#include "gamebehavior.h"

using namespace game_engine;

class GreenBox : public GameBehaviour
{
    public:
    	GreenBox(GameObject* gobj) : GameBehaviour(gobj)
        {
            eventListener[BroadcastEvent::UpdateBoxPosition] = true;
        };

        void OnRecivedBroadcast(BroadcastMessageData bmd) override;
        void Start() override;
        void Update() override;
        void Draw(Vector2I v2 = Vector2I::zero) override;

        void SetData(string dt);

    private:
        void BoxON();
        void BoxOff();

        int boxSurfaceID;
        int dotSurfaceID;
        map<char, int> spritemap;

        int tick = 0;
        char target[4][4];
        bool sw;
};