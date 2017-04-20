#pragma once
#include "gamebehavior.h"

using namespace game_engine;

class SpawnPlayer : public GameBehaviour
{
    public:
        SpawnPlayer(GameObject* gobj) :GameBehaviour(gobj) { }
        ~SpawnPlayer() {};

        void ParseJSON(json j) override;
        void Start() override;
        void Update() override;
        void OnDrawGizmos(CDC *pDC) override;
    protected:
    private:
        int count;
        int target;
		Vector2 pos;
};