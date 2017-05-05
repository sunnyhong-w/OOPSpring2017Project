#pragma once
#include"gamebehavior.h"

using namespace game_engine;

class Spring : public GameBehaviour
{
public:
	Spring(GameObject* gobj) : GameBehaviour(gobj) {}
	void Start() override;
	void OnAnimationEnd(AnimationData ad) override;
	void OnCollisionStay(Collider* c) override;
	void OnCollisionExit(Collider* c) override;
	int speed;
private:
	bool pressed;
	bool canBounce;
};