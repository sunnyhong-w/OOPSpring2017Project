#include "stdafx.h"
#include <ddraw.h>
#include "Engine.h"
#include "SlideScreenEffect.h" //Include you header

void SlideScreenEffect::Draw(Vector2I v2)
{
    Vector2I windowsize = Vector2I(GameSetting::GetSizeX(), GameSetting::GetSizeY());

    auto cdc = game_framework::CDDraw::GetBackCDC();
    LPDIRECTDRAWSURFACE surface = NULL;
    game_framework::CDDraw::CreateSurface(cdc, &surface, &(SlideScreenEffectShadingFunction));
    game_framework::CDDraw::ReleaseBackCDC();

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

void SlideScreenEffectShadingFunction(int x, int y, float &r, float &g, float &b,BYTE *data)
{
    r = 1 - (1 - r) * (1 - 0.5);
    g = 1 - (1 - g) * (1 - 0.5);
    b = 1 - (1 - b) * (1 - 0.5);
}
