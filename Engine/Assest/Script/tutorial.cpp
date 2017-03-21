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
	
	int speed = 500;
    //vel = rb->velocity;

    if (rb->colliderInfo.bottom || rb->colliderInfo.top)
        vel.y = 0;

    vel.x = 0;

    if (Input::GetKeyClick(VK_LBUTTON))
        this->transform->SetWorldPosition(Input::GetMouseWorldPos());

	if (Input::GetKeyPressing('A') || Input::GetKeyPressing(VK_LEFT))
        vel.x = -1 * speed;
	if (Input::GetKeyPressing('D') || Input::GetKeyPressing(VK_RIGHT))
        vel.x =  speed;
    if (Input::GetKeyDown('W') || Input::GetKeyDown(VK_SPACE))
        vel.y = MaxJumpVelocity;

    vel.y += gravity * Time::deltaTime;
    rb->velocity = vel * tiledPixel * Time::deltaTime;

    ////

    if (Input::GetKeyPressing(VK_NUMPAD8))
        GameScene::CameraPosition() = GameScene::CameraPosition() + Vector2I::up * 10;
    if (Input::GetKeyPressing(VK_NUMPAD6))
        GameScene::CameraPosition() = GameScene::CameraPosition() + Vector2I::right * 10;
    if (Input::GetKeyPressing(VK_NUMPAD4))
        GameScene::CameraPosition() = GameScene::CameraPosition() + Vector2I::left * 10;
    if (Input::GetKeyPressing(VK_NUMPAD2))
        GameScene::CameraPosition() = GameScene::CameraPosition() + Vector2I::down * 10;

    if (Input::GetKeyDown(VK_F5))
        GameScene::NowScene()->LoadScene("Main");
       
}

void Tutorial::OnRecivedBoardcast(json j)
{
}

void Tutorial::Jump(Vector2 & velocity)
{
    velocity.y = MaxJumpVelocity;
}
