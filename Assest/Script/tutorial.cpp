#include "stdafx.h"
#include "Engine.h"
#include "tutorial.h"

void Tutorial::ParseJSON(json j)
{
    for (json::iterator it = j.begin(); it != j.end(); it++)
    {
        if (it.key() == "speed")
            this->speed = it.value().get<int>();
    }
}

void Tutorial::Start()
{
    time = 0;
}

void Tutorial::Update()
{
    int l = 60;
    transform->position.x = Easing::easeFT(Easing::easeInOutSine, 60 - abs(time - 60), 240, 480, 60);
    transform->position = Easing::easeFT(Easing::easeInBounce, l - abs(time - l), Vector2(240, 320), Vector2(480, 120), l);
    GameObject* goj = GameObject::findGameObjectByName("TextRenderer");

    if (goj != nullptr)
    {
        goj->GetComponent<TextRenderer>()->LoadText("test");
        goj->GetComponent<TextRenderer>()->SetPosition(Vector2I(0, 0));
    }

    time++;

    if (time > l * 2)
        time = 0;
}

void Tutorial::OnRecivedBoardcast(json j)
{
}
