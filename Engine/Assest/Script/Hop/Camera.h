#pragma once
#include "gamebehavior.h"

using namespace game_engine;

class Camera : public GameBehaviour
{
    public:
		Camera(GameObject* gobj) : GameBehaviour(gobj) {}
		void Start() override;
		void LateUpdate() override;
    protected:
    private:
		Vector2 playerPos;
		Vector2 velocity;
		GameObject* player;
};