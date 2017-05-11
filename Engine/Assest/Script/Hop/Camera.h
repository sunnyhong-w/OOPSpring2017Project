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
		void OnCollisionEnter(Collider* c);
		void OnCollisionStay(Collider* c);
		void Shake(double t, double f, double s);
    protected:
    private:
        void ShakeF();

		double totalTime;
		double frequency;
		double strength;
		double time;
        int phaserand;
        int phaserandY;
		bool shouldShake;
		Vector2 playerPos;
		Vector2 velocity;
		Vector2 shakeVelocity;
		GameObject* player;
};