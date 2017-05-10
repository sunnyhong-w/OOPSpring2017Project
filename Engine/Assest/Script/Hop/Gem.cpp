#include "stdafx.h"
#include "Engine.h"
#include "Gem.h" //Include you header

void Gem::Start()
{	
    name = this->gameObject->GetName();
	currentVelocity = Vector2::zero;
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
	radius = Vector2(30.0f, 30.0f);
}

void Gem::Update()
{
	if (GameStatus::status[to_string((int)StatusName::Gem)][name].get<int>() == 2) {
		this->gameObject->spriteRenderer->SetAnchorRaito(Vector2(0.5, 0.5));
		Vector2 minusedScale = (this->transform->scale - Vector2::one*0.01);
		GameObject * player = GameObject::findGameObjectByName("Player");
		Vector2 targetPos = player->transform->GetWorldPosition() + (player->spriteRenderer->GetSize().GetV2()/2);
		
		this->transform->scale = Vector2( clamp(minusedScale.x,0,FLT_MAX),clamp(minusedScale.y,0,FLT_MAX));
		this->transform->SetWorldPosition(targetPos + radius *minusedScale * Vector2(sinf((minusedScale.x + 0.25) * 2 * 3.1415926 * 1.2), cosf((minusedScale.y + 0.25) * 2 * 3.1415926 *1.2)));
		if (this->transform->scale == Vector2::zero)
		{
			Destroy(this->gameObject);
		}
	}
}

void Gem::OnCollisionEnter(Collider * c)
{
	GameStatus::status[to_string((int)StatusName::Gem)][name] = 2;
	GameStatus::SaveFile();
	this->gameObject->collider->SetEnable(false);
    if (name == "RedGem")
        game_framework::CGame::Instance()->SendEvent(WM_CLOSE, 0, 0, "Hop");
	
}

void Gem::OnRecivedBroadcast(BroadcastMessageData bmd)
{
	if (bmd.event == BroadcastEvent::RedRoomFinish)
	{
		this->gameObject->spriteRenderer->SetEnable(true);
		this->gameObject->collider->SetEnable(true);
	}
}
