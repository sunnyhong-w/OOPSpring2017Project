#include "stdafx.h"
#include "Engine.h"
#include "Button.h" //Include you header

void Button::Start()
{
	isPressing = false;
	ticker = 0;
}

void Button::Update()
{
	if (ticker == 0) 
	{
		isPressing = false;	
	}
	else if (ticker == 1)
	{
		json data;
		data["room"] = "red";
		data["name"] = this->gameObject->GetName();
		GameScene::Broadcast(BroadcastEvent::ButtonRelease, data);
		ticker--;
	}
	else
	{
		ticker--;
	}
	
}

void Button::OnCollisionEnter(Collider * c)
{
	if (!isPressing)
	{
		json data;
		data["room"] = "red";
		data["name"] = this->gameObject->GetName();
		GameScene::Broadcast(BroadcastEvent::ButtonPressed, data);
	}
	isPressing = true;
	ticker = 60;
}

void Button::OnCollisionStay(Collider * c)
{
	ticker = 60;
}

void Button::OnCollisionExit(Collider * c)
{
	ticker = 60;
}
