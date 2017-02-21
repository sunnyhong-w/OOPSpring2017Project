#pragma once
#include"gamebehavior.h"

using namespace game_engine;

class Tutorial : public GameBehaviour
{
public:
	Tutorial(GameObject *gobj) : GameBehaviour(gobj) {}
	void ParseJSON(json j) override;
	void Start() override;
	void Update() override;
	void OnRecivedBoardcast(json j) override;

	int speed;
private:
	double time;
};