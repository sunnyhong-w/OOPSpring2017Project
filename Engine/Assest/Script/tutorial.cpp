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
    //this->gameObject->GetComponent<SpriteRenderer>()->enable = false;
    GameObject* goj = GameObject::findGameObjectByName("TextRenderer");

    if (goj != nullptr)
    {
        goj->GetComponent<TextRenderer>()->LoadText("test");
    }
}

void Tutorial::Update()
{
    int l = 60;
    //transform->position = Easing::easeFT(Easing::easeInBounce, l - abs(time - l), Vector2(240, 320), Vector2(480, 120), l);
    GameObject* gobj = GameObject::findGameObjectByName("TextRenderer");
	if(gobj != nullptr)
		gobj->GetComponent<TextRenderer>()->SetPosition(textPos);
    winpos = Easing::easeFT(Easing::easeInBounce, l - abs(time - l), Vector2(240, 320), Vector2(480, 120), l).GetV2I();

    //winpos.x = Easing::easeFT(Easing::easeInBounce, l - abs(time - l), 0,800, l);

    if (Input::GetKeyClick(VK_LBUTTON))
        gobj->GetComponent<TextRenderer>()->NextLine();

    time++;

    if (time > l * 2)
        time = 0;

    json j;
    j["MoveWindow"]["x"] = winpos.x;
    j["MoveWindow"]["y"] = winpos.y;
    //game_framework::CGame::Instance()->BoardcastMessage(j, "TEST_A");
}

void Tutorial::OnRecivedBoardcast(json j)
{
}
