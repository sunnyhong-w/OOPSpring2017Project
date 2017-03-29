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
		json send;
		send["data"]["room"] = "red";
		send["data"]["name"] = this->gameObject->GetName();
		send["event"] = "button unpressed";
		game_framework::CGame::Instance()->BoardcastMessage(send);
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
		json send;
		send["data"]["room"] = "red";
		send["data"]["name"] = this->gameObject->GetName();
		send["event"] = "button pressed";
		game_framework::CGame::Instance()->BoardcastMessage(send);
	}
	isPressing = true;
	ticker = 150;
}

void Button::OnCollisionStay(Collider * c)
{
	ticker = 150;
}

void Button::OnCollisionExit(Collider * c)
{
	ticker = 150;
}
