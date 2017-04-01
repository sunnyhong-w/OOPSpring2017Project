#include "stdafx.h"
#include "Engine.h"
#include "spring.h"

void Spring::ParseJSON(json j)
{
}

void Spring::Start()
{
	pressed = false;
}

void Spring::Update()
{
}

void Spring::OnRecivedBoardcast(BoardcastMessageData bmd)
{
}

void Spring::OnDrawGizmos(CDC * pDC)
{     
	pDC->TextOutA(0,100,("pressed : "+to_string(pressed)).c_str());
}



void Spring::OnCollisionEnter(Collider * c)
{
	
}

void Spring::OnCollisionStay(Collider * c)
{
	int maxHeight = 5;
	pressed = true;
	lastState = pressed;
	Player* pl = c->gameObject->GetComponent<Player>();
	if (pl->gameObject->rigidbody->colliderInfo.bottom)
	{
		pl->bounce = true;
	}
	TRACE("\n---------why----------\n");
}

void Spring::OnCollisionExit(Collider * c)
{
	pressed = false;
	lastState = pressed;
}
