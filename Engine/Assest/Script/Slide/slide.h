#pragma once
#include "gamebehavior.h"

using namespace game_engine;

struct PassState
{
    bool top;
    bool bottom;
    bool left;
    bool right;
};

void from_json(const json& j, PassState& ps);
void to_json(json& j, const PassState& ps);

struct SlideBoxSetting 
{
    Vector2I position;
    PassState state;
};

void from_json(const json& j, SlideBoxSetting& sbs);
void to_json(json& j, const SlideBoxSetting& sbs);

class Slide : public GameBehaviour
{
public:
	Slide(GameObject* gobj) : GameBehaviour(gobj) {}
	void ParseJSON(json j) override;
	void Start() override;
	void Update() override;
	void OnRecivedBoardcast(json j) override;
	void OnDrawGizmos(CDC* pDC) override;

    vector<SlideBoxSetting> boxdata;
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