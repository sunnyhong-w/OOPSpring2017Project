#include "stdafx.h"
#include "Engine.h"
#include "Camera.h" //Include you header

void Camera::Start()
{
	player = nullptr;
	//GameScene::CameraPosition() = GameObject::findGameObjectByName("Player")->transform->GetPostion().GetV2I();
}

void Camera::LateUpdate()
{
	if(player == nullptr)
		player = GameObject::findGameObjectByName("Player");
	else
	{
		GameScene::CameraPosition() = this->gameObject->spriteRenderer->GetRealRenderPostion();
		playerPos = player->transform->GetPostion();
		Vector2::Damp(this->transform->GetPostion(), playerPos, velocity, 0.0001f, 1, 0.5f);
		
		this->gameObject->rigidbody->velocity = (velocity * Time::deltaTime * 8).floorSpecial();
	}
}
