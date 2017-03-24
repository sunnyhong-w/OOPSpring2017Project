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
	pressed = true;
}

void Button::OnCollisionExit(Collider * c)
{
	pressed = false;
}
