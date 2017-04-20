#pragma once
#include "gamebehavior.h"

using namespace game_engine;

class TransportController : public GameBehaviour
{
    public:
        TransportController(GameObject* gobj) : GameBehaviour(gobj) 
        {
            RegisterEvent(BoardcastEvent::UpdateBoxPosition);
        };
        void OnRecivedBoardcast(BoardcastMessageData bmd) override;
    protected:
    private:
        map <string, map<string, GameObject*>> edgeMapGameObject;
};