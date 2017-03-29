#pragma once
#include "gamebehavior.h"

using namespace game_engine;

class Slide : public GameBehaviour
{
public:
	Slide(GameObject* gobj) : GameBehaviour(gobj) {}
	void ParseJSON(json j) override;
	void Start() override;
	void Update() override;
	void OnRecivedBoardcast(json j) override;
	void OnDrawGizmos(CDC* pDC) override;
private:
	void SendData();

	bool locked;
	Collider* collider;
	Rigidbody* rigidbody;
	Vector2 oWorldPos;
	Vector2 clickPos;
	Vector2 targetPos;
	Vector2 vel;
};