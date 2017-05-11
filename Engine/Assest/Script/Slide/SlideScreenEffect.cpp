#include "stdafx.h"
#include <ddraw.h>
#include "Engine.h"
#include "SlideScreenEffect.h" //Include you header

hsv rgb2hsv(rgb in)
{
    hsv         out;
    double      min, max, delta;

    min = in.r < in.g ? in.r : in.g;
    min = min  < in.b ? min : in.b;

    max = in.r > in.g ? in.r : in.g;
    max = max  > in.b ? max : in.b;

    out.v = max;                                // v
    delta = max - min;
    if (delta < 0.00001)
    {
        out.s = 0;
        out.h = 0; // undefined, maybe nan?
        return out;
    }
    if (max > 0.0) { // NOTE: if Max is == 0, this divide would cause a crash
        out.s = (delta / max);                  // s
    }
    else {
        // if max is 0, then r = g = b = 0              
        // s = 0, v is undefined
        out.s = 0.0;
        out.h = NAN;                            // its now undefined
        return out;
    }
    if (in.r >= max)                           // > is bogus, just keeps compilor happy
        out.h = (in.g - in.b) / delta;        // between yellow & magenta
    else
        if (in.g >= max)
            out.h = 2.0 + (in.b - in.r) / delta;  // between cyan & yellow
        else
            out.h = 4.0 + (in.r - in.g) / delta;  // between magenta & cyan

    out.h *= 60.0;                              // degrees

    if (out.h < 0.0)
        out.h += 360.0;

    return out;
}

rgb hsv2rgb(hsv in)
{
    double      hh, p, q, t, ff;
    long        i;
    rgb         out;

    if (in.s <= 0.0) {       // < is bogus, just shuts up warnings
        out.r = in.v;
        out.g = in.v;
        out.b = in.v;
        return out;
    }
    hh = in.h;
    if (hh >= 360.0) hh = 0.0;
    hh /= 60.0;
    i = (long)hh;
    ff = hh - i;
    p = in.v * (1.0 - in.s);
    q = in.v * (1.0 - (in.s * ff));
    t = in.v * (1.0 - (in.s * (1.0 - ff)));

    switch (i) {
    case 0:
        out.r = in.v;
        out.g = t;
        out.b = p;
        break;
    case 1:
        out.r = q;
        out.g = in.v;
        out.b = p;
        break;
    case 2:
        out.r = p;
        out.g = in.v;
        out.b = t;
        break;

    case 3:
        out.r = p;
        out.g = q;
        out.b = in.v;
        break;
    case 4:
        out.r = t;
        out.g = p;
        out.b = in.v;
        break;
    case 5:
    default:
        out.r = in.v;
        out.g = p;
        out.b = q;
        break;
    }
    return out;
}

rgb SlideScreenEffect::dark;
rgb SlideScreenEffect::light;

void SlideScreenEffect::Update()
{
    if (timecount >= lerptime)
    {
        nowdark = targetdark;
        nowlight = targetlight;
    }
    
    dark.r = nowdark.r + (targetdark.r - nowdark.r) * clamp(timecount, 0, lerptime) / lerptime;
    dark.g = nowdark.g + (targetdark.g - nowdark.g) * clamp(timecount, 0, lerptime) / lerptime;
    dark.b = nowdark.b + (targetdark.b - nowdark.b) * clamp(timecount, 0, lerptime) / lerptime;

    light.r = nowlight.r + (targetlight.r - nowlight.r) * clamp(timecount, 0, lerptime) / lerptime;
    light.g = nowlight.g + (targetlight.g - nowlight.g) * clamp(timecount, 0, lerptime) / lerptime;
    light.b = nowlight.b + (targetlight.b - nowlight.b) * clamp(timecount, 0, lerptime) / lerptime;

    timecount += Time::deltaTime;
}

void SlideScreenEffect::Draw(Vector2I v2)
{
    Vector2I windowsize = Vector2I(GameSetting::GetSizeX(), GameSetting::GetSizeY());

    LPDIRECTDRAWSURFACE surface = NULL;
    game_framework::CDDraw::LoadBitmapA(surface, R"(.\Assest\Bitmap\Slide\Background.bmp)" ,&(SlideScreenEffectShadingFunction));
    
    int blt_flag = DDBLT_WAIT;

    game_framework::CDDraw::BltBackColor(DEFAULT_BG_COLOR);

    game_framework::CDDraw::BltBitmapToBack(surface, blt_flag,
            CRect(CPoint(v2.x ,v2.y), CSize(windowsize.x, windowsize.y)),
            CRect(CPoint(v2.x, v2.y), CSize(windowsize.x, windowsize.y)), true);

    //game_framework::CDDraw::BltBitmapToBack(surface, blt_flag,
    //    CRect(CPoint(v2.x + windowsize.x, v2.y - windowsize.y * 0.5f), CSize(windowsize.x, windowsize.y)),
    //    CRect(CPoint(v2.x, v2.y), CSize(windowsize.x, windowsize.y)), true);
    //
    //game_framework::CDDraw::BltBitmapToBack(surface, blt_flag,
    //    CRect(CPoint(v2.x + windowsize.x, v2.y + windowsize.y * 0.5f), CSize(windowsize.x, windowsize.y)),
    //    CRect(CPoint(v2.x, v2.y), CSize(windowsize.x, windowsize.y)), true);

    surface->Release();
}

void SlideScreenEffect::OnRecivedBroadcast(BroadcastMessageData bmd)
{
    string newroom = bmd.data["name"].get<string>();

    nowdark = dark;
    nowlight = light;

    if (newroom == "Blue")
    {
        targetdark = rgb(102, 131, 255);
        targetlight = rgb(208, 209, 255);
        timecount = 0;
    }
    else if (newroom == "Red")
    {
        targetdark = rgb(255, 112, 102);
        targetlight = rgb(255, 219, 208);
        timecount = 0;
    }
    else if (newroom == "Yellow")
    {
        targetdark = rgb(255, 216, 102);
        targetlight = rgb(255, 251, 208);
        timecount = 0;
    }
    else if (newroom == "Green")
    {
        targetdark = rgb(196, 255, 102);
        targetlight = rgb(229, 255, 208);
        timecount = 0;
    }
    else if (newroom == "White")
    {
        targetdark = rgb(240, 240, 240);
        targetlight = rgb(248, 248, 248);
        timecount = 0;
    }
}

void SlideScreenEffectShadingFunction(int x, int y, float &r, float &g, float &b, BYTE *data)
{
    rgb ret;

    if ((int)(r * 255) == 102)
    {
        ret.r = SlideScreenEffect::dark.r;
        ret.g = SlideScreenEffect::dark.g;
        ret.b = SlideScreenEffect::dark.b;
    }
    else if((int)(r * 255) == 208)
    {
        ret.r = SlideScreenEffect::light.r;
        ret.g = SlideScreenEffect::light.g;
        ret.b = SlideScreenEffect::light.b;
    }
    else
    {
        ret.r = r;
        ret.g = g;
        ret.b = b;
    }

    r = ret.r;
    g = ret.g;
    b = ret.b;
}
