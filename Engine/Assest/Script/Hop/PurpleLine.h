#pragma once
#include "gamebehavior.h"

using namespace game_engine;

class PurpleLine : public GameBehaviour
{
    public:
		PurpleLine(GameObject* gobj) : GameBehaviour(gobj) {}
		void Start();
		void Update();
		void OnRecivedBroadcast(BroadcastMessageData bmd);
		void OnDrawGizmos(CDC *pDC);
		static bool lineLinked;
    protected:
    private:
		Vector2I slideWorldPos;
		Vector2I slidePos;
};