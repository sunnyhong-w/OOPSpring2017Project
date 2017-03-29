#pragma once
#include"gamebehavior.h"

using namespace game_engine;

class Player : public GameBehaviour
{
    public:
		Player(GameObject* gobj) : GameBehaviour(gobj) { tmp = true; }
        void ParseJSON(json j) override;
        void Start() override;
        void Update() override;
        void OnRecivedBoardcast(json j) override;
		void OnDrawGizmos(CDC* pDC) override;

        int speed;
		Vector2 vel = Vector2::zero;
		bool bounce;
    private:
		bool tmp = true;
        double time;
        Vector2I textPos = Vector2I::zero;
		Vector2I winpos = Vector2I::zero;
		bool isBouncing;

        float gravity;
        int tiledPixel;
        float MinJumpHeight = .5f;
        float MaxJumpHeight = 2;
        float MaxJumpVelocity;
        float MinJumpVelocity;
        float jumpTimeApex = .4f;
		bool canJump = true;
        void Jump(Vector2 &velocity);
};