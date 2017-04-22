#include "stdafx.h"
#include "Engine.h"
#include "CollectedGem.h" //Include you header

void CollectedGem::Start()
{
	this->gameObject->GetComponent<SpriteRenderer>()->SetEnable(false);
	
	
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
}
