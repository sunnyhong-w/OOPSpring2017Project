#include "stdafx.h"
#include "Engine.h"
#include "Gem.h" //Include you header

void Gem::Start()
{	
	if (this->gameObject->GetName() == "RedGem")
	{	
		this->gameObject->spriteRenderer->SetEnable(false);
		this->gameObject->collider->SetEnable(false);
		this->RegisterEvent(BoardcastEvent::RedRoomFinish);
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
	GameStatus::status[to_string((int)StatusName::Gem)][this->gameObject->GetName()] = 2;
	GameStatus::SaveFile();
	Destroy(this->gameObject);
}

void Gem::OnRecivedBroadcast(BoardcastMessageData bmd)
{
	if (bmd.event == BoardcastEvent::RedRoomFinish)
	{
		this->gameObject->spriteRenderer->SetEnable(true);
		this->gameObject->collider->SetEnable(true);
	}
}
