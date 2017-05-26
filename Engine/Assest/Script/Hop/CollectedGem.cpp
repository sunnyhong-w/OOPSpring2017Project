#include "stdafx.h"
#include "Engine.h"
#include "CollectedGem.h" //Include you header

void CollectedGem::Start()
{
	this->gameObject->GetComponent<SpriteRenderer>()->SetEnable(false);
	
	int index = 0;

	if (this->name == "GreenGem")
		index = 1;
	else if (this->name == "RedGem")
		index = 2;
	else if (this->name == "WhiteGem")
		index = 3;
	else if (this->name == "YellowGem")
		index = 4;
	else if (this->name == "PurpleGem")
		index = 5;
	time = index * 0.2;
}

void CollectedGem::Update()
{
	if (!this->gameObject->GetComponent<SpriteRenderer>()->GetEnable())
	{
		if (GameStatus::status.find("0") != GameStatus::status.end())
		{
			if (GameStatus::status["0"].find(this->name) != GameStatus::status["0"].end())
			{
				if ((int)GameStatus::status["0"][this->name] == 2)
				{
					this->gameObject->GetComponent<SpriteRenderer>()->SetEnable(true);
				}
			}
		}
	}

	float d = 1;
	this->gameObject->spriteRenderer->SetOffset(Vector2I(0, (int)Easing::easeFT(Easing::easeInOutSine, Easing::backForth(time, d, Time::deltaTime), -10, 10, d)));

}
