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
	waitSmoke = 0;
	AudioPlayer::GetSource("RSIC")->Play();
}

void Player::Update()
{
    Rigidbody *rb = gameObject->rigidbody;
    rb->TimeSliceCollision = true;
	
	int speed = 2;

    if (rb->colliderInfo.bottom || rb->colliderInfo.top)
        vel.y = 0;

    vel.x = 0;

    if (Input::GetKeyPressing('A') || Input::GetKeyPressing(VK_LEFT))
    {
        vel.x += -1 * speed;
        this->gameObject->spriteRenderer->SetFlipX(true);
    }

    if (Input::GetKeyPressing('D') || Input::GetKeyPressing(VK_RIGHT))
    {
        vel.x += speed;
        this->gameObject->spriteRenderer->SetFlipX(false);
    }

	if (Input::GetKeyDown('A') || Input::GetKeyDown(VK_LEFT) || Input::GetKeyDown('D') || Input::GetKeyDown(VK_RIGHT))
		waitSmoke = 0.2f;

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

    if (Input::GetKeyDown('V'))
        this->gameObject->spriteRenderer->SetFlipX(!this->gameObject->spriteRenderer->GetFlipX());

    if (Input::GetKeyDown('H'))
        this->gameObject->spriteRenderer->SetFlipY(!this->gameObject->spriteRenderer->GetFlipY());

    if (Input::GetKeyDown('B'))
        this->gameObject->spriteRenderer->SetFlip(!this->gameObject->spriteRenderer->GetFlipX(), !this->gameObject->spriteRenderer->GetFlipY());

	
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

    if (Input::GetKeyDown(VK_F3))
    {
        GameObject *gobj = GameObject::findGameObjectByName("Anim");
        if (gobj != nullptr)
        {
            gobj->animationController->PlayOneShot("RightShine");
        }
    }

	if (this->gameObject->rigidbody->velocity.x!= 0 && waitSmoke < 0 && this->gameObject->rigidbody->colliderInfo.bottom == true)
	{
		waitSmoke = 0.15f;
		GameObject* gobj = InstantiateJSON(GameObject::GetPrefrabs("Smoke"));
		gobj->transform->SetWorldPosition(this->transform->GetWorldPosition()+Vector2(0,12));
	}

	bounce = false;
	waitSmoke -= Time::deltaTime;
}

void Player::OnRecivedBroadcast(BroadcastMessageData bmd)
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
			gobj->collider->SetEnable(true);
			gobj->spriteRenderer->SetEnable(true);
		}
		else
		{
			gobj->collider->SetEnable(false);
			gobj->spriteRenderer->SetEnable(false);
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
		AudioPlayer::GetSource("Jump")->PlayOneShot();
		velocity.y = MaxJumpVelocity;
		canJump = false;
	}
}
