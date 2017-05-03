#include"StdAfx.h"
#include"Engine.h"
#include "textStamp.h"
#include<fstream>
#include <codecvt>
using namespace std;

void TextStamp::ParseJSON(json j)
{
}

void TextStamp::Start()
{
}

void TextStamp::Update()
{
    shake();
}

void TextStamp::OnRecivedBroadcast(BroadcastMessageData bmd)
{
}

void TextStamp::Stamp()
{
    SR->SetSourcePos(this->bitmapPos * size);
    SR->Draw();
}

void TextStamp::Setfont(UINT surfaceID, Vector2I size, Vector2I bitmapPos)
{
    SR = this->gameObject->AddComponentOnce<SpriteRenderer>();
    SR->UnsafeSetSurfaceID(surfaceID);
    SR->SetSize(size);
    this->size = size;
    this->bitmapPos = bitmapPos;
}

void TextStamp::shake()
{
    int temp = Easing::backForth(dp, 3);
    SR->SetOffset(Vector2I(0, Easing::easeFT(Easing::easeInOutSine, temp, -2, 2, 3)));
    //this->transform->position = this->transform->position + dp.GetV2();
}

void TextStamp::randomShake()
{
    SR->SetOffset(Vector2I(rand() % 5 - 2, rand() % 5 - 2));
}

void TextStamp::setPosition(Vector2 stampPos)
{
    this->transform->SetPosition(stampPos);
}
