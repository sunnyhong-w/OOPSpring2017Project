#include "stdafx.h"
#include "Engine.h"
#include "button.h"

void Button::ParseJSON(json j)
{
}

void Button::Start()
{
	pressed = false;
}

void Button::Update()
{
	json j;
	j["pressed"] = pressed;
	if (lastState != pressed)
		game_framework::CGame::Instance()->BoardcastMessage(j);
}

void Button::OnRecivedBoardcast(json j)
{
}

void Button::OnDrawGizmos(CDC * pDC)
{     
	pDC->TextOutA(0,100,("pressed : "+to_string(pressed)).c_str());
}



void Button::OnCollisionEnter(Collider * c)
{
	int maxHeight = 5;
	pressed = true;
	lastState = pressed;
	Player* pl = c->gameObject->GetComponent<Player>();
	pl->bounce = true;
}

void Button::OnCollisionExit(Collider * c)
{
	pressed = false;
	lastState = pressed;
}
