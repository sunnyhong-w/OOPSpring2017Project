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
    rb->TimeSliceCollision = true;
	
	int speed = 5;
    //vel = rb->velocity;

    if (rb->colliderInfo.bottom || rb->colliderInfo.top)
        vel.y = 0;

    vel.x = 0;

	float extraGravityHack = 0;
	if (rb->colliderInfo.bottom)
		extraGravityHack = 1;

    if (Input::GetKeyClick(VK_LBUTTON))
        this->transform->SetWorldPosition(Input::GetMouseWorldPos());

	if (Input::GetKeyPressing('A') || Input::GetKeyPressing(VK_LEFT))
        vel.x = -1 * speed;
	if (Input::GetKeyPressing('D') || Input::GetKeyPressing(VK_RIGHT))
        vel.x =  speed;
	//TRACE(("\ncolliderINFO Bottom : " + to_string(rb->colliderInfo.bottom) + "\n").c_str());
	if (rb->colliderInfo.bottom)
	{
		canJump = true;
	}

	if (Input::GetKeyDown('W') || Input::GetKeyDown(VK_SPACE))
	{
		Jump(vel);
	}
	vel.y += gravity * Time::deltaTime;
	rb->velocity = vel * tiledPixel * Time::deltaTime + extraGravityHack * Vector2::down;
			////

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
    if (Input::GetKeyDown(VK_F5))
        GameScene::NowScene()->LoadScene("Main");
	Vector2 size = Vector2((float)SIZE_X / 2, (float)SIZE_Y / 2);
	GameScene::CameraPosition() = (this->transform->GetPostion() - size).GetV2I();
}

void Tutorial::OnRecivedBoardcast(BoardcastMessageData bmd)
{
}

void Tutorial::OnDrawGizmos(CDC * pDC)
{
	Rigidbody *rb = this->gameObject->GetComponent<Rigidbody>();
	pDC->TextOutA(0, 20, ("colliderINFO : " + rb->colliderInfo.toString()).c_str());
	pDC->TextOutA(0, 40, ("Can Jump : " + to_string(canJump)).c_str());
	pDC->TextOutA(0, 60, ("velocity : " + rb->velocity.toString()).c_str());
}

void Tutorial::Jump(Vector2 & velocity)
{
	if (canJump)
	{
		velocity.y = MaxJumpVelocity;
		canJump = false;
	}
}
