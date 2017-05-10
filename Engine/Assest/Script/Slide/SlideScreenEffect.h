#pragma once
#include "gamebehavior.h"

using namespace game_engine;

void SlideScreenEffectShadingFunction(int x, int y, float &r, float &g, float &b, BYTE *data);

class SlideScreenEffect : public GameBehaviour
{
public:
    SlideScreenEffect(GameObject* gobj) : GameBehaviour(gobj) {}
    void Draw(Vector2I v2 = Vector2I::zero) override;
};