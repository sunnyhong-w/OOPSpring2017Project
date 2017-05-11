#pragma once
#include "gamebehavior.h"

using namespace game_engine;

void SlideScreenEffectShadingFunction(int x, int y, float &r, float &g, float &b, BYTE *data);


struct rgb {
    rgb()
    {
        this->r = 0;
        this->g = 0;
        this->b = 0;
    }

    rgb(double r, double g, double b)
    {
        this->r = r / 255;
        this->g = g / 255;
        this->b = b / 255;
    }

    double r;       // a fraction between 0 and 1
    double g;       // a fraction between 0 and 1
    double b;       // a fraction between 0 and 1
};

class SlideScreenEffect : public GameBehaviour
{
public:
    SlideScreenEffect(GameObject* gobj) : GameBehaviour(gobj) 
    {

    }
    void Start() override;
    void Update() override;
    void Draw(Vector2I v2 = Vector2I::zero) override;
    void OnRecivedBroadcast(BroadcastMessageData bmd) override;
    rgb nowdark;
    rgb targetdark;

    rgb nowlight;
    rgb targetlight;
    float timecount;
    float lerptime;

    int tick = 0;

    static rgb dark;
    static rgb light;
};