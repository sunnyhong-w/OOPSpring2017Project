#include "stdafx.h"
#include "Engine.h"
#include "Gem.h" //Include you header
#include "GameStatus.h"
void Gem::OnCollisionEnter(Collider * c)
{
	GameStatus::status[to_string((int)StatusName::Gem)] = 1;
	GameStatus::MakeSaveFile();
	Destroy(this->gameObject);
}
