#pragma once
#include "gamebehavior.h"

using namespace game_engine;

class Camera : public GameBehaviour
{
    public:
		Camera(GameObject* gobj) : GameBehaviour(gobj) {}
		void Start() override;
		void LateUpdate() override;
		void OnDrawGizmos(CDC *pDC) override;
    protected:
    private:
		Vector2 playerPos;
		Vector2 velocity;
		GameObject* player;
};