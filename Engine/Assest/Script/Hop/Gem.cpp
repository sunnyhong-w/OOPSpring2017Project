#include "stdafx.h"
#include "Engine.h"
#include "Gem.h" //Include you header

void Gem::Start()
{			
	if ((int)GameStatus::status["0"] == 2)
			{
				this->transform->SetPosition(Vector2(1100, 970));
				this->gameObject->collider->SetEnable(false);
			}
}

void Gem::OnCollisionEnter(Collider * c)
{
	GameStatus::status[to_string((int)StatusName::Gem)] = 2;
	GameStatus::SaveFile();
	this->transform->SetPosition(Vector2(1100, 970));
	this->gameObject->collider->SetEnable(false);
	
}
