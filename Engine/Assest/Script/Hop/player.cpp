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
	else if (Input::GetKeyUp(VK_SPACE) && !isBouncing)
	{
		if (vel.y < -9)
			vel = Vector2::up * 9;
	}
	


	vel.y += gravity * Time::deltaTime;
	rb->velocity = vel * tiledPixel * Time::deltaTime;



    if (Input::GetKeyPressing(VK_NUMPAD8))
        GameScene::CameraPosition() = GameScene::CameraPosition() + Vector2I::up * 10;
    if (Input::GetKeyPressing(VK_NUMPAD6))
        GameScene::CameraPosition() = GameScene::CameraPosition() + Vector2I::right * 10;
    if (Input::GetKeyPressing(VK_NUMPAD4))
        GameScene::CameraPosition() = GameScene::CameraPosition() + Vector2I::left * 10;
    if (Input::GetKeyPressing(VK_NUMPAD2))
        GameScene::CameraPosition() = GameScene::CameraPosition() + Vector2I::down * 10;
	if (Input::GetKeyDown(VK_NUMPAD5))
	{
		//GameScene::CameraPosition() = (this->transform->GetPostion() - size).GetV2I();
	}

	Vector2 size = Vector2((float)SIZE_X / 2, (float)SIZE_Y / 2);
	
    if (!Input::GetKeyPressing(VK_F5))
        GameScene::CameraPosition() = (this->transform->GetPostion() - size).GetV2I();

    if (Input::GetKeyDown(VK_F4))
    {
        auto gobj = GameObject::findGameObjectByName("YellowBox");
        if(gobj != nullptr)
        { 
            gobj->collider->SetEnable(!gobj->collider->GetEnable());
        }
    }

	bounce = false;
}

void Player::OnRecivedBoardcast(BoardcastMessageData bmd)
{
}

void Player::OnDrawGizmos(CDC * pDC)
{
	Rigidbody *rb = this->gameObject->rigidbody;
	pDC->TextOutA(0, 20, ("colliderINFO : " + rb->colliderInfo.toString()).c_str());
	pDC->TextOutA(0, 40, ("Can Jump : " + to_string(canJump)).c_str());
	pDC->TextOutA(0, 60, ("velocity : " + rb->velocity.toString()).c_str());
	pDC->TextOutA(0, 80, ("vel y : " + to_string(vel.y)).c_str());
}

void Player::Jump(Vector2 & velocity)
{
	if (canJump)
	{
		velocity.y = MaxJumpVelocity;
		canJump = false;
	}
}
