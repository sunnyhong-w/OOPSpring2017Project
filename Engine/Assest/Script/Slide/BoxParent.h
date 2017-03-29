#pragma once
#include "gamebehavior.h"

using namespace game_engine;

class BoxParent : public GameBehaviour
{
    public:
    	BoxParent(GameObject* gobj) : GameBehaviour(gobj){}
        void Start() override;
        void CheckData();
        void OnRecivedBoardcast(json j) override;
    private:
        string boxdata[4][4];

};