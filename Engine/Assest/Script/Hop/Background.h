#pragma once
#include "gamebehavior.h"

using namespace game_engine;

class Background : public GameBehaviour
{
    public:
		Background(GameObject* gobj) : GameBehaviour(gobj) {}
		void Start() override;
		void Update() override;
		void SetBackground(string roomName);
    protected:
    private:
		Vector2 cameraPos;
		GameObject *camera;
		GameObject *map;
};