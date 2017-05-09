#include "stdafx.h"
#include "Engine.h"
#include "Gem.h" //Include you header

void Gem::Start()
{	
    name = this->gameObject->GetName();

	if (name == "RedGem")
	{	
		this->gameObject->spriteRenderer->SetEnable(false);
		this->gameObject->collider->SetEnable(false);
		this->RegisterEvent(BroadcastEvent::RedRoomFinish);
	}

	if (GameStatus::status.find("0") !=GameStatus::status.end())
	{
		if (GameStatus::status["0"].find(this->gameObject->GetName()) != GameStatus::status["0"].end())
		{
			if ((int)GameStatus::status["0"][this->gameObject->GetName()] == 2)
			{
				Destroy(this->gameObject);
			}
		}
	}
}

void Gem::OnCollisionEnter(Collider * c)
{
	GameStatus::status[to_string((int)StatusName::Gem)][name] = 2;
	GameStatus::SaveFile();

    if (name == "RedGem")
        game_framework::CGame::Instance()->SendEvent(WM_CLOSE, 0, 0, "Hop");

	Destroy(this->gameObject);
}

void Gem::OnRecivedBroadcast(BroadcastMessageData bmd)
{
	if (bmd.event == BroadcastEvent::RedRoomFinish)
	{
		this->gameObject->spriteRenderer->SetEnable(true);
		this->gameObject->collider->SetEnable(true);
	}
}
