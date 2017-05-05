#include "stdafx.h"
#include "Engine.h"
#include "Camera.h" //Include you header

void Camera::Start()
{
	player = nullptr;
	this->RegisterEvent(BroadcastEvent::UpdateBoxPosition);
	shouldShake = false;
	time = 0;
	
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
				playerPos = player->transform->GetWorldPosition() + (player->collider->collisionInfo.size / 2);
                this->transform->SetPosition(playerPos);
                GameScene::CameraPosition() = this->gameObject->spriteRenderer->GetRealRenderPostion();
                this->gameObject->rigidbody->velocity = Vector2::zero;
            }
        }
        else
        {
            GameScene::CameraPosition() = this->gameObject->spriteRenderer->GetRealRenderPostion();
            playerPos = player->transform->GetWorldPosition() + (player->collider->collisionInfo.size / 2);
			float playerCamerPosY = player->transform->GetWorldPosition().y - GameScene::CameraPosition().y;

			Vector2::Damp(this->transform->GetWorldPosition(), playerPos, velocity, 0.0001f, 1, 0.5f);
			this->gameObject->rigidbody->velocity = (velocity * Time::deltaTime * 8).floorSpecial();

			if (playerCamerPosY + player->collider->collisionInfo.size.y > 250)
			{
				if(player->rigidbody->velocity.y > 0)
				this->gameObject->rigidbody->velocity.y = player->rigidbody->velocity.y;
			}
			else if(playerCamerPosY  <= 60)
			{
				if (player->rigidbody->velocity.y < 0)
				{
					this->gameObject->rigidbody->velocity.y = player->rigidbody->velocity.y;
				}
			}
        }
		
    }	
	ShakeF();

}

void Camera::OnDrawGizmos(CDC * pDC)
{
	game_framework::CDDraw::DrawLine(pDC, Vector2I(200, 250), Vector2I(280, 250), RGB(255,0,0));
}

void Camera::OnCollisionEnter(Collider * c)
{
	if (!Input::GetKeyPressing(VK_F5))
	{
		string cname = c->gameObject->GetName();

		auto position = this->gameObject->spriteRenderer->GetRealRenderPostion();

		if (cname.find("Top") != string::npos)
		{
			int dy = (c->transform->GetWorldPosition().y + c->collisionInfo.size.y) - position.y;
			this->transform->Translate(0, dy);
		}
		else if (cname.find("Bottom") != string::npos)
		{
			int dy = c->transform->GetWorldPosition().y - position.y - this->gameObject->collider->collisionInfo.size.y;
			this->transform->Translate(0, dy);
		}
		else if (cname.find("Right") != string::npos)
		{
			int dx = c->transform->GetWorldPosition().x - position.x - this->gameObject->collider->collisionInfo.size.x;
			this->transform->Translate(dx, 0);
		}
		else if (cname.find("Left") != string::npos)
		{
			int dx = (c->transform->GetWorldPosition().x + c->collisionInfo.size.x) - position.x;
			this->transform->Translate(dx, 0);
		}
	}
}

void Camera::OnCollisionStay(Collider * c)
{
	OnCollisionEnter(c);
}

void Camera::OnRecivedBroadcast(BroadcastMessageData bmd)
{
	if (bmd.event == BroadcastEvent::UpdateBoxPosition) 
	{
		Shake(0.75, 30, 15);
		
	}
}

void Camera::Shake(double t, double f, double s)
{
	f = f*(rand() % 2 == 0 ? 1 : -1);
	totalTime = t;
	frequency = f;
	strength = s;
	time = 0;
	shouldShake = true;
}

void Camera::ShakeF()
{
	if (shouldShake)
	{
		time += Time::deltaTime;
		Vector2I offset = this->gameObject->spriteRenderer->GetOffset();
		this->gameObject->spriteRenderer->SetOffset(Vector2(sin((totalTime - time) * frequency )*(totalTime - time) * strength,
			sin((totalTime - time) * frequency + 1)*(totalTime - time) * strength).GetV2I());
		if (time > totalTime)
		{
			time = 0;
			shouldShake = false;
		}
	}
}
