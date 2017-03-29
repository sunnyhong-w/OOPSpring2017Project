#pragma once
#include"gamebehavior.h"

using namespace game_engine;

class Spring : public GameBehaviour
{
public:
	Spring(GameObject* gobj) : GameBehaviour(gobj) {}
	void ParseJSON(json j) override;
	void Start() override;
	void Update() override;
	void OnRecivedBoardcast(json j) override;
	void OnDrawGizmos(CDC *pDC) override;

	void OnCollisionEnter(Collider* c) override;
	void OnCollisionStay(Collider* c) override;
	void OnCollisionExit(Collider* c) override;
	int speed;
private:
	bool pressed;
	bool lastState;
};