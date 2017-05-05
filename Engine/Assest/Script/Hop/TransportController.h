#pragma once
#include "gamebehavior.h"

using namespace game_engine;

class TransportController : public GameBehaviour
{
    public:
        TransportController(GameObject* gobj) : GameBehaviour(gobj) 
        {
            RegisterEvent(BroadcastEvent::UpdateBoxPosition);
        };
        void OnRecivedBroadcast(BroadcastMessageData bmd) override;
    protected:
    private:
        map <string, map<string, GameObject*>> edgeMapGameObject;
};