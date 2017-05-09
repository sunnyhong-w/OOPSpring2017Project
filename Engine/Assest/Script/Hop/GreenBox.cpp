#include "stdafx.h"
#include "Engine.h"
#include "GreenBox.h" //Include you header

void GreenBox::OnRecivedBroadcast(BroadcastMessageData bmd)
{
    char newstate[4][4];

    for (json::iterator it = bmd.data.begin(); it != bmd.data.end(); ++it)
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
            char c = target[x][y];

            if (c != 'x')
            {
                if (c >= 'A' && c <= 'Z' && c != newstate[x][y])
                {
                    boxState = false;
                    break;
                }
                else if (c >= 'a' && c <= 'z' && c + ('A' - 'a') == newstate[x][y])
                {
                    boxState = false;
                    break;
                }
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
    this->gameObject->SetLayer(Layer::Tile);

    spritemap['W'] = 0;
    spritemap['B'] = 8;
    spritemap['G'] = 16;
    spritemap['R'] = 24;
    spritemap['Y'] = 32;
    spritemap['w'] = 0;
    spritemap['b'] = 8;
    spritemap['g'] = 16;
    spritemap['r'] = 24;
    spritemap['y'] = 32;

    this->gameObject->spriteRenderer->LoadBitmapData("GreenSprite");
    dotSurfaceID = this->gameObject->spriteRenderer->GetSurfaceID();

    this->gameObject->spriteRenderer->LoadBitmapData("GreenBox");
    boxSurfaceID = this->gameObject->spriteRenderer->GetSurfaceID();
    this->gameObject->spriteRenderer->SetSize(Vector2I(64, 64));

    BoxOff();
}

void GreenBox::Update()
{
    if (tick == 1)
        BoxOff();

    if (tick > 0)
        tick--;
}

void GreenBox::Draw(Vector2I v2)
{
    if(sw)
        this->gameObject->spriteRenderer->SetSourcePos(Vector2I(64, 0));
    else
        this->gameObject->spriteRenderer->SetSourcePos(Vector2I(0, 0));

    this->gameObject->spriteRenderer->Draw(v2);

    this->gameObject->spriteRenderer->UnsafeSetSurfaceID(dotSurfaceID);
    this->gameObject->spriteRenderer->SetSize(Vector2I(8, 8));

    for (int y = 0; y < 4; y++)
    {
        for (int x = 0; x < 4; x++)
        {
            char c = target[x][y];

            if (c != 'x')
            {
                this->gameObject->spriteRenderer->SetSourcePos(Vector2I(spritemap[c], 0));
                this->gameObject->spriteRenderer->Draw(v2 - Vector2I(16,16) - Vector2I(8, 8) * Vector2I(x , y));

                if (c >= 'a' && c <= 'z')
                {
                    this->gameObject->spriteRenderer->SetSourcePos(Vector2I(40, 0));
                    this->gameObject->spriteRenderer->Draw(v2 - Vector2I(16, 16) - Vector2I(8, 8) * Vector2I(x, y));
                }
            }
        }
    }

    this->gameObject->spriteRenderer->UnsafeSetSurfaceID(boxSurfaceID);
    this->gameObject->spriteRenderer->SetSize(Vector2I(64, 64));
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
    if (!sw)
    {
        this->gameObject->collider->SetEnable(true);
        tick = 0;
        sw = true;

        auto cam = GameObject::findGameObjectByName("Camera");
        auto player = GameObject::findGameObjectByName("Player");
        if (cam != nullptr && player != nullptr && player->GetComponent<Player>()->GetRoomName() == "Green")
        {
            cam->GetComponent<Camera>()->Shake(1, 4, 10);
            AudioPlayer::GetSource("GreenFit")->PlayOneShot();
        }
    }
}

void GreenBox::BoxOff()
{
    if (sw)
    {
        this->gameObject->collider->SetEnable(false);
        sw = false;

        auto cam = GameObject::findGameObjectByName("Camera");
        auto player = GameObject::findGameObjectByName("Player");
        if (cam != nullptr && player != nullptr && player->GetComponent<Player>()->GetRoomName() == "Green")
        {
            cam->GetComponent<Camera>()->Shake(1, 4, 10);
            AudioPlayer::GetSource("GreenFit")->PlayOneShot();
        }
    }
}
