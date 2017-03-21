#include "stdafx.h"
#include "Engine.h"
#include "tutorial.h"

void Tutorial::ParseJSON(json j)
{
    MinJumpHeight = .5f;
    MaxJumpHeight = 4;
    jumpTimeApex = .4f;
    tiledPixel = 32;
}

void Tutorial::Start()
{
    gravity =(2 * MaxJumpHeight) / pow(jumpTimeApex, 2);
    MaxJumpVelocity = -1 * gravity * jumpTimeApex;
    MinJumpVelocity = sqrt(2 * abs(gravity) * MinJumpHeight);
}

void Tutorial::Update()
{
    Rigidbody *rb = gameObject->GetComponent<Rigidbody>();

	
	int speed = 3;
    //vel = rb->velocity;

    if (rb->colliderInfo.bottom || rb->colliderInfo.top)
        vel.y = 0;

    vel.x = 0;

    if (Input::GetKeyClick(VK_LBUTTON))
        this->transform->SetWorldPosition(Input::GetMousePos().GetV2());

	if (Input::GetKeyPressing('A') || Input::GetKeyPressing(VK_LEFT))
        vel.x = -1 * speed;
	if (Input::GetKeyPressing('D') || Input::GetKeyPressing(VK_RIGHT))
        vel.x =  speed;
    if (Input::GetKeyDown('W') || Input::GetKeyDown(VK_SPACE))
        vel.y = MaxJumpVelocity;

    vel.y += gravity * Time::deltaTime;
    rb->velocity = vel * tiledPixel * Time::deltaTime;
}

void Tutorial::OnRecivedBoardcast(json j)
{
}

void Tutorial::Jump(Vector2 & velocity)
{
    velocity.y = MaxJumpVelocity;
}
