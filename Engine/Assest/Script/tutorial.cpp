#include "stdafx.h"
#include "Engine.h"
#include "tutorial.h"

void Tutorial::ParseJSON(json j)
{

}

void Tutorial::Start()
{

}

void Tutorial::Update()
{
	Vector2 vel = Vector2::zero;
	int speed = 5;
	if (Input::GetKeyTrigger(VK_LEFT))
		vel = vel + Vector2::left * speed;
	if (Input::GetKeyTrigger(VK_RIGHT))
		vel = vel + Vector2::right* speed;
	if (Input::GetKeyTrigger(VK_UP))
		vel = vel + Vector2::down* speed;
	if (Input::GetKeyTrigger(VK_DOWN))
		vel = vel + Vector2::up* speed;
	Rigidbody *rb = gameObject->GetComponent<Rigidbody>();

	if (rb != nullptr)
	{
		rb->velocity = vel;
	}
}

void Tutorial::OnRecivedBoardcast(json j)
{
}
