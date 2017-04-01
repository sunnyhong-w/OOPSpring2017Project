#include "stdafx.h"
#include "Engine.h"
#include "GreenBox.h" //Include you header

void GreenBox::OnRecivedBoardcast(BoardcastMessageData bmd)
{
    char newstate[4][4];

    for (json::iterator it = bmd.data.begin(); it != bmd.data.end(); it++)
    {
        string name = it.key();


        Vector2I pos = it.value()["position"];
        json boxdata = it.value()["data"];

        for (auto j : boxdata)
        {
            Vector2I tpos = pos + j["position"];
            newstate[tpos.x][tpos.y] = name[0];
        }
    }

    bool boxState = true;

    for (int y = 0; y < 4; y++)
    {
        for (int x = 0; x < 4; x++)
        {
            if (target[x][y] != 'x' && target[x][y] != newstate[x][y])
            {
                boxState = false;
                break;
            }
        }
    }

    if (boxState)
        BoxON();
    else
        tick = 1;
}

void GreenBox::Start()
{
    BoxNum = this->gameObject->spriteRenderer->GetSourcePos().y / 64;
    BoxOff();
}

void GreenBox::Update()
{
    if (tick == 1)
        BoxOff();

    if (tick > 0)
        tick--;
}

void GreenBox::SetData(string dt)
{
    for (int y = 0; y < 4; y++)
    {
        for (int x = 0; x < 4; x++)
        {
            target[x][y] = dt[x + y * 4];
        }
    }
}

void GreenBox::BoxON()
{
    Vector2I pos(64, 64);
    pos = pos * Vector2I(1, BoxNum);

    this->gameObject->spriteRenderer->SetSourcePos(pos);
    this->gameObject->collider->enable = true;

    tick = 0;
}

void GreenBox::BoxOff()
{
    Vector2I pos(64, 64);
    pos = pos * Vector2I(0, BoxNum);

    this->gameObject->spriteRenderer->SetSourcePos(pos);
    this->gameObject->collider->enable = false;
}
