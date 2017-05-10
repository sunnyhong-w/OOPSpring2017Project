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

void SlideScreenEffectShadingFunction(int x, int y, BYTE &r, BYTE &g, BYTE &b, BYTE *data)
{
    r = (1 - (1 - (float)r / 255) * (1 - 0.5)) * 255;
    g = (1 - (1 - (float)g / 255) * (1 - 0.5)) * 255;
    b = (1 - (1 - (float)b / 255) * (1 - 0.5)) * 255;
}
