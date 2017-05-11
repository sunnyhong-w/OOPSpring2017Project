#include "stdafx.h"
#include <ddraw.h>
#include "Engine.h"
#include "SlideScreenEffect.h" //Include you header

rgb SlideScreenEffect::dark;
rgb SlideScreenEffect::light;

void SlideScreenEffect::Start()
{
    lerptime = .5f;

    nowdark = rgb(102, 131, 255);
    targetdark = rgb(102, 131, 255);

    nowlight = rgb(208, 209, 255);
    targetlight = rgb(208, 209, 255);

    tick = 0;
    RegisterEvent(BroadcastEvent::ChangeRoom);

    string newroom = "Blue";

    if (GameStatus::status.find("last") != GameStatus::status.end())
    {
        newroom = GameStatus::status["last"]["room"].get<string>();
    }

    if (newroom == "Blue")
    {
        targetdark = rgb(102, 131, 255);
        targetlight = rgb(208, 209, 255);
        nowdark = rgb(102, 131, 255);
        nowlight = rgb(208, 209, 255);
        timecount = 0;
    }
    else if (newroom == "Red")
    {
        targetdark = rgb(255, 112, 102);
        targetlight = rgb(255, 219, 208);
        nowdark = rgb(255, 112, 102);
        nowlight = rgb(255, 219, 208);
        timecount = 0;
    }
    else if (newroom == "Yellow")
    {
        targetdark = rgb(255, 216, 102);
        targetlight = rgb(255, 251, 208);
        nowdark = rgb(255, 216, 102);
        nowlight = rgb(255, 251, 208);
        timecount = 0;
    }
    else if (newroom == "Green")
    {
        targetdark = rgb(196, 255, 102);
        targetlight = rgb(229, 255, 208);
        nowdark = rgb(196, 255, 102);
        nowlight = rgb(229, 255, 208);
        timecount = 0;
    }
    else if (newroom == "White")
    {
        targetdark = rgb(240, 240, 240);
        targetlight = rgb(248, 248, 248);
        nowdark = rgb(240, 240, 240);
        nowlight = rgb(248, 248, 248);
        timecount = 0;
    }
}

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
