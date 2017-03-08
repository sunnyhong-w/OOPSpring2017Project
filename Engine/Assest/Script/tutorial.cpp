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
    //time = 0;
    //this->gameObject->GetComponent<SpriteRenderer>()->enable = false;
    //GameObject* goj = GameObject::findGameObjectByName("TextRenderer");
	//
    //if (goj != nullptr)
    //{
    //    goj->GetComponent<TextRenderer>()->LoadText("test");
    //}
}
void Tutorial::Update()
{
    //int l = 60;
    ////transform->position = Easing::easeFT(Easing::easeInBounce, l - abs(time - l), Vector2(240, 320), Vector2(480, 120), l);
    //GameObject* goj = GameObject::findGameObjectByName("TextRenderer");
    //goj->GetComponent<TextRenderer>()->SetPosition(textPos);
    //textPos = Easing::easeFT(Easing::easeInBounce, l - abs(time - l), Vector2(240, 320), Vector2(480, 120), l).GetV2I();
	//
    //if (Input::GetKeyClick(VK_LBUTTON))
    //    goj->GetComponent<TextRenderer>()->NextLine();
	//
    //time++;
	//
    //if (time > l * 2)
    //    time = 0;
	TRACE("\n\n!\n\n");
	if (Input::GetKeyDown(VK_SPACE))
	{
		//AfxMessageBox("!");
		if(tmp)
			this->gameObject->GetComponent<AnimationController>()->JumpState("Jump");
		else
			this->gameObject->GetComponent<AnimationController>()->JumpState("Move");

		tmp = !tmp;
	}
}

void Tutorial::OnRecivedBoardcast(json j)
{
}
