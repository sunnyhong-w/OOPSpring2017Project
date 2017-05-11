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

struct hsv {
    hsv()
    {
        this->h = 0;
        this->s = 0;
        this->v = 0;
    }

    hsv(double h, double s, double v)
    {
        this->h = h;
        this->s = s;
        this->v = v;
    }

    double h;       // angle in degrees
    double s;       // a fraction between 0 and 1
    double v;       // a fraction between 0 and 1
};

static hsv   rgb2hsv(rgb in);
static rgb   hsv2rgb(hsv in);

class SlideScreenEffect : public GameBehaviour
{
public:
    SlideScreenEffect(GameObject* gobj) : GameBehaviour(gobj) 
    {
        lerptime = .5f;

        nowdark = rgb(102,131,255);
        targetdark = rgb(102, 131, 255);

        nowlight = rgb(208, 209, 255);
        targetlight = rgb(208, 209, 255);

        tick = 0;
    }
    void Update() override;
    void Draw(Vector2I v2 = Vector2I::zero) override;
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