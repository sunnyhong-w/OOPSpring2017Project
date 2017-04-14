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
    if (Input::GetKeyPressing(VK_F5))
    {
        if (Input::GetKeyPressing(VK_NUMPAD8))
            GameScene::CameraPosition() = GameScene::CameraPosition() + Vector2I::up * 10;
        if (Input::GetKeyPressing(VK_NUMPAD6))
            GameScene::CameraPosition() = GameScene::CameraPosition() + Vector2I::right * 10;
        if (Input::GetKeyPressing(VK_NUMPAD4))
            GameScene::CameraPosition() = GameScene::CameraPosition() + Vector2I::left * 10;
        if (Input::GetKeyPressing(VK_NUMPAD2))
            GameScene::CameraPosition() = GameScene::CameraPosition() + Vector2I::down * 10;
        if (Input::GetKeyDown(VK_NUMPAD5))
            GameScene::CameraPosition() = (this->transform->GetPostion() - Vector2(((float)SIZE_X / 2), ((float)SIZE_Y / 2))).GetV2I();
    }
    else
    {
        if (player == nullptr)
        {
            player = GameObject::findGameObjectByName("Player");
            if (player != nullptr)
            {
                this->transform->SetPosition(player->transform->GetWorldPosition());
                GameScene::CameraPosition() = this->gameObject->spriteRenderer->GetRealRenderPostion();
                this->gameObject->rigidbody->velocity = Vector2::zero;
            }
        }
        else
        {
            GameScene::CameraPosition() = this->gameObject->spriteRenderer->GetRealRenderPostion();
            playerPos = player->transform->GetWorldPosition() + (player->collider->collisionInfo.size / 2);
			float playerCamerPosY = player->transform->GetWorldPosition().y - GameScene::CameraPosition().y;
			if (playerCamerPosY + player->collider->collisionInfo.size.y > 250)
			{
				this->gameObject->rigidbody->velocity.y =player->rigidbody->velocity.y;
			}
			else if(playerCamerPosY  <= 60)
			{
				if (player->rigidbody->velocity.y < 0)
				{
					this->gameObject->rigidbody->velocity = player->rigidbody->velocity;
				}
			}
			else
			{
            Vector2::Damp(this->transform->GetWorldPosition(), playerPos, velocity, 0.0001f, 1, 0.5f);
            this->gameObject->rigidbody->velocity = (velocity * Time::deltaTime * 8).floorSpecial();
			}
        }
    }
}

void Camera::OnDrawGizmos(CDC * pDC)
{
	game_framework::CDDraw::DrawLine(pDC, Vector2I(0, 250), Vector2I(480, 250), RGB(255,0,0));

}
