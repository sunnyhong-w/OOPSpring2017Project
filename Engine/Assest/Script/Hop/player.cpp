#include "stdafx.h"
#include "Engine.h"
#include "player.h"

void Player::ParseJSON(json j)
{
    MinJumpHeight = .5f;
    MaxJumpHeight = 3;
    jumpTimeApex = .4f;
    tiledPixel = 32;
}

void Player::Start()
{
    gravity =(2 * MaxJumpHeight) / pow(jumpTimeApex, 2);
    MaxJumpVelocity = -1 * gravity * jumpTimeApex;
    MinJumpVelocity = sqrt(2 * abs(gravity) * MinJumpHeight);
	bounce = false;
	isBouncing = false;
}

void Player::Update()
{
    Rigidbody *rb = gameObject->rigidbody;
    rb->TimeSliceCollision = true;
	
	int speed = 5;

    if (rb->colliderInfo.bottom || rb->colliderInfo.top)
        vel.y = 0;

    vel.x = 0;

	if (Input::GetKeyPressing('A') || Input::GetKeyPressing(VK_LEFT))
        vel.x += -1 * speed;
	
	if (Input::GetKeyPressing('D') || Input::GetKeyPressing(VK_RIGHT))
        vel.x +=  speed;

	if (rb->colliderInfo.bottom)
	{
		canJump = true;
		isBouncing = false;
	}
	else
	{
		canJump = false;
	}

	if (bounce && rb->colliderInfo.bottom)
	{
		vel.y += MaxJumpVelocity * 1.5;
		isBouncing = true;
	}

	if (Input::GetKeyDown('W') || Input::GetKeyDown(VK_UP) || Input::GetKeyDown(VK_SPACE))
	{
		Jump(vel);
	}
	else if ((Input::GetKeyUp(VK_SPACE) || Input::GetKeyUp('W') || Input::GetKeyUp(VK_UP)) && !isBouncing)
	{
		if (vel.y < -9)
			vel = Vector2::up * 9;
	}
	
	vel.y += gravity * Time::deltaTime;
	rb->velocity = vel * tiledPixel * Time::deltaTime;

    if (Input::GetKeyDown(VK_F4))
    {
        auto gobj = GameObject::findGameObjectByName("YellowBox");
        if(gobj != nullptr)
        { 
            gobj->collider->SetEnable(!gobj->collider->GetEnable());
			gobj->spriteRenderer->SetEnable(!gobj->spriteRenderer->GetEnable());
        }
    }

	bounce = false;
}

void Player::OnRecivedBoardcast(BoardcastMessageData bmd)
{
}

void Player::OnDrawGizmos(CDC * pDC)
{
	pDC->TextOutA(0, 20, ("Now Room : " + roomName).c_str());
	pDC->TextOutA(0, 40, ("Player Pos : " + this->transform->GetWorldPosition().toString()).c_str());
}

void Player::SetRoomName(string name)
{
	roomName = name;

	auto gobj = GameObject::findGameObjectByName("YellowBox");
	if (gobj != nullptr)
	{
		if (name == "Yellow")
		{
			this->gameObject->collider->SetEnable(true);
			this->gameObject->spriteRenderer->SetEnable(true);
		}
		else
		{
			this->gameObject->collider->SetEnable(false);
			this->gameObject->spriteRenderer->SetEnable(false);
		}
	}
}

string Player::GetRoomName()
{
	return roomName;
}

void Player::Jump(Vector2 & velocity)
{
	if (canJump)
	{
		velocity.y = MaxJumpVelocity;
		canJump = false;
	}
}
